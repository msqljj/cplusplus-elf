#ifndef TIMEUTIL_HPP_
#define TIMEUTIL_HPP_

#include <string>
#include <chrono>
#include <iostream>
#include <ctime>
#include <time.h>
#include <iomanip>
#include <vector>

#if defined(_WIN32) && defined(_MSC_VER)
#include "time_msvc.hpp"
#endif

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifndef UN_USED(x)
#define UN_USED(x) (void)x;
#endif


#define elf_t elf::Time

/**
common-name:
tt -> std::time_t
t  -> struct tm
ts -> time_stamp
dt -> date_time
pat -> pattern

DT_PAT_N date time pattern normal
*/

namespace elf
{
  typedef std::chrono::duration<int, std::ratio<1 * 60 * 60 * 24, 1>> day_t;
  typedef std::chrono::duration<int, std::ratio<1 * 60 * 60, 1>> hour_t;
  typedef std::chrono::duration<int, std::ratio<1 * 60, 1>> minute_t;
  typedef std::chrono::duration<long long, std::ratio<1, 1>> second_t;
  typedef std::chrono::duration<long long , std::ratio<1, 1000>> milli_t;
  typedef std::chrono::duration<long long , std::ratio<1, 1000000>> mirco_t;
  typedef std::chrono::duration<long long , std::ratio<1, 1000000000>> nano_t;

  extern const auto DT_PAT_N_A = "%Y-%m-%d %H:%M:%S";
  extern const auto DT_PAT_N_W = L"%Y-%m-%d %H:%M:%S";


#define TIME_T
#define TM
#define TS
#define COMMON
#define DT

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
    
/*****************************************************************************************/
    DT
  public:
    static struct tm parse_t(IN const std::string & dt, IN const char * const  pat = nullptr)
    {
      std::tm t = { 0 };
#if defined(_WIN32) && defined(_MSC_VER)
      msvc_elf::strptime_msvc(dt.c_str(), nullptr == pat ? DT_PAT_N_A : pat, &t);
#else
      strptime(date_time.c_str(), nullptr == pat ? DT_PAT_N_A : pat, &t);
#endif
      return t;
    }

    static std::time_t parse_tt(IN const std::string & dt, IN const char * const  pat = nullptr)
    {
      auto t = parse_t(dt, pat);
      auto tt = toTt(t);
      return tt;
    }

    static std::string minA(IN const char * const pat = nullptr)
    {
      return toStringA(min_t(nullptr), pat);
    }

    static std::string minA(IN const struct tm & t, IN const char * const  pat = nullptr)
    {
      auto t0 = min_t(&t);
      return toStringA(t0, pat);
    }

    static std::wstring minW(IN const wchar_t * const pat = nullptr)
    {
      return toStringW(min_t(nullptr), pat);
    }

    static std::wstring minW(IN const struct tm & t, IN const wchar_t * const  pat = nullptr)
    {
      auto t0 = min_t(&t);
      return toStringW(t0, pat);
    }

    static std::string maxA(IN const char * const pat = nullptr)
    {
      return toStringA(max_t(nullptr), pat);
    }

    static std::string maxA(IN const struct tm & t, IN const char * const  pat = nullptr)
    {
      auto t0 = max_t(&t);
      return toStringA(t0, pat);
    }

    static std::wstring maxW(IN const wchar_t * const pat = nullptr)
    {
      return toStringW(max_t(nullptr), pat);
    }

    static std::wstring maxW(IN const struct tm & t, IN const wchar_t * const  pat = nullptr)
    {
      auto t0 = max_t(&t);
      return toStringW(t0, pat);
    }

/*****************************************************************************************/
    COMMON
  public:


    static std::vector<struct tm> pastTidyHours(IN int past, IN const struct tm * const pt = nullptr)
    {
      std::vector<struct tm> v;
      if (0 == past){
        v.push_back(tidy_t());
        return v;
      }
      struct tm t = tidy_t(pt);
      int differ = past > 0 ? past : -past;
      bool before = past > 0;
      for (int index = 0; index < differ; ++index)
      {
        //
        v.push_back(plusHours_t(t, before ? index - past + 1 : index)); //keep order
      }
      return v;
    }

    static std::vector<struct tm> pastHours(IN int past, IN const struct tm * const pt = nullptr)
    {
      std::vector<struct tm> v;
      if (0 == past){
        v.push_back(now_t());
        return v;
      }
      struct tm t = nullptr == pt ? now_t() : *pt;
      int differ = past > 0 ? past : -past;
      bool before = past > 0;
      for (int index = 0; index < differ; ++index)
      {
        //
        v.push_back(plusHours_t(t, before ? index - past + 1 : index)); //keep order
      }
      return v;
    }

    static std::string tidyA(IN const char * const  pat = nullptr)
    {
      auto t = now_t();
      t.tm_min = 0;
      t.tm_sec = 0;
      auto dt = toStringA(t, pat);
      return dt;
    }

    static std::wstring tidyW(IN const wchar_t * const  pat = nullptr)
    {
      auto t = now_t();
      t.tm_min = 0;
      t.tm_sec = 0;
      auto dt = toStringW(t, pat);
      return dt;
    }

    static std::string timeA(IN const char * const pat = nullptr)
    {
      auto t = now_t();
      return toStringA(t, pat);
    }
    static std::wstring timeW(IN const wchar_t * const pat = nullptr)
    {
      auto t = now_t();
      return toStringW(t, pat);
    }

    static std::string toStringA(IN const struct tm & t, IN const  char * const pat = nullptr)
    {
      std::string dt(24, '\0');
      auto size = strftime(const_cast<char *>(dt.data()),
        24,
        nullptr == pat ? DT_PAT_N_A : pat,
        &t);
      if (0 == size)
      {
        dt = std::string(48, '\0');
        strftime(const_cast<char *>(dt.data()),
          48,
          nullptr == pat ? DT_PAT_N_A : pat,
          &t);
      }
      return dt;
    }
    static std::string toStringA(IN std::time_t tt, IN const char * const pat = nullptr)
    {
      auto t = toT(tt);
      return toStringA(t, pat);
    }

    static std::wstring toStringW(IN const struct tm & t, IN const wchar_t * const  pat = nullptr)
    {
      //TODO
      std::wstring dt(24, '\0');
      auto size = wcsftime(const_cast<wchar_t *>(dt.data()),
        24,
        nullptr == pat ? DT_PAT_N_W : pat,
        &t);
      if (0 == size)
      {
        dt = std::wstring(48, '\0');
        wcsftime(const_cast<wchar_t *>(dt.data()),
          48,
          nullptr == pat ? DT_PAT_N_W : pat,
          &t);
      }
      return dt;
    }
    static std::wstring toStringW(IN std::time_t tt, IN const wchar_t * const pat = nullptr)
    {
      //TODO
      auto t = toT(tt);
      return toStringW(t, pat);

    }
/*****************************************************************************************/
    TM
  public:
    static struct tm min_t(const struct tm * const pt = nullptr)
    {
      struct tm t = nullptr == pt ? now_t() : *pt;
      t.tm_sec = 0;
      t.tm_min = 0;
      t.tm_hour = 0;
      return t;
    }
    static struct tm max_t(const struct tm * const pt = nullptr)
    {
      struct tm t = nullptr == pt ? now_t() : *pt;
      t.tm_sec = 59;
      t.tm_min = 59;
      t.tm_hour = 23;
      return t;
    }
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
    inline static std::time_t toTt(IN const struct tm & t)
    {
      return mktime(&(const_cast<struct tm &>(t)));
    }

    /** keep memcpy away*/
    inline static struct tm tidy_t(const struct tm * const pt = nullptr) {
      struct tm t = nullptr == pt ? now_t() : *pt;
      t.tm_min = 0;
      t.tm_sec = 0;
      return t;
    }

    static struct tm plusDays_t(IN const struct tm & t, IN int days)
    {
      if (0 == days)
        return t;
      auto tt = toTt(t);
      tt += std::chrono::duration_cast<second_t>(day_t(days)).count();
      return toT(tt);
    }

    static struct tm plusHours_t(IN const struct tm & t, IN int hours) {
      if (0 == hours)
        return t;
      auto tt = toTt(t);
      tt += std::chrono::duration_cast<second_t>(hour_t(hours)).count();// std::chrono::seconds(hoursToAdd * 60 * 60 * 1).count();
      return toT(tt);
    }

    static struct tm plusMinutes_t(IN const struct tm & t, IN int minutes)
    {
      if (0 == minutes)
        return t;
      auto tt = toTt(t);
      tt += std::chrono::duration_cast<second_t>(minute_t(minutes)).count();
      return toT(tt);
    }

    static struct tm plusSeconds_t(IN const struct tm & t, IN int seconds)
    {
      if (0 == seconds)
        return t;
      auto tt = toTt(t);
      tt += seconds;
      return toT(tt);
    }
/*****************************************************************************************/
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