#include "FileProperties.h"

int64_t
FileProperties::wintimeToEpoch(uint64_t winTime)
{
	return winTime / WINDOWS_TICK - SEC_TO_UNIX_EPOCH;
}

#ifdef WINDOWS

void
FileProperties::writeWinTime(std::filesystem::path filePath, uint64_t createTime, uint64_t writeTime, uint64_t accessTime)
{
    FILETIME newCreateTime = *reinterpret_cast<FILETIME*>(&createTime);
    FILETIME newWriteTime = *reinterpret_cast<FILETIME*>(&writeTime);
    FILETIME newAccessTime = *reinterpret_cast<FILETIME*>(&accessTime);
    FileProperties::writeFileTime(filePath, newCreateTime, newWriteTime, newAccessTime);
}

int64_t
FileProperties::readWinTimeMod(std::filesystem::path filePath)
{
    std::tuple<FILETIME, FILETIME, FILETIME> times = readFileTime(filePath);
    return *reinterpret_cast<int64_t*>(&std::get<0>(times));
}

std::tuple<FILETIME, FILETIME, FILETIME>
FileProperties::readFileTime(std::filesystem::path filePath)
{
    std::string winPath = filePath.string();
    std::wstring winPathWide(winPath.begin(), winPath.end());
    LPCWSTR winPathPtr(winPathWide.c_str());

    HANDLE hFile = CreateFile(winPathPtr, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    std::tuple<FILETIME, FILETIME, FILETIME> outTime;
    GetFileTime(hFile, &std::get<0>(outTime), &std::get<1>(outTime), &std::get<2>(outTime));
    CloseHandle(hFile);
    return outTime;
}

void
FileProperties::writeFileTime(std::filesystem::path filePath, FILETIME createTime, FILETIME writeTime, FILETIME accessTime)
{
    std::string winPath = filePath.string();
    std::wstring winPathWide(winPath.begin(), winPath.end());
    LPCWSTR winPathPtr(winPathWide.c_str());

    HANDLE hFile = CreateFile(winPathPtr, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    SetFileTime(hFile, &createTime, &accessTime, &writeTime);
    CloseHandle(hFile);
}

#else

void
FileProperties::writeWinTime(std::filesystem::path filePath, uint64_t createTime, uint64_t writeTime, uint64_t accessTime)
{
	utimbuf linTime;
	linTime.actime = wintimeToEpoch(accessTime);
	linTime.modtime = wintimeToEpoch(writeTime);
    writeFileTime(
        filePath,
        linTime
    );
}

int64_t
FileProperties::readWinTimeMod(std::filesystem::path filePath)
{
    time_t modtime = readFileTime(filePath);
    return (int64_t)modtime;
}

time_t
FileProperties::readFileTime(std::filesystem::path filePath)
{
    struct stat result;
    stat(filePath.c_str(), &result);
    struct timespec& modtime = result.st_mtim;
    return modtime.tv_sec;
}
void
FileProperties::writeFileTime(std::filesystem::path filePath, const struct utimbuf& time)
{
    utime(filePath.c_str(), &time);
}

#endif


