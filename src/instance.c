#include <COMiC/os.h>

_COMiC_OS_Instance *last_instance = NULL;

int _COMiC_OS_Instance_Init(_COMiC_OS_Instance *storage, COMiC_size heap_max_size)
{
    _COMiC_OS_GlobalLock_Acquire();

    storage->heap = HeapCreate(0, 0, heap_max_size);
    if (storage->heap == NULL)
    {
        return -1;
    }

    storage->_prev = last_instance;
    storage->_next = NULL;
    last_instance = storage;
    storage->thread_list = NULL;

    _COMiC_OS_GlobalLock_Release();

    return 0;
}

void _COMiC_OS_Instance_Finalize(_COMiC_OS_Instance *storage)
{
    _COMiC_OS_GlobalLock_Acquire();

    if (storage == last_instance)
    {
        last_instance = storage->_prev;
    }

    if (storage->_prev != NULL)
    {
        storage->_prev->_next = storage->_next;
    }

    if (storage->_next != NULL)
    {
        storage->_next->_prev = storage->_prev;
    }

    if (HeapDestroy(storage->heap) == 0)
    {
        COMiC_Error_Fatal("can't destroy custom heap");
    }

    _COMiC_OS_GlobalLock_Release();
}

_COMiC_OS_Instance *_COMiC_OS_Instance_GetCurrent(void)
{
    COMiC_OS_Thread *thr = COMiC_OS_Thread_GetCurrent();
    if (thr == NULL)
    {
        return NULL;
    }
    return thr->os_instance;
}


COMiC_OS_Thread *COMiC_OS_Thread_GetCurrent(void)
{
    _COMiC_OS_Instance *os;
    COMiC_OS_Thread *thr;
    HANDLE cthr;

    cthr = GetCurrentThread();

    _COMiC_OS_GlobalLock_Acquire();

    for (os = last_instance; os != NULL; os = os->_next)
    {
        for (thr = os->thread_list; thr != NULL; thr = thr->_next)
        {
            if (thr == cthr)
            {
                _COMiC_OS_GlobalLock_Release();
                return thr;
            }
        }
    }

    _COMiC_OS_GlobalLock_Release();
    return NULL;
}