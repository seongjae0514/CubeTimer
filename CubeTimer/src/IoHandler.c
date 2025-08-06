#include "Debug.h"
#include <Windows.h>
#include "Window.h"
#include "Timer.h"
#include "IoHandler.h"
#include "Record.h"
#include "Scramble.h"

/* Global varialbes *************************************************************/

static BOOL     SpaceKeyPressed = FALSE;
static UINT_PTR RepaintTimerId = 0;

/* Private functions ************************************************************/

static VOID CALLBACK IopRepaintFunc(HWND hWnd, UINT uMsg, UINT_PTR id, DWORD dwTime)
{
    WndRepaintMainWindow();
}

static BOOL IopSetTimer(VOID)
{
    if (RepaintTimerId)
    {
        return FALSE;
    }
    else
    {
        RepaintTimerId = SetTimer(NULL, 0, 10, IopRepaintFunc);
        return TRUE;
    }
}

static BOOL IopKillTimer(VOID)
{
    if (!RepaintTimerId)
    {
        return FALSE;
    }
    else
    {
        KillTimer(NULL, RepaintTimerId);
        RepaintTimerId = 0;
        return TRUE;
    }
}

/* Global functions *************************************************************/

BOOL IoInitializeIoHandler(VOID)
{
    return TRUE;
}

BOOL IoUninitializeIoHandler(VOID)
{
    IopKillTimer();
    return TRUE;
}

BOOL IoHandleSpaceKeyDown(VOID)
{
    /* 이미 스페이스 키가 눌려있으면 반환함 */

    if (SpaceKeyPressed)
    {
        return FALSE;
    }

    /* 타이머의 상태 얻기 */

    TimerStatus timerStatus = TmGetTimerStatus();

    switch (timerStatus)
    {
        /* 기록 중: 여기서 트리거하면 타이머가 멈춤 */
    case TM_RECORDING:
    {
        IopKillTimer();
        WCHAR szScrambleBuffer[256];
        ScScrambleToString(szScrambleBuffer, sizeof(szScrambleBuffer));
        RcAddRecord(TmGetElapsedTime(), szScrambleBuffer);
        ScDeleteScramble();
        ScGenerateScramble(SC_CUBE_33, rand() % 5 + 12);
        break;
    }

    /* 멈춤: 여기서 트리거하면 타이머 시작 대기가 시작됨 */
    case TM_STOP:
    {
        IopSetTimer();
        break;
    }
    }

    WndRepaintMainWindow();
    TmTrigger();

    SpaceKeyPressed = TRUE;
    return TRUE;
}

BOOL IoHandleSpaceKeyUp(VOID)
{
    /* 스페이스 키가 눌려있지 않다면 반환시킴 */

    if (!SpaceKeyPressed)
    {
        return FALSE;
    }

    /* 타이머의 상태 얻기 */

    TimerStatus timerStatus = TmGetTimerStatus();

    /* 멈춤 상태에서 키 업 이벤트가 발생함: 키 다운 핸들러가 타이머를 멈춤. 따라서 핸들링 안 함. */

    if (timerStatus == TM_STOP)
    {
        SpaceKeyPressed = FALSE;
        return TRUE;
    }

    /* 화면 다시 그리기 보내고 트리거하기 */

    WndRepaintMainWindow();
    timerStatus = TmTrigger();

    if (timerStatus == TM_RECORDING)
    {
        IopSetTimer();
    }
    else
    {
        IopKillTimer();
    }

    SpaceKeyPressed = FALSE;
    return TRUE;
}