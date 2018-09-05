#ifndef TIMEUTIL_HPP_
#define TIMEUTIL_HPP_

#include <string>
#include <chrono>
#include <iostream>
#include <ctime>
#include <time.h>
#include <iomanip>

#ifdef _WIN32
#include "Time_MSVC.hpp"
#elif __APPLE__

#elif __linux__

#elif __unix__
#endif

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif



#define elf_t elf::Time

/**
  common-name:
    tt -> std::time_t
    t  -> struct tm
    ts -> time_stamp
    dt -> date_time
*/

namespace elf
{
  typedef std::chrono::duration<int, std::ratio<1 * 60 * 60 * 24>> day_t;
  typedef std::chrono::duration<int, std::ratio<1 * 60 * 60>> hour_t;
  typedef std::chrono::duration<int, std::ratio<1 * 60, 1>> minute_t;
  typedef std::chrono::duration<int, std::ratio<1, 1>> second_t;
  typedef std::chrono::duration<int, std::ratio<1, 1000>> milli_t;
  typedef std::chrono::duration<int, std::ratio<1, 1000000>> mirco_t;
  typedef std::chrono::duration<int, std::ratio<1, 1000000000>> nano_t;
  extern const auto DATE_TIME_NORMAL_A = "%Y-%m-%d %H:%M:%S";
  extern const auto DATE_TIME_NORMAL_W = L"%Y-%m-%d %H:%M:%S";


#define TIME_T
#define TM
#define TS
#define COMMON

#ifdef UNICODE
#define time timeW
#define toString toStringW
#else
#define time timeA
#define toString toStringA
#endif


  class Time
  {
  private:
    explicit Time() = default;
    ~Time() = default;

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
      auto time_stamp = now_tt();
      tm t = {};
#if defined(_WIN32) && defined(_MSC_VER)
      localtime_s(&t, &time_stamp);
#else
      localtime_r(&time_stamp, &t);
#endif
      return c_str(t);
    }

    static std::tm parse(const std::string &date_time, const char *const pattern = nullptr)
    {
      std::tm t = { 0 };
#if defined(_WIN32) && defined(_MSC_VER)
      msvc_elf::strptime_msvc(date_time.c_str(), nullptr == pattern ? DATE_TIME_NORMAL_A : pattern, &t);
#else
      strptime(date_time.c_str(), nullptr == pattern ? DATE_TIME_NORMAL_A : pattern, &t);
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
      struct tm result = { 0 };
#ifdef _WIN32
      localtime_s(&t, &tt);
#else
      localtime_r(&tt, &result);
#endif
      return result;
    }

    inline static std::string c_str(struct tm & t, const char *const pattern = nullptr)
    {
      std::string date_time(24, '\0');
      std::size_t size = std::strftime(const_cast<char *>(date_time.data()),
        24,
        nullptr == pattern ? DATE_TIME_NORMAL_A : pattern,
        &t);
      if (0 != size)
      {
        date_time = std::string(size + 1, '\0');
        std::strftime(const_cast<char *>(date_time.data()),
          size,
          nullptr == pattern ? DATE_TIME_NORMAL_A : pattern,
          &t);
      }
      return date_time;
    }

COMMON
public:
  static std::string timeA(const char * const pattern = nullptr)
  {
    auto t = now_t();
    return toStringA(t, pattern);
  }
  static std::wstring timeW(const wchar_t * const pattern = nullptr)
  {
    auto t = now_t();
    return toStringW(t, pattern);
  }

  static std::string toStringA(struct tm & t, const  char * const pattern = nullptr) 
  {
    std::string dt(24, '\0');
    auto size = strftime(const_cast<char *>(dt.data()),
      24,
      nullptr == pattern ? DATE_TIME_NORMAL_A : pattern,
      &t);
    if (0 == size) 
    {
      dt = std::string(48, '\0');
      strftime(const_cast<char *>(dt.data()),
        48,
        nullptr == pattern ? DATE_TIME_NORMAL_A : pattern,
        &t);
    }
    return dt;
  }
  static std::string toStringA(std::time_t tt, const char * const pattern = nullptr)
  {
    auto t = toT(tt);
    return toStringA(t, pattern);
  }

  static std::wstring toStringW(struct tm & t, const  wchar_t * const  pattern = nullptr)
  {
    //TODO
    std::wstring dt(24, '\0');
    auto size = wcsftime(const_cast<wchar_t *>(dt.data()),
      24,
      nullptr == pattern ? DATE_TIME_NORMAL_W : pattern,
      &t);
    if (0 == size)
    {
      dt = std::wstring(48, '\0');
      wcsftime(const_cast<wchar_t *>(dt.data()),
        48,
        nullptr == pattern ? DATE_TIME_NORMAL_W : pattern,
        &t);
    }
    return dt;
  }
  static std::wstring toStringW(std::time_t tt, const wchar_t * const pattern = nullptr)
  {
    //TODO
    auto t = toT(tt);
    return toStringW(t, pattern);

  }

TM
public:
  /**
    timestamp based on struct tm
  */
  inline static struct tm now_t()
  {
    auto tt = now_tt();
    auto t = toT(tt);
    return t;
  }
  /**
    struct tm to std::time_t
  */
  inline static std::time_t toTt(IN struct tm & t)
  {
    return mktime(&t);
  }


TIME_T
public:
  /**
    timestamp based on std::time_t
  **/
  inline static std::time_t now_tt()
  {
    return std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  }
  
  /**
    std::time_t to struct tm
  */
  inline static struct tm toT(IN std::time_t tt) {
    struct tm t = { 0 };
#ifdef _WIN32
    localtime_s(&t, &tt);
#else
    localtime_r(&tt, &t);
#endif
    return t;
  }

  private:
  };
} // namespace elf

#endif