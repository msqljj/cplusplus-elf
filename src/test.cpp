#include <iostream>
//#include <chrono>
#include <algorithm>
#include "hpp/time.hpp"
//#define _MSC_VER 8

int main(int argc, char const *argv[])
{
   UN_USED(argc)
    UN_USED(argv)

    /**
        %Y year
        %m month
        %d day
        %H hour
        %M minute
        %S second
    */
    auto start = std::chrono::steady_clock::now();
    std::cout << "date time now test : " << std::endl;
    std::cout << "\tdate_time_normal_pat : " << elf_t::timeA() << std::endl;
    std::cout << "\tdate_time_tiny_pat : " << elf_t::timeA("%Y%m%d%H%M%S") << std::endl;
    std::cout << "\tstd::time_t time stamp : " << elf_t::now_tt() << std::endl;
    std::cout << "\tstruct tm time stamp : " << elf_t::toStringA(elf_t::now_t()) << std::endl;
    std::cout << std::endl;

    std::cout << "date time parse test: " << std::endl;
    std::string dt0("2018-01-29 11:22:33");
    auto t0 = elf_t::parse_t(dt0);
    auto tt0 = elf_t::parse_tt(dt0);
    std::cout << "\tstruct tm parse from \"" << dt0 << "\" : " << elf_t::toStringA(t0) << std::endl;
    std::cout << "\tstd::time_t parse from \"" << dt0 << "\" : " << tt0 << std::endl;
    std::cout << std::endl;


    std::cout << "list past years month days hours minutes second test : " << std::endl;
    auto past = 3;
    std::vector<struct tm> v_past = elf_t::pastHours(past);
    std::for_each(v_past.cbegin(), v_past.cend(), [past](const tm & t) ->void {
        std::cout << "\tpast " << past << " hours " << elf_t::toStringA(const_cast<struct tm &>(t)) << std::endl;
    });

    std::vector<struct tm> v_tidy_past = elf_t::pastTidyHours(past);
    std::for_each(v_tidy_past.cbegin(), v_tidy_past.cend(), [past](const tm & t) ->void {
        std::cout << "\tpast " << past << " tidy hours " << elf_t::toStringA(const_cast<struct tm &>(t)) << std::endl;
    });
    std::cout << std::endl;


    std::cout << "date time calculate test :" << std::endl;
    std::string dt1("2018-01-29 11:22:33");
    auto t1 = elf_t::parse_t(dt1);
    auto t2 = elf_t::plusHours_t(t1, 10);
    std::cout << "\tdate time : " << dt1 << " plus " << 10 << " hours is : " << elf_t::toStringA(t2) << std::endl;
    std::cout << "\tdate time : " << dt1 << " plus " << -20 << " hours is : " << elf_t::toStringA(elf_t::plusHours_t(elf_t::parse_t(dt1), -20)) << std::endl;
    std::cout << "\tnow min : " << elf_t::toStringA(elf_t::min_t()) << std::endl;
    std::cout << "\tnow max : " << elf_t::toStringA(elf_t::max_t()) << std::endl;

    std::cout << "\tnow min auto : " << elf_t::minA() << std::endl;
    std::cout << "\tnow max auto : " << elf_t::maxA() << std::endl;

    std::cout << "\tdate time : " << dt1 << " min : " << elf_t::minA(t1) << std::endl;
    std::cout << "\tdate time : " << dt1 << " max : " << elf_t::maxA(t1) << std::endl;
    std::cout << std::endl;
    auto end = std::chrono::steady_clock::now();
    
    std::cout << "cost time : " << std::chrono::duration_cast<elf::milli_t>(elf::nano_t(end - start)).count() << std::endl;

    std::cin.get();
    return 0;
}
