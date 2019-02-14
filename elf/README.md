# c-plus-plus elf time library
	1. C++ 11
	2. header only
	3. thread-safe
	4. Java-LocalDateTime-like 
	5. ascii and unicode supported
	6. cross platform(windows、linux、macOS)
	7. template keep api alike

********

## Modules

**elf::time::header-only::library**

	如无特殊说明,以下规则在本文件中严格适用
	t -> struct tm
	tt-> std::time_t
	dt-> date time(经常以字符串形式出现的~)
	v-> std::vector
	a-> ascii
	w-> unicode
	pat-> pattern
	now-> 现在的时间
	when-> 取何时的意思,即根据一个时间转换成另外一个通用时间

	%Y year
	%m month
	%d day
	%H hour
	%M minute
	%S second

	除第一个例子以外,其他例子中均使用以下别称.
	using time_wstr = elf::time<std::wstring>;
	using time_str = elf::time<std::string>;
	using time_tt = elf::time<std::time_t>;
	using time_tm =  elf::time<struct tm>;

***now***

	static T elf::time<T>::now(const char * const pat = nullptr);	//template static function declaration
	取现在的时间,支持struct tm、 std::time_t、std::string和std::wstring返回.当以string返回时支持指定时间格式化方式。
<code>
	
	#include "time.hpp"
	auto t = elf::time<struct tm>::now();	// t is struct tm
	auto tt = elf::time<std::time_t>::now();		//tt is std::time_t
	auto dt = elf::time<std::string>::now();		// dt is std::string, 
	auto wdt = elf::time<std::wstring>::now();	//wdt is std::wstring

	auto t0 = time_tm::now();	// t0 is struct tm
	auto tt0 = time_tt::now();	// tt0 is std::time_t
	auto dt0 = time_str::now();	// dt0 is std::string
	auto wdt0 = time_wstr::now();	// wdt0 is std::wstring
</code>

***when***
	
	static T elf::time<T>::when(const char * const pat = nullptr);	//template static function declaration
	static T elf::time<T>::when(const struct tm & t, const char * const pat = nullptr);	//template static function declaration
	static T elf::time<T>::when(const std::time_t tt, const char * const pat = nullptr);	//template static function declaration
	static T elf::time<T>::when(const std::string & dt, const char * const pat = nullptr);	//template static function declaration
	static T elf::time<T>::when(const std::wstring & dt, const wchar_t * const pat = nullptr);	//template static function declaration

	std::string、std::wstring、struct tm、 std::time_t 互相转换

<code>

	#include "time.hpp"
	auto t = time_tm::when("2018-08-08 11:12:13");	// std::string to struct tm
	auto t0 = time_tm::when(L"20180909120000", L"%Y%m%d%H%M%S");	// std::wstring to struct tm;
	auto tt = time_tt::when(t0);		// struct tm to std::time_t
	auto dt = time_str::when(tt);	// std::time_t to std::string, value is 2018-09-09 12:00:00, formatted by default pattern
	auto wdt = time_wstr::when(tt);	// std::time_t to std::wstring
	auto tt0 = time_tt::when(wdt);	//std::wstring to std::time_t
	etc. ……

</code>

***plus***
	
	static T plus_days/mins/secs(int days/mins/secs, const char * const pat = nullptr);//template static function declaration
	static T plus_days/mins/secs(int days/mins/secs, const struct tm & t, const char * const pat = nullptr);
	static T plus_days/mins/secs(int days/mins/secs, const std::time_t tt, const char * const pat = nullptr);
	static T plus_days/mins/secs(int days/mins/secs, const std::string & dt, const char * const pat = nullptr);
	static T plus_days/mins/secs(int days/mins/secs, const std::wstring & dt, const wchar_t * const pat = nullptr);
	保证同一类型的api在调用时的api一致性

<code>

	#include "time.hpp"
	auto t = time_tm::now();
	auto tt = time_tt::now();
	auto dt = time_str::now();
	auto wdt = time_wstr::now();

	auto t0 = time_tm::plus_days(10, tt);	// std::time_t plus 10 days and then convert to struct tm
	t0 = time_tm::plus_hours(5, dt);		//std::string plus 5 hours and then convet to struct tm
	t0 = time_tm::plus_mins(-30, wdt);	// std::wstring plus -30 minutes and then convet to struct tm
	t0 = time_tm::plus_secs(20, t);	// struct tm plus 20 seconds and then return struct tm

	auto tt0 = time_tt::plus_days(3, t);		// struct tm plus 3 days and then convert to std::time_t
	tt0 = time_tt::plus_hours(0, tt);		// std::time_t plus 0 hours and return std::time_t
	tt0 = time_tt::plus_mins(11, dt);	//	std::string plus 11 minutes and then convert to std::time_t
	tt0 = time_tt::plus_secs(-100, wdt);		//std::wstring plus -100 seconds and then convert to std::time_t

	auto dt0 = time_str::plus_days(29, t);
	dt0 = time_str::plus_hours(10, tt);
	dt0 = time_str::plus_mins(-50, dt);
	dt0 = time_str::plus_secs(10, wdt);

	auto wdt0 = time_wstr::plus_days(110, t);
	wdt0 = time_wstr::plus_hours(110, tt);
	wdt0 = time_wstr::plus_mins(110, dt);
	wdt0 = time_wstr::plus_secs(110, wdt);

</code>

***tidy***

	static T tidy(const char * const pat = nullptr)
	static T tidy(const struct tm & t, const char * const pat = nullptr);
	static T tidy(const std::time_t tt, const char * const pat = nullptr);
	static T tidy(const std::string & dt, const char * const pat = nullptr);
	static T tidy(const std::wstring & dt, const wchar_t * const pat = nullptr);
	取整点的小时,例如2019-02-03 12:11:34 -> 2019-02-03 12:00:00


<code>

	#include "time.hpp"
	auto t = time_tm::tidy();	//tidy struct tm from  now
	auto tt = time_tt::tidy(t);	//tidy std::time_t from 	struct tm
	auto dt = time_str::tidy(tt);	//tidy std::string from std::time_t
	auto wdt = time_wstr::tidy(dt);	// tidy std::wstring from std::string

</code>


***gone***

	static std::vector<T> gone_days/mins/secs(int days/mins/secs, const char * const pat = nullptr);
	static std::vector<T> gone_days/mins/secs(int days/mins/secs, const struct tm & t, const char * const pat = nullptr);
	static std::vector<T> gone_days/mins/secs(int days/mins/secs, const  std::time_t tt, const char * const pat = nullptr);
	static std::vector<T> gone_days/mins/secs(int days/mins/secs, const std::string & dt, const char * const pat = nullptr);
	static std::vector<T> gone_days/mins/secs(int days/mins/secs, const std::wstring &dt, const wchar_t * const pat = nullptr);
	枚举包含当天在内的过去的几天或者是未来的几天.

<code>

	auto v_t = time_tm::gone_days(10);	//	v_t is a vector<struct tm> contains 10 struct tm
	std::for_each(v_t.cbegin(), v_t.cend(), [](const struct tm & t)->void{
		std::cout << time_str::when(t).c_str() << std::endl;		// struct tm to std::string and cout
	});	

	auto v_tt = time_tt::gone_hours(5);	// v_tt is a vector<std::time_t> contains 5 std::time_t
	std::for_each(v_tt.cbegin(), v_tt.cend(), [](const std::time_t tt)->void{
		std::cout << tt << std::endl;
	});

	auto v_dt = time_str::gone_mins(20); // v_dt is a vector<std::string> contains 20 std::string
	std::for_each(v_dt.cbegin(), v_dt.cend(), [](const std::string & dt)->void{
		std::cout << dt << std::endl;
	});

	auto v_wdt = time_wstr::gone_secs(11);	// v_wdt is a vector<std::wstring> contaions 11 std::wstring
	std::for_each(v_wdt.cbegin(), v_wdt.cend(), [](const std::wstring & wdt)->void{
		std::wcout << wdt << std::endl;
	});

</code>

