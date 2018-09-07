#ifndef _T_TIME_HPP_
#define _T_TIME_HPP_
#include <chrono>
#include <type_traits>
#include<string>


//#define TIME_T elf::T_Time<std::time_t>
//#define TM elf::T_Time<struct tm>
//#define DT elf::T_Time<std::string>
//#define UTIL elf::Util

namespace elf {
	template <class T> class TimeHelper;
	class Util;

	template <class T>
	class T_Time {
		friend class elf::TimeHelper<T>;
	public:
		static T now()
		{
			return TimeHelper<T>::now();
		}
		
	};




	template<class T>
	class TimeHelper{
	private:
		explicit TimeHelper() = default;
		~TimeHelper() = default;
	public:
		static T now()
		{
			return T();
		}
	};

	template<>
	class TimeHelper<std::time_t> {

	private:
		explicit TimeHelper() = default;
		~TimeHelper() = default;
	public:
		static std::time_t now()
		{
			return std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		}
	};

	template<>
	class TimeHelper<std::string> {
	private:
		explicit TimeHelper() = default;
		~TimeHelper() = default;
	public:
		static std::string now()
		{
			auto t = Util::toT(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));;// TimeHelper<struct tm>::now()
			std::string dt(24, '\0');
			auto size = strftime(const_cast<char *>(dt.data()),
				24,
				DT_PAT_N_A,
				&t);
			if (0 == size)
			{
				dt = std::string(48, '\0');
				strftime(const_cast<char *>(dt.data()),
					48,
					 DT_PAT_N_A,
					&t);
			}
			return dt;
		}
	};

	template<>
	class TimeHelper<struct tm> {
	private:
		explicit TimeHelper() = default;
		~TimeHelper() = default;
	public:
		static struct tm now()
		{
			auto tt = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
			auto t = Util::toT(tt);
			return t;
		}
	};

	class Util {
	private:
		explicit Util() = default;
		~Util() = default;
	public:
		static struct tm toT(std::time_t tt)
		{
			struct tm t = { 0 };
#ifdef _WIN32
			localtime_s(&t, &tt);
#else
			localtime_r(&tt, &t);
#endif
			return t;
		}
	};
}


#endif
