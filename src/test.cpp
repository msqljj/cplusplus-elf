#include <iostream>
//#include <chrono>
#include <algorithm>
#include "hpp/Time.hpp"
//#define _MSC_VER 8

int main(int argc, char const *argv[])
{
   std::cout << elf_t::now_tt() << std::endl;
    auto t = elf_t::now_t();
    auto dt = elf_t::toStringA(t);

    std::cout << dt << std::endl;
    std::cout << elf_t::toStringA(elf_t::now_t()) << std::endl;

    std::cout << elf_t::toStringA(elf_t::now_tt()) << std::endl;

    std::cout << elf_t::timeA() << std::endl;

    std::wcout << elf_t::timeW() << std::endl;

    std::wcerr << elf_t::time() << std::endl;

    std::cout << elf_t::tidyA() << std::endl;
    std::wcout << elf_t::tidyW() << std::endl;

    std::cout << "______________________________________" << std::endl;
    std::vector<struct tm> v = elf_t::listTidyHours(10);
    std::for_each(v.cbegin(), v.cend(), [](const tm & t) ->void{
        struct tm r = t;
        std::cout <<  "list tidy hours " <<  elf_t::toStringA(r) << std::endl;
    });

    std::cin.get();
    return 0;
}
