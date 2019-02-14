#include <iostream>
#include <algorithm>
#include <random>
#include <chrono>

#include "./pool/ThreadPool.hpp"

int main(int argc, char** argv){
    ThreadPool pool(4);
    std::default_random_engine rd;
    std::uniform_int_distribution<int> rang(100, 1000);
    for(int idx = 0; idx < 20; ++idx){
        pool.commit([=](int x, int y, int t){
            std::cout << "thread id : " << std::this_thread::get_id() 
                << " x = " << x << " y = " << y <<
                " sleep time = " << t << " ms" <<
                " id = " << idx << std::endl;
                
            std::this_thread::sleep_for(std::chrono::milliseconds(t));
        }, rang(rd), rang(rd), rang(rd));
    }
    std::vector<std::future<int> > results;
    for (auto index = 20; index < 50; ++index){
        results.push_back(
            pool.commit([=]()->int{
                return index;
            })
        );
    }
    for ( auto & r : results){
        std::cout << "get result from thread "
        << " index = " << r.get() << std::endl;
    }
    char command = std::cin.get();
    if (command == 'q'){
        pool.shutdown(true);
    }else if (command == 'e'){
        pool.shutdown(true);
        try
        {
            pool.commit([](){
                std::cout << "i want to get an exception" << std::endl;
            });
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        } 
    }
    std::cout << "test finish, OY!" << std::endl;
    return 0;
}