#include <COMiC/types.h>
#include <COMiC/os.h>

#ifndef COMiC__OS_H
#define COMiC__OS_H

#include <Windows.h>

#if COMiC_LIMITED
# pragma pack(push, 1)
#endif
typedef struct COMiC_Lock
{
    HANDLE mutex_handler;
} COMiC_Lock;

typedef struct COMiC_OS COMiC_OS;

typedef struct COMiC_OS_Thread
{
    struct COMiC_OS_Thread *_prev;
    struct COMiC_OS_Thread *_next;
    HANDLE thread_handler;
    COMiC_OS *os_instance;
    COMiC_Error error;
} COMiC_OS_Thread;

struct COMiC_OS
{
    struct COMiC_OS *_prev;
    struct COMiC_OS *_next;
    HANDLE heap;
    COMiC_size heap_size;
    COMiC_OS_Thread root_thread;
    COMiC_OS_Thread *thread_list;
    COMiC_Lock global_lock;
};


#if COMiC_LIMITED
# pragma pop()
#endif

#endif /* COMiC__OS_H */