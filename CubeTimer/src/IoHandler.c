#include "Debug.h"
#include <Windows.h>
#include "Window.h"
#include "Timer.h"
#include "IoHandler.h"
#include "Record.h"
#include "Scramble.h"
#include "Button.h"
#include "Dialog.h"
#include "Renderer.h"

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
			RcUninitialize();
			TmInitializeTimer();
			RcInitialize();
            RdInitializeScroll();
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

            if (!RcTogglePlusTwo(index - 1))
            {
                MessageBoxW(WndGetMainWindowHandle(), L"�Է°��� �߸��Ǿ����ϴ�.", L"����", MB_OK | MB_ICONERROR);
            }

            WndRepaintMainWindow();
            return TRUE;
        }

        /* ��� ���� ��ư ���� */
        case BUTTON_ID_SAVERECORD:
        {
            WCHAR wBuffer[MAX_PATH] = { 0 };
            BOOL dlRet = DlShowSaveFileDialog(WndGetMainWindowHandle(), wBuffer, _countof(wBuffer), L"Cube timer record file (*.ctr)\0*.ctr\0All files\0*.*\0");

            if (!dlRet)
            {
                return TRUE;
            }

            INT ret = RcSaveRecordToFile(wBuffer);

            switch (ret)
            {
                case 0:
                {
                    MessageBoxW(
                        WndGetMainWindowHandle(),
                        L"���� ����", L"�˸�",
                        MB_OK | MB_ICONINFORMATION
                    );
                    break;
                }

                case EACCES:
                {
                    MessageBoxW(
                        WndGetMainWindowHandle(),
                        L"������ �����߽��ϴ�. ������ �����մϴ�.",
                        L"����",
                        MB_OK | MB_ICONERROR
                    );
                    break;
                }

                default:
                {
                    WCHAR wBuffer[256];
                    wsprintfW(wBuffer, L"������ �����߽��ϴ�. �� �� ���� ������ �߻��߽��ϴ�. ���� �ڵ�: %d", ret);
                    MessageBoxW(
                        WndGetMainWindowHandle(),
                        wBuffer,
                        L"����",
                        MB_ICONERROR | MB_OK
                    );
                    break;
                }
            }
            return TRUE;
        }

        /* ��� �ҷ����� ��ư ���� */
        case BUTTON_ID_LOADRECORD:
        {
            WCHAR wBuffer[MAX_PATH] = { 0 };
            BOOL dlRet = DlShowOpenFileDialog(WndGetMainWindowHandle(), wBuffer, _countof(wBuffer), L"Cube timer record file (*.ctr)\0*.ctr\0All files\0*.*\0");

            if (!dlRet)
            {
                return TRUE;
            }

            INT ret = RcLoadRecordFromFile(wBuffer);

            switch (ret)
            {
                case 0:
                {
                    MessageBoxW(
                        WndGetMainWindowHandle(),
                        L"�ҷ����� ����", L"�˸�",
                        MB_OK | MB_ICONINFORMATION
                    );
                    break;
                }

                case EACCES:
                {
                    MessageBoxW(
                        WndGetMainWindowHandle(),
                        L"���� ���⸦ �����߽��ϴ�. ������ �����մϴ�.",
                        L"����",
                        MB_OK | MB_ICONERROR
                    );
                    break;
                }

                case RECORD_FILE_ERROR_INVALID_FILE_TYPE:
                {
                    MessageBoxW(
                        WndGetMainWindowHandle(),
                        L"�ҷ����⸦ �����߽��ϴ�. \r\n�ùٸ� ť�� Ÿ�̸� ��� ���� (.ctr) �� �ƴմϴ�. \r\n������ �ջ�Ǿ��� �� �ֽ��ϴ�.",
                        L"����",
                        MB_OK | MB_ICONERROR
                    );
                    break;
                }

                case RECORD_ERROR_ALLOCATION_FAILED:
                {
                    MessageBoxW(
                        WndGetMainWindowHandle(),
                        L"�޸� �Ҵ��� �����߽��ϴ�. \r\n�޸𸮰� �ջ�Ǿ��ų� ������ �߸��Ǿ��� �� �ֽ��ϴ�.",
                        L"����",
                        MB_OK | MB_ICONERROR
                    );
                    break;
                }

                case RECORD_ERROR_ADD_RECORD_FAILED:
                {
                    MessageBoxW(
                        WndGetMainWindowHandle(),
                        L"��� �߰��� �����߽��ϴ�. \r\n�޸𸮰� �ջ�Ǿ��ų� ������ �߸��Ǿ��� �� �ֽ��ϴ�.",
                        L"����",
                        MB_OK | MB_ICONERROR
                    );
                    break;
                }

                case RECORD_FILE_ERROR_VALIDATION_CODE_NOT_EQUAL:
                {
                    MessageBoxW(
                        WndGetMainWindowHandle(),
                        L"��� ������ �ùٸ� ť�� Ÿ�̸� ��� ���� (*.ctr) �� �ƴմϴ�.\r\n"
                        "������ �ջ�� �� �����ϴ�.\r\n"
                        "������ 0.9.1 ������ �������� ������ ��� �̷� ������ �߻��� �� �ֽ��ϴ�.\r\n"
                        "�ش� ���� �̸��� Ÿ�̸ӿ��� ������ ����� ���̻� ȣȯ���� �ʽ��ϴ�.\r\n"
                        "����� �ҷ������� �Ͽ����� ������ ���� �� �ֽ��ϴ�.",
                        L"����",
                        MB_OK | MB_ICONERROR
                    );
                    break;
                }

                default:
                {
                    WCHAR wBuffer[256];
                    wsprintfW(wBuffer, L"���� ���⸦ �����߽��ϴ�. �� �� ���� ������ �߻��߽��ϴ�. ���� �ڵ�: %d", ret);
                    MessageBoxW(
                        WndGetMainWindowHandle(),
                        wBuffer,
                        L"����",
                        MB_ICONERROR | MB_OK
                    );
                    break;
                }
            }
            return TRUE;
        }
    }
    return FALSE;
}

BOOL IoHandleMouseWheel(WPARAM wParam, LPARAM lParam)
{
    INT count = GET_WHEEL_DELTA_WPARAM(wParam) / 120;

    if (count > 0)
    {
        for (UINT i = 0; i < count; i++)
        {
            RdScrollRecord(FALSE, WndGetMainWindowHandle());
        }
    }
    else
    {
        for (UINT i = 0; i < -count; i++)
        {
            RdScrollRecord(TRUE, WndGetMainWindowHandle());
        }
    }

    WndRepaintMainWindow();

    return TRUE;
}