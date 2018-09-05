#include <iostream>
//#include <chrono>
#include "hpp/Time.hpp"
//#define _MSC_VER 8

int main(int argc, char const *argv[])
{
    std::cout << elf_t::now_tt() << std::endl;
    auto t = elf_t::now_t();
    auto dt = elf_t::toStringA(t);

    std::cout << dt << std::endl;
    auto t1 = elf_t::now_t();
    std::cout << elf_t::toStringA(t1) << std::endl;

    std::cout << elf_t::toStringA(elf_t::now_tt()) << std::endl;

    std::cout << elf_t::timeA() << std::endl;

    std::wcout << elf_t::timeW() << std::endl;

    std::cerr << elf_t::time() << std::endl;

    std::cin.get();
    return 0;
}
