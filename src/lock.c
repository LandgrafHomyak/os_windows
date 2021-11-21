#include <Windows.h>

#include <COMiC/os.h>

int COMiC_OS_Lock_Create(COMiC_Lock *storage)
{
    storage->mutex_handler = CreateMutex(NULL, FALSE, NULL);
    if (storage->mutex_handler == NULL)
    {
        COMiC_Error_Set("Can't create mutex");
        return -1;
    }
    return 0;
}

int COMiC_OS_Lock_Lock(COMiC_Lock *storage, COMiC_uint32 milliseconds)
{
    if (WaitForSingleObject(storage->mutex_handler, milliseconds == 0 ? INFINITE : milliseconds) == WAIT_FAILED)
    {
        COMiC_Error_Set("Can't acquire mutex");
        return -1;
    }
    return 0;
}

void COMiC_OS_Lock_Unlock(COMiC_Lock *storage)
{
    if (ReleaseMutex(storage->mutex_handler))
    {
        COMiC_Error_Fatal("Can't release mutex");
        return;
    }
}

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

void COMiC_OS_Lock_Destroy(COMiC_Lock *storage)
{
    if (CloseHandle(storage->mutex_handler) == 0)
    {
        COMiC_Error_Fatal("Can't destroy mutex");
        return;
    }
}
