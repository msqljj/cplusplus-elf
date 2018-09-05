#ifndef TIMEUTIL_HPP_
#define TIMEUTIL_HPP_

#include <string>
#include <chrono>
#include <iostream>
//#include <ctime>
#include <time.h>
#include <iomanip>

#ifdef _WIN32
#include "time_mscv.hpp"
#elif __APPLE__

#elif __linux__

#elif __unix__
#endif

#define elf_t elf::TimeUtil

namespace elf
{
typedef std::chrono::duration<int, std::ratio<1 * 60 * 60 * 24>> day_t;
typedef std::chrono::duration<int, std::ratio<1 * 60 * 60>> hour_t;
typedef std::chrono::duration<int, std::ratio<1 * 60, 1>> minute_t;
typedef std::chrono::duration<int, std::ratio<1, 1>> second_t;
typedef std::chrono::duration<int, std::ratio<1, 1000>> milli_t;
typedef std::chrono::duration<int, std::ratio<1, 1000000>> mirco_t;
typedef std::chrono::duration<int, std::ratio<1, 1000000000>> nano_t;
extern const auto DATE_TIME_NORMAL = "%Y-%m-%d %H:%M:%S";
class TimeUtil
{
private:
  explicit TimeUtil() = default;
  ~TimeUtil() = default;

public:
  /* @deprecated
  static std::string now(std::size_t length = 0, const char *const pattern = nullptr)
  {
    auto time_stamp = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()); //std::time(nullptr);
    auto t = localtime(&time_stamp);
    std::string date_time(0 == length ? 24 : length, '\0');
    std::strftime(const_cast<char *>(date_time.data()),
                  24,
                  nullptr == pattern ? DATE_TIME_NORMAL : pattern,
                  std::localtime(&time_stamp));
    return date_time;
  }
  */

  static std::string now(const char *const pattern = nullptr)
  {
    std::string date_time(24, '\0');
    auto time_stamp = stamp();
    tm t = {};
    localtime_r(&time_stamp, &t);
    return c_str(t);
  }

  static std::tm parse(const std::string &date_time, const char *const pattern = nullptr)
  {
    std::tm t = {0};
#if defined(__GNUC__)
    strptime(date_time.c_str(), nullptr == pattern ? DATE_TIME_NORMAL : pattern, &t);
#elif defined(_MSC_VER)
    msvc_elf::strptime_msvc(date_time.c_str(), nullptr == pattern ? DATE_TIME_NORMAL : pattern, &t);
#endif
    return t;
  }
  // TODO month year need to add from struct tm;
  static std::time_t plusMonths(std::time_t t, int monthsToAdd)
  {
    return 0;
  }
  static std::time_t plusDays(std::time_t t, int daysToAdd)
  {
    if (0 == daysToAdd)
      return t;
    t += std::chrono::seconds(daysToAdd * 24 * 60 * 60 * 1).count();
    return t;
  }

  static std::time_t plusHours(std::time_t t, int hoursToAdd)
  {
    if (0 == hoursToAdd)
      return t;
    t += std::chrono::seconds(hoursToAdd * 60 * 60 * 1).count();
    return t;
  }

  static struct tm plusDays(struct tm t, int daysToAdd)
  {
    std::time_t tt = mktime(&t);
    tt += std::chrono::seconds(daysToAdd * 24 * 60 * 60 * 1).count();
    struct tm result = {0};
    localtime_r(&tt, &result);
    return result;
  }

  inline static std::string c_str(struct tm t, const char *const pattern = nullptr)
  {
    std::string date_time(24, '\0');
    std::size_t size = std::strftime(const_cast<char *>(date_time.data()),
                                     24,
                                     nullptr == pattern ? DATE_TIME_NORMAL : pattern,
                                     &t);
    if (0 != size)
    {
      date_time = std::string(size + 1, '\0');
      std::strftime(const_cast<char *>(date_time.data()),
                    size,
                    nullptr == pattern ? DATE_TIME_NORMAL : pattern,
                    &t);
    }
    return date_time;
  }
  inline static struct tm timetTotm(std::time_t t)
  {
    struct tm tt = {0};
    localtime_r(&t, &tt);
    return tt;
  }
  inline static std::time_t stamp()
  {
    return std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  }

  /*static std::string get_now(){
    std::time_t t = std::time(nullptr);
    tm r;
    localtime_r(&t, &r);
  }*/

private:
};
} // namespace elf

#endif