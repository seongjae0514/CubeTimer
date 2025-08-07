#include "Debug.h"
#include <Windows.h>
#include "Window.h"
#include "Timer.h"
#include "IoHandler.h"
#include "Record.h"
#include "Scramble.h"
#include "Button.h"
#include "Dialog.h"

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
    /* �̹� �����̽� Ű�� ���������� ��ȯ�� */

    if (SpaceKeyPressed)
    {
        return FALSE;
    }

    /* Ÿ�̸��� ���� ��� */

    TimerStatus timerStatus = TmGetTimerStatus();

    switch (timerStatus)
    {
        /* ��� ��: ���⼭ Ʈ�����ϸ� Ÿ�̸Ӱ� ���� */
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

        /* ����: ���⼭ Ʈ�����ϸ� Ÿ�̸� ���� ��Ⱑ ���۵� */
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
    /* �����̽� Ű�� �������� �ʴٸ� ��ȯ��Ŵ */

    if (!SpaceKeyPressed)
    {
        return FALSE;
    }

    /* Ÿ�̸��� ���� ��� */

    TimerStatus timerStatus = TmGetTimerStatus();

    /* ���� ���¿��� Ű �� �̺�Ʈ�� �߻���: Ű �ٿ� �ڵ鷯�� Ÿ�̸Ӹ� ����. ���� �ڵ鸵 �� ��. */

    if (timerStatus == TM_STOP)
    {
        SpaceKeyPressed = FALSE;
        return TRUE;
    }

    /* ȭ�� �ٽ� �׸��� ������ Ʈ�����ϱ� */

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

BOOL IoHandleButtonPress(WPARAM wParam)
{
    switch (LOWORD(wParam))
    {
        /* �ʱ�ȭ ��ư ���� */
        case BUTTON_ID_INIT:
        {
            TmUninitializeTimer();
			RcUninitializeRecordTable();
			TmInitializeTimer();
			RcInitializeRecordTable();
            return TRUE;
        }

        /* +2 ��ư ���� */
        case BUTTON_ID_PLUSTWO:
        {
            WCHAR wBuffer[INPUT_DIALOG_MAX_INPUT_SIZE];

            BOOL ret = DlShowInputDialog(
                WndGetMainWindowHandle(),
                L"�Է�",
                L"�� ��° ��Ͽ� +2�� �Ͻðڽ��ϱ�?",
                wBuffer
            );

            if (!ret)
            {
                return TRUE;
            }

            INT index = _wtoi(wBuffer);

            if (index < 1)
            {
                MessageBoxW(WndGetMainWindowHandle(), L"�Է°��� �߸��Ǿ����ϴ�", L"����", MB_OK | MB_ICONERROR);
            }

            RcGoToFirstRecordNode();
			RcGoToNextRecordNode(index - 1);
            RcPlusTwoCurrentNodeRecord();
            WndRepaintMainWindow();
            return TRUE;
        }
    }
    return FALSE;
}