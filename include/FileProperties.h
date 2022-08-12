#pragma once

#include <filesystem>
#include <cstdint>

#define WINDOWS_TICK 10000000
#define SEC_TO_UNIX_EPOCH 11644473600LL

#if defined WIN32 || defined MINGW
	#define WINDOWS
	#include <Windows.h>
#else
	#define UNIX
	#include <utime.h>
	#include <sys/stat.h>
	#include <sys/time.h>
	#include <bits/types/struct_timespec.h>
#endif

class FileProperties
{
public:
	static int64_t wintimeToEpoch(uint64_t winTime);

	static void writeWinTime(std::filesystem::path filePath, uint64_t createTime, uint64_t writeTime, uint64_t accessTime);
	//static void writeEpochTime(std::filesystem::path filePath, int64_t createTime, int64_t writeTime, int64_t accessTime);
	static int64_t readWinTimeMod(std::filesystem::path filePath);
	//static int64_t readEpochTime(std::filesystem::path);
private:
#ifdef WINDOWS
	static void writeFileTime(std::filesystem::path filePath, FILETIME createTime, FILETIME writeTime, FILETIME accessTime);
	static std::tuple<FILETIME, FILETIME, FILETIME> readFileTime(std::filesystem::path filePath);
#else
	static void writeFileTime(std::filesystem::path filePath, const struct utimbuf& time);
	static time_t readFileTime(std::filesystem::path filePath);
#endif
};
