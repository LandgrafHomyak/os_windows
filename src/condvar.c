#include <Windows.h>
#include <COMiC/types.h>
#include <COMiC/os.h>


COMiC_success COMiC_OS_CondVar_Create(COMiC_CondVar *storage, COMiC_Lock *lock)
{
    InitializeConditionVariable(&(storage->cv_storage));
    storage->lock = lock;
    return COMiC_OK;
}

COMiC_success COMiC_OS_CondVar_Wait(COMiC_CondVar *storage)
{
    return SleepConditionVariableCS(&(storage->cv_storage), &(storage->lock->cs_storage), INFINITE) == 0 ? COMiC_FAILED : COMiC_OK;

}

void COMiC_OS_CondVar_Notify(COMiC_CondVar *storage)
{
    WakeConditionVariable(&(storage->cv_storage));
}

void COMiC_OS_CondVar_NotifyAll(COMiC_CondVar *storage)
{
    WakeAllConditionVariable(&(storage->cv_storage));
}

void COMiC_OS_CondVar_Destroy(COMiC_CondVar *storage)
{}