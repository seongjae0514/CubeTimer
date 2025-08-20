#include "Debug.h"

#include <Windows.h>

#include "Timer.h"

/* Global variables *******************************************************/

static MillisecTime  StartTimeMs     = 0;
static MillisecTime  ElapsedTimeMs   = 0;
static TimerStatus   CurrentStatus   = TM_STOP;
static LARGE_INTEGER PerformanceFreq = { 0 };

/* Private functions ******************************************************/

BOOL TmpInitializeTickCounter(VOID)
{
    return QueryPerformanceFrequency(&PerformanceFreq);
}

ULONGLONG TmpGetTickCount(VOID)
{
    LARGE_INTEGER tickUs;
    QueryPerformanceCounter(&tickUs);

    return (tickUs.QuadPart * 1000) / PerformanceFreq.QuadPart;
}

/* Global functions *******************************************************/

BOOL TmInitialize(VOID)
{
    StartTimeMs = 0;
    ElapsedTimeMs = 0;
    CurrentStatus = TM_STOP;
    return TmpInitializeTickCounter();
}

BOOL TmUninitialize(VOID)
{
    return TRUE;
}

TimerStatus TmTrigger(VOID)
{
    switch (CurrentStatus)
    {
        case TM_STOP:
        {
            StartTimeMs = TmpGetTickCount();
            return CurrentStatus = TM_WAITING;
        }
        case TM_WAITING:
        {
            if (TmpGetTickCount() - StartTimeMs >= TIMER_MIN_WAIT_TIME)
            {
                StartTimeMs = TmpGetTickCount();
                return CurrentStatus = TM_RECORDING;
            }
            else
            {
                return CurrentStatus = TM_STOP;
            }
        }
        case TM_RECORDING:
        {
            ElapsedTimeMs = TmpGetTickCount() - StartTimeMs;
            return CurrentStatus = TM_STOP;
        }
    }
    return TM_STOP;
}

MillisecTime TmGetElapsedTime(VOID)
{
    switch (CurrentStatus)
    {
        case TM_RECORDING:
        {
            return ElapsedTimeMs = TmpGetTickCount() - StartTimeMs;
        }
        default:
        {
            return ElapsedTimeMs;
        }
    }
}

TimerStatus TmGetTimerStatus(VOID)
{
    return CurrentStatus;
}

BOOL TmCanStart(VOID)
{
    if (CurrentStatus != TM_WAITING)
    {
        return FALSE;
    }

    return TmpGetTickCount() - StartTimeMs >= TIMER_MIN_WAIT_TIME;
}
