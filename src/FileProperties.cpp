#include "FileProperties.h"

using namespace WarframeExporter;

FileProperties::TimeEpoch
FileProperties::dosToEpoch(TimeDos dosTime)
{
	return (dosTime - WINDOWS_TIME_MAGIC) / WINDOWS_TIME_TICK;
}

FileProperties::TimeDos
FileProperties::epochToDos(TimeEpoch epochTime)
{
    return epochTime * WINDOWS_TIME_TICK + WINDOWS_TIME_MAGIC;
}

FileProperties::TimeDos
FileProperties::wipeNanoseconds(TimeDos dosTime)
{
    return epochToDos(dosToEpoch(dosTime));
}

#ifdef WINDOWS

/////////////////////////////////////////////////////
// Windows overrides of cross-platform interface

FileProperties::TimeDos
FileProperties::filetimeStructToUint64(const FILETIME& dosTime)
{
    return (TimeDos(dosTime.dwHighDateTime) << 32) | TimeDos(dosTime.dwLowDateTime);
}

FILETIME
FileProperties::uint64ToFiletimeStruct(const TimeDos& dosTime)
{
    FILETIME time;
    time.dwHighDateTime = dosTime >> 32;
    time.dwLowDateTime = static_cast<DWORD>(dosTime);
    return time;
}

void
FileProperties::writeEpoch(std::filesystem::path filePath, TimeEpoch createTime, TimeEpoch writeTime, TimeEpoch accessTime)
{
    writeFileTime(filePath, epochToDos(createTime), epochToDos(writeTime), epochToDos(accessTime));
}

void
FileProperties::writeDos(std::filesystem::path filePath, TimeDos createTime, TimeDos writeTime, TimeDos accessTime, bool ignoreNano)
{
    writeFileTime(filePath, createTime, writeTime, accessTime);
}

FileProperties::TimeEpoch
FileProperties::readEpoch(std::filesystem::path filePath)
{
    std::tuple<FILETIME, FILETIME, FILETIME> times = readFileTime(filePath);
    TimeDos rawTime = filetimeStructToUint64(std::get<0>(times));
    return dosToEpoch(rawTime);
}

FileProperties::TimeDos
FileProperties::readDos(std::filesystem::path filePath, bool ignoreNano)
{
    std::tuple<FILETIME, FILETIME, FILETIME> times = readFileTime(filePath);
    TimeDos rawTime = filetimeStructToUint64(std::get<0>(times));
    if (ignoreNano)
        return wipeNanoseconds(rawTime);
    return rawTime;
}

/////////////////////////////////////////////////////
// Windows platform-specific

std::tuple<FILETIME, FILETIME, FILETIME>
FileProperties::readFileTime(std::filesystem::path filePath)
{
    std::string dosPath = filePath.string();
    std::wstring dosPathWide(dosPath.begin(), dosPath.end());
    LPCWSTR dosPathPtr(dosPathWide.c_str());

    HANDLE hFile = CreateFileW(dosPathPtr, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    std::tuple<FILETIME, FILETIME, FILETIME> outTime;
    GetFileTime(hFile, &std::get<0>(outTime), &std::get<1>(outTime), &std::get<2>(outTime));
    CloseHandle(hFile);
    return outTime;
}

void
FileProperties::writeFileTime(std::filesystem::path filePath, TimeDos createTime, TimeDos writeTime, TimeDos accessTime)
{
    std::string dosPath = filePath.string();
    std::wstring dosPathWide(dosPath.begin(), dosPath.end());
    LPCWSTR dosPathPtr(dosPathWide.c_str());

    HANDLE hFile = CreateFileW(dosPathPtr, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    
    FILETIME FcreateTime = uint64ToFiletimeStruct(createTime);
    FILETIME FwriteTime = uint64ToFiletimeStruct(writeTime);
    FILETIME FaccessTime = uint64ToFiletimeStruct(accessTime);
    SetFileTime(hFile, &FcreateTime, &FaccessTime, &FwriteTime);

    CloseHandle(hFile);
}

#else

/////////////////////////////////////////////////////
// Unix overrides of "cross-platform" interface

void
FileProperties::writeDos(std::filesystem::path filePath, TimeDos createTime, TimeDos writeTime, TimeDos accessTime, bool ignoreNano)
{
	utimbuf linTime;
	linTime.actime = dosToEpoch(accessTime);
	linTime.modtime = dosToEpoch(writeTime);
    
    writeFileTime(filePath,linTime);
}

void
FileProperties::writeEpoch(std::filesystem::path filePath, TimeEpoch createTime, TimeEpoch writeTime, TimeEpoch accessTime)
{
    utimbuf timeBuf;
    timeBuf.actime = accessTime;
    timeBuf.modtime = writeTime;

    writeFileTime(filePath, timeBuf);
}

FileProperties::TimeDos
FileProperties::readDos(std::filesystem::path filePath, bool ignoreNano)
{
    time_t modtime = readFileTime(filePath);

    return epochToDos(modtime);
}

FileProperties::TimeEpoch
FileProperties::readEpoch(std::filesystem::path filePath)
{
    time_t modtime = readFileTime(filePath);

    return modtime;
}

/////////////////////////////////////////////////////
// Unix platform-specific

FileProperties::TimeEpoch
FileProperties::readFileTime(std::filesystem::path filePath)
{
    struct stat result;
    stat(filePath.c_str(), &result);
    return result.st_mtime;
}

void
FileProperties::writeFileTime(std::filesystem::path filePath, const struct utimbuf& time)
{
    utime(filePath.c_str(), &time);
}

#endif


