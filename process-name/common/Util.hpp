#ifndef _UTIL_H_
#define _UTIL_H_

#include <string>   //strlen(2) strdup(1)
#include <math.h> //min(1)

/**
 * origin: 进程原始名
 * base: char数组, 存储修改后的进程名
 * nul:
 * end: 
 * 
*/
typedef struct _PROC_TITLE_INFO{
    const char * origin;
    char * base, *nul, *end;
}PTI, *PPTI;
extern char ** environ;

class Util{
private:
    Util() = default;
    ~Util() = default;

public:
    /**
     * gloabl init function, keep it be called only once
     * 
    */
    static PPTI init_proc_title_info(int argc, char ** argv){
        auto ** envp = environ;
        auto base = argv[0];
        if(nullptr == base){
            return nullptr;
        }
        char * end = (&base[strlen(base)]) + 1; // +1 for '\0'
        for (auto idx = 0; idx < argc || (idx >= argc && nullptr != argv[idx]); ++idx){
            // 不知道redis为什么这样写, 我猜可能是遇到特殊的argv, 待探索
            if(nullptr == argv[idx] || argv[idx] < end){    //注意这里比较的都是指针,不涉及到数据
                continue;
            }
            end = argv[idx] + strlen(argv[idx]) + 1; // +1 for '\0'
        }

        for (auto idx = 0; nullptr != envp[idx]; ++idx){
            if (envp[idx] < end){   //注意这里比较的都是指针,不涉及到数据
                continue;
            }
            end = envp[idx] + strlen(envp[idx]) + 1;    // +1 for '\0'
        }
        PPTI ppti = new PTI();
        ppti->origin = strdup(argv[0]);
        if(nullptr == ppti->origin){
            delete ppti;
            ppti = nullptr;
            return ppti;
        }
    #ifdef __GLIBC__
        //TODO
    #elif __APPLE__
        auto name = strdup(getprogname());
        if(nullptr == name){
            free(const_cast<char *>(ppti->origin));
            delete ppti;
            ppti = nullptr;
            return ppti;
        }
        setprogname(name);
    #endif
        if( !set_new_env(environ) ){
            free(const_cast<char *>(ppti->origin));
            delete ppti;
            ppti = nullptr;
            return ppti;
        }

        if( !set_new_argv(argc, argv) ){
            free(const_cast<char *>(ppti->origin));
            delete ppti;
            ppti = nullptr;
            return ppti;
        }
        ppti->base = base;
        ppti->end = end;
        return ppti;
    }

    static bool set_proc_title(const PPTI ppti, const char * fmt, ...){
        if(nullptr == ppti){
            return false;
        }
        if(nullptr == ppti->base){
            return false;
        }

        char buf[256] = {0};
        va_list ap;
        auto len = 0;
        if(nullptr != fmt){
            va_start(ap, fmt);
            len = vsnprintf(buf, sizeof buf, fmt, ap);
            va_end(ap);
        }else{
            len = snprintf(buf, sizeof buf, "%s", ppti->origin);
        }
        if(len <= 0){
            return false;
        }
        memset(ppti->base, 0, len + 1);
        memcpy(ppti->base, buf, len);
        return true;
    } 
    


private:
    template<class T>
    static T min(T && l, T && r){
        return l > r ? r : l;
    }

    static bool set_new_argv(int argc, char ** argv){
        for(auto idx = 1; idx < argc || (idx >= argc && nullptr != argv[idx]); ++idx){
            if(nullptr == argv[idx]){
                continue;
            }
            auto arg = strdup(argv[idx]);
            if(nullptr == arg){
                return false;
            }
            argv[idx] = arg;
        }
        return true;
    }

    static bool set_new_env(char ** old_env){
        extern char ** environ;
        if( environ != old_env){
            return true;
        }
        if(! clear_env()){
            environ = old_env;
            return false;
        }
        char * eq = nullptr;
        for (auto idx = 0; nullptr != old_env[idx]; ++idx){
            eq = strchr(old_env[idx], '=');
            if(nullptr == eq){
                continue;
            }
            *eq = '\0';
            int result = setenv(old_env[idx], eq + 1, true);
            *eq = '=';
            if(0 != result){
                environ = old_env;
                return false;
            }
        }
        return true;
    }

    static bool clear_env(void){
    #ifdef __GLIBC__
        //TODO
    #else
        extern char ** environ;

        /**
         * 相当于 char * arr[1] = {nullptr};
         * static char ** temp_env = statc_cast<char **>(arr);
         * 其主要目的是为了使environ变成空置,从而使environ失效
         * 关于这个函数的实现请点击下面的链接查看redis作者对其的说明和吐槽
         * For discussion on the portability of the various methods, see
         * http://lists.freebsd.org/pipermail/freebsd-stable/2008-June/043136.html
         */
        static char ** temp_env = static_cast<char **>(malloc(sizeof *temp_env));
        if(nullptr == temp_env){
            return false;
        }
        temp_env[0] = nullptr;
        environ = temp_env;
        return true;
    #endif
    }
};
#endif