#include <iostream>
#include "./common/Util.hpp"

extern char **environ;

//#define UNUSED(x) (void)x;

int main(int argc, char ** argv){
    auto ppti = Util::init_proc_title_info( argc, argv);
    std::cout << ppti->origin << "-" << ppti->base << std::endl;;
    std::cout << Util::set_proc_title(ppti, "%s-%d", "named", 1) << std::endl;
    std::cout << getprogname() << std::endl;
    std::cout << getprogname() << std::endl;
    std::cout << ppti->end - ppti->base << std::endl;
}