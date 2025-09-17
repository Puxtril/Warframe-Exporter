#pragma once

#include <filesystem>
#include <cstdint>

#define WINDOWS_TIME_TICK 10000000
#define WINDOWS_TIME_MAGIC 116444736000000000LL

#if defined WIN32 || defined MINGW
	#define WINDOWS
	#include <Windows.h>
#else
	#define UNIX
	#include <utime.h>
	#include <sys/stat.h>
	#include <sys/time.h>
#endif

namespace WarframeExporter
{
	// Windows tracks time in 100ns units since January 1, 1601.
	// Unix tracks time in 1s units since January 1, 1970.
	class FileProperties
	{
	public:
		typedef int64_t TimeEpoch;
		typedef int64_t TimeDos;

		static TimeEpoch dosToEpoch(TimeDos dosTime);
		static TimeDos epochToDos(TimeEpoch epochTime);
		static TimeDos wipeNanoseconds(TimeDos dosTime);

		// Implemention is determined at compile-time
		// "Cross platform interface"
		static void writeEpoch(std::filesystem::path filePath, TimeEpoch createTime, TimeEpoch writeTime, TimeEpoch accessTime);
		static void writeDos(std::filesystem::path filePath, TimeDos createTime, TimeDos writeTime, TimeDos accessTime, bool ignoreNano = false);
		static TimeEpoch readEpoch(std::filesystem::path filePath);
		static TimeDos readDos(std::filesystem::path filePath, bool ignoreNano = false);

	// Platform-specific
	private:
	#ifdef WINDOWS
		static TimeDos filetimeStructToUint64(const FILETIME& dosTime);
		static FILETIME uint64ToFiletimeStruct(const TimeDos& dosTime);
		static void writeFileTime(std::filesystem::path filePath, TimeDos createTime, TimeDos writeTime, TimeDos accessTime);
		static std::tuple<FILETIME, FILETIME, FILETIME> readFileTime(std::filesystem::path filePath);
	#else
		static void writeFileTime(std::filesystem::path filePath, const struct utimbuf& time);
		static TimeEpoch readFileTime(std::filesystem::path filePath);
	#endif
	};
}
