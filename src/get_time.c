#include <Windows.h>

#include <COMiC/os.h>

COMiC_OS_Time COMiC_OS_GetTime(void)
{
    SYSTEMTIME tm;
    GetSystemTime(&tm);
    return (COMiC_OS_Time) {
            .year = tm.wYear,
            .month = tm.wMonth,
            .day = tm.wDay,
            .hour = tm.wHour,
            .minute = tm.wMinute,
            .second = tm.wSecond,
            .milliseconds = tm.wMilliseconds
    };
}