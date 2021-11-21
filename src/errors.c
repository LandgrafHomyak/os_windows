#include <assert.h>
#include <stdio.h>

#include <COMiC/os.h>

void COMiC_Error_SetHeap(void (*deallocator)(char *), const char *msg)
{
    COMiC_OS_Thread *thread = COMiC_OS_Thread_GetCurrent();

    if (thread == NULL)
    {
        return;
    }

    _COMiC_Error_SetHeap(&(thread->error), deallocator, msg);
}


const char *COMiC_Error_Get(void)
{
    COMiC_OS_Thread *thread = COMiC_OS_Thread_GetCurrent();

    if (thread == NULL)
    {
        return NULL;
    }

    return _COMiC_Error_Get(&(thread->error));
}

void COMiC_Error_Clear(void)
{
    COMiC_OS_Thread *thread = COMiC_OS_Thread_GetCurrent();

    if (thread == NULL)
    {
        return;
    }

    _COMiC_Error_Clear(&(thread->error));
}

void COMiC_Error_Fatal(const char *msg)
{
    fprintf(stderr, "Fatal error: %s", msg);
    FatalAppExitA(0, msg);
#ifdef _MSC_VER

#include <crtdbg.h>

    _ASSERT_EXPR(1, msg);
#else
    assert("fatal error");
#endif
    ExitProcess(-239);
    exit(-239);
}