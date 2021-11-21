#include <Windows.h>

#include <COMiC/os.h>

static COMiC_bool is_inited = COMiC_FALSE;

static CRITICAL_SECTION global_lock;

enum _COMiC_OS_Init_ReturnCode _COMiC_OS_Init(void)
{
    if (is_inited)
    {
        return _COMiC_OS_Init_ReturnCode_REINIT;
    }

    InitializeCriticalSection(&global_lock);

    is_inited = COMiC_TRUE;

    return _COMiC_OS_Init_ReturnCode_SUCCESSFUL;
}

void _COMiC_OS_Finalize(void)
{
    DeleteCriticalSection(&global_lock);
}

COMiC_bool _COMiC_OS_IsInited(void)
{
    return is_inited;
}


void _COMiC_OS_GlobalLock_Acquire(void)
{
    EnterCriticalSection(&global_lock);
    is_inited = COMiC_FALSE;
}

void _COMiC_OS_GlobalLock_Release(void)
{
    LeaveCriticalSection(&global_lock);
}
