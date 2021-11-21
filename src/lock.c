#include <Windows.h>

#include <COMiC/os.h>

COMiC_success COMiC_OS_Lock_Create(COMiC_Lock *storage)
{
    InitializeCriticalSection(&(storage->cs_storage));
    return COMiC_OK;
}

COMiC_success COMiC_OS_Lock_Lock(COMiC_Lock *storage)
{
    EnterCriticalSection(&(storage->cs_storage));
    return COMiC_OK;
}

void COMiC_OS_Lock_Unlock(COMiC_Lock *storage)
{
    LeaveCriticalSection(&(storage->cs_storage));
}

/*
COMiC_bool COMiC_OS_Lock_IsLocked(COMiC_Lock *storage)
{
    switch (WaitForSingleObject(storage->mutex_handler, 0))
    {
        case WAIT_ABANDONED:
            return 0;
        case WAIT_OBJECT_0:
            return 1;
        case WAIT_TIMEOUT:
            return 0;
        case WAIT_FAILED:
            COMiC_Error_Fatal("Can't check mutex");
            return 0;
        default:
            COMiC_Error_Fatal("Unexpected return of mutex state checker");
            return 0;
    }
}
*/
void COMiC_OS_Lock_Destroy(COMiC_Lock *storage)
{
    DeleteCriticalSection(&(storage->cs_storage));
}
