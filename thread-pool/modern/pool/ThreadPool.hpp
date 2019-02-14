#ifndef _THREAD_POOL_H
#define _THREAD_POOL_H

#include <memory>
#include <functional>
#include <future>
#include <condition_variable>
#include <queue>
#include <algorithm>
#include <vector>
#include <thread>
#include <typeinfo>
/**
 * 几个需要注意的点:
 * 1、tasks的读写锁需要优化成带优先级的锁, 可以肯定线程池的绝大部分使用场景commit task比run task更密集
 * 2、根据tasks以及cpu扩展线程数
 * 3、支持允许缓存的task数,如果超出此数将采取拒绝策略
 * 4、拒绝策略
*/
class ThreadPool{
public:
    ThreadPool(int core, int max = 0, int cache = 0): core(core),//由于max和cache暂时没用到,因此赋值0
                    max(max), cache(cache), quit(false), force(false){
        
    }  
    ~ThreadPool(){
        this->quit.store(true);
        this->enable.notify_all();
        std::for_each(this->pool.begin(), this->pool.end(), [](std::thread & t){
            if(t.joinable()){
                t.join();
            }
        });
    }
public:
    void start(){
for(auto idx = 0; idx < core; ++idx){
            pool.push_back(std::thread([this](){
                // 第一次退出,判断是否要强制退出
                bool quit = this->force.load() ? this->quit.load() : false;
                for(; !quit;){
                    std::unique_lock<std::mutex> lock(this->oper_lock);
                    this->enable.wait(lock, [this](){
                        return this->quit.load() || !this->tasks.empty();
                    });
                    // 不是强制退出时可从这里退出
                    if(this->quit.load() && this->tasks.empty()){
                        return;
                    }
                    std::function<void()> task = std::move(this->tasks.front());
                    this->tasks.pop();

                    task();
                }
            }));
        }
    }
    void shutdown(bool force = false){
        this->quit.store(true);
        this->force.store(force);
    }

    //void commit(std::function<void (void * param)> task);
    template<class T, class... Args>
    auto commit(T && t, Args&&...args)->std::future<decltype(t(args...))>{
        using TYPE = decltype(t(args...));
        if(this->quit.load()){
            //dont know return what, so throw an exception
            throw std::runtime_error("thread pool is alreay shutdown.");
        }
        // 1、std::packaged_task<decltype(f(args...))() 类似std::function\
            但是会将其封装的可调用元素的结果封装在std::future中
        // 2、std::make_shared 创建std::packaged_task<decltype(f(args...))()\
            类型的智能指针
        // 3、std::bind(std::forward<T>(t), std::forward<Args>(args)...)当做\
            std::packaged_task的构造参数
        auto task = std::make_shared<std::packaged_task<TYPE()> >( 
            std::bind(std::forward<T>(t), std::forward<Args>(args)...)
        );
        std::future<TYPE> result = task->get_future();
        std::lock_guard<std::mutex> lock(this->oper_lock);
        //将packaged_task 包裹在一个签名为void()的lambda函数中调用,因为此lambda函数符合std::function<void()>\
            的签名,所以可以放到queue中
        this->tasks.emplace([task](){
            (*task)();  //调用packaged_task
        });
        this->enable.notify_one();  // 在线程池中唤醒一个休眠的线程
        return result;
    }
private:
    //void move();



private:
    std::vector<std::thread> pool;
    std::queue<std::function<void()> > tasks;
    int core;   //线程池核心线程数
    int max;    //线程池根据tasks量以及cpu数最大可扩展的量
    int cache;  //运行tasks可缓存的最大task数,超出次数后commit将采取拒绝策略

    std::atomic<bool> quit;     //线程池shutdown条件, true时shutdown
    std::atomic<bool> force;    //是否强制shutdown,true时有剩余的task将不执行直接退出, false时等待执行完所有的task再退出
    std::condition_variable enable;     //
    std::mutex oper_lock;   // queue的读写锁
};



#endif