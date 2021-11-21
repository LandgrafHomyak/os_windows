#include <Windows.h>

#include <COMiC/os.h>

struct thread_launcher_info
{
    void (*func)(void *data);

    void *data;
    COMiC_OS_Thread *thread;
};

static DWORD thread_proc(struct thread_launcher_info *data)
{
    COMiC_OS_Thread *thread;
    DWORD ret;
    data->func(data->data);
    thread = data->thread;
    COMiC_Free(data);
    return _COMiC_Error_Get(&(thread->error)) != NULL ? 1 : 0;
}

static void COMiC_OS_Thread_Register(_COMiC_OS_Instance *os_instance, COMiC_OS_Thread *storage, HANDLE thread)
{
    storage->thread_handler = thread;
    storage->os_instance = os_instance;
    storage->error = (COMiC_Error) {.message = NULL, .deallocator=NULL};
    storage->_next = os_instance->thread_list;
    storage->_prev = NULL;
    os_instance->thread_list = storage;
}


int COMiC_OS_Thread_Create(COMiC_OS_Thread *storage, void (*func)(void *data), void *data)
{
    _COMiC_OS_Instance *os_instance;
    struct thread_launcher_info *tli;

    _COMiC_OS_GlobalLock_Acquire();

    os_instance = _COMiC_OS_Instance_GetCurrent();
    if (os_instance == NULL)
    {
        return -1;
    }

    tli = COMiC_Alloc(sizeof(*tli));
    tli->func = func;
    tli->data = data;
    tli->thread = storage;


    storage->thread_handler = CreateThread(
            NULL,
            0,
            (LPTHREAD_START_ROUTINE) thread_proc,
            data,
            CREATE_SUSPENDED,
            NULL
    );

    if (storage->thread_handler == NULL)
    {
        COMiC_Free(tli);
        COMiC_Error_Set("Can't create thread");
        return -1;
    }


    COMiC_OS_Thread_Register(os_instance, storage, storage->thread_handler);
    _COMiC_OS_GlobalLock_Release();
    return 0;
}

int COMiC_OS_Thread_Pause(COMiC_OS_Thread *storage)
{
    if (SuspendThread(storage->thread_handler) == (DWORD) -1)
    {
        COMiC_Error_Set("Can't pause thread");
        return -1;
    }

    return 0;
}

int COMiC_OS_Thread_Resume(COMiC_OS_Thread *storage)
{
    if (ResumeThread(storage->thread_handler) == (DWORD) -1)
    {
        COMiC_Error_Set("Can't resume thread");
        return -1;
    }
    return 0;
}

int COMiC_OS_Thread_Wait(COMiC_OS_Thread *storage, COMiC_uint32 milliseconds)
{
    if (WaitForSingleObject(storage->thread_handler, milliseconds == 0 ? INFINITE : milliseconds) == WAIT_FAILED)
    {
        COMiC_Error_Set("Can't wait for thread");
        return -1;
    }
    return 0;
}

int COMiC_OS_Thread_Kill(COMiC_OS_Thread *storage)
{
    if (TerminateThread(storage->thread_handler, 0))
    {
        COMiC_Error_Set("Can't kill thread");
        return -1;
    }
    return 0;
}

void COMiC_OS_Thread_Destroy(COMiC_OS_Thread *storage)
{
    COMiC_OS_Thread_Unregister(storage);

    if (CloseHandle(storage->thread_handler) == 0)
    {
        COMiC_Error_Fatal("Can't destroy thread");
        return;
    }
}

int COMiC_OS_Thread_RegisterCurrent(_COMiC_OS_Instance *os_instance, COMiC_OS_Thread *storage)
{
    _COMiC_OS_Instance *os = os_instance;
    COMiC_OS_Thread *thr;

    _COMiC_OS_GlobalLock_Acquire();

    for (; os->_prev != NULL; os = os->_prev)
    {}

    storage->thread_handler = GetCurrentThread();

    for (; os != NULL; os = os->_next)
    {
        for (thr = os->thread_list; thr != NULL; thr = thr->_next)
        {
            if (thr == storage->thread_handler)
            {
                COMiC_Error_Set("Re-registering thread not allowed");
                _COMiC_OS_GlobalLock_Release();
                return -1;
            }
        }
    }

    COMiC_OS_Thread_Register(os_instance, storage, storage->thread_handler);
    _COMiC_OS_GlobalLock_Release();
    return 0;
}

void COMiC_OS_Thread_Unregister(COMiC_OS_Thread *storage)
{
    _COMiC_OS_GlobalLock_Acquire();

    if (storage->os_instance->thread_list == storage)
    {
        storage->os_instance->thread_list = storage->_next;
    }
    if (storage->_prev != NULL)
    {
        storage->_prev->_next = storage->_next;
    }
    if (storage->_next != NULL)
    {
        storage->_next->_prev = storage->_prev;
    }

    _COMiC_Error_Clear(&storage->error);


    _COMiC_OS_GlobalLock_Acquire();
}
