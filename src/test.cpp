#include <iostream>
//#include <chrono>
#include "hpp/TimeUtil.hpp"
//#define _MSC_VER 8

int main(int argc, char const *argv[])
{
    /* code */
    auto date = elf_t::now();
    std::cout << static_cast<void *>(&date) << std::endl;
    std::cout << date << std::endl;
    auto start = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
    for (auto index = 0; index < 5; ++index)
    {
        auto time_ = elf_t::now();
        //std::cout << time_ << std::endl;
    }
    auto end = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
    std::cout << "thread safe time getter : " << (end - start).count() // std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
              << std::endl;

    std::cout << "------------------------------------" << std::endl;
    auto st1 =  elf_t::now();
    std::cout << st1 << std::endl;
    auto t1 = elf_t::parse(st1);
    std::cout << elf_t::c_str(t1) << std::endl;

    //struct tm t2 = elf_t::
    std::time_t t2 = elf_t::stamp();
    t2 = elf_t::plusDays(t2, -1);
    std::cout << elf_t::c_str(elf_t::timetTotm(t2)) << std::endl;
    std::cin.get();
    return 0;
}
