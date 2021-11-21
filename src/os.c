#include <Windows.h>

#include <COMiC/os.h>

static COMiC_OS *last_instance = NULL;

static COMiC_Error os_error = {.message = NULL, .deallocator = NULL};

const char *COMiC_OS_Error_Get(void)
{
    return _COMiC_Error_Get(&os_error);
}

void COMiC_OS_Error_Clear(void)
{
    _COMiC_Error_Clear(&os_error);
}

int COMiC_OS_Init(COMiC_OS *storage, COMiC_size heap_size)
{
    _COMiC_Error_Clear(&os_error);

    storage->thread_list = NULL;


    if (COMiC_OS_Lock_Create(&(storage->global_lock)))
    {
        _COMiC_Error_Set(&os_error, "Can't create global mutex");
        return -1;
    }


    if (COMiC_OS_Thread_RegisterCurrent(storage, &(storage->root_thread)))
    {
        _COMiC_Error_Set(&os_error, "Can't register thread");
        COMiC_OS_Lock_Destroy(&(storage->global_lock));
        return -1;
    }

    storage->heap_size = heap_size;
    storage->heap = HeapCreate(0, 0, heap_size);
    if (storage->heap == NULL)
    {
        _COMiC_Error_Set(&os_error, "Can't create custom heap");
        COMiC_OS_Thread_Unregister(&storage->root_thread);
        COMiC_OS_Lock_Destroy(&(storage->global_lock));
        return -1;
    }

    storage->_prev = last_instance;
    last_instance = storage;
    storage->_next = NULL;

    return 0;
}

void COMiC_OS_Finalize(COMiC_OS *storage)
{
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
        /* todo set error */
        COMiC_Error_Fatal("Can't destroy custom heap (maybe it was corrupted)");
        return;
    }

    storage->heap = NULL;
    storage->heap_size = 0;

    COMiC_OS_Thread_Unregister(&storage->root_thread);
    COMiC_OS_Lock_Destroy(&(storage->global_lock));
}


COMiC_OS_Thread *COMiC_OS_Thread_GetCurrent(void)
{
    COMiC_OS *os = last_instance;
    COMiC_OS_Thread *thr;
    HANDLE cthr;

    for (; os->_prev != NULL; os = os->_prev)
    {}

    cthr = GetCurrentThread();

    for (; os != NULL; os = os->_next)
    {
        for (thr = os->thread_list; thr != NULL; thr = thr->_next)
        {
            if (thr == cthr)
            {
                return cthr;
            }
        }
    }

    return NULL;
}

COMiC_OS *COMiC_OS_GetCurrent(void)
{
    COMiC_OS_Thread *cthr = COMiC_OS_Thread_GetCurrent();
    if (cthr == NULL)
    {
        return NULL;
    }
    return cthr->os_instance;
}