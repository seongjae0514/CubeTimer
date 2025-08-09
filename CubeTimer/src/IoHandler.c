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

BOOL IoHandleButtonPress(WPARAM wParam)
{
    switch (LOWORD(wParam))
    {
        /* 초기화 버튼 눌림 */
        case BUTTON_ID_INIT:
        {
            TmUninitializeTimer();
			RcUninitialize();
			TmInitializeTimer();
			RcInitialize();
            RdInitializeScroll();
            return TRUE;
        }

        /* +2 버튼 눌림 */
        case BUTTON_ID_PLUSTWO:
        {
            WCHAR wBuffer[INPUT_DIALOG_MAX_INPUT_SIZE];

            BOOL ret = DlShowInputDialog(
                WndGetMainWindowHandle(),
                L"입력",
                L"몇 번째 기록에 +2를 하시겠습니까?",
                wBuffer
            );

            if (!ret)
            {
                return TRUE;
            }

            INT index = _wtoi(wBuffer);

            if (index < 1)
            {
                MessageBoxW(WndGetMainWindowHandle(), L"입력값이 잘못되었습니다", L"오류", MB_OK | MB_ICONERROR);
            }

            if (!RcTogglePlusTwo(index - 1))
            {
                MessageBoxW(WndGetMainWindowHandle(), L"입력값이 잘못되었습니다.", L"오류", MB_OK | MB_ICONERROR);
            }

            WndRepaintMainWindow();
            return TRUE;
        }

        /* 기록 저장 버튼 눌림 */
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
                        L"저장 성공", L"알림",
                        MB_OK | MB_ICONINFORMATION
                    );
                    break;
                }

                case EACCES:
                {
                    MessageBoxW(
                        WndGetMainWindowHandle(),
                        L"저장을 실패했습니다. 권한이 부족합니다.",
                        L"오류",
                        MB_OK | MB_ICONERROR
                    );
                    break;
                }

                default:
                {
                    WCHAR wBuffer[256];
                    wsprintfW(wBuffer, L"저장을 실패했습니다. 알 수 없는 오류가 발생했습니다. 오류 코드: %d", ret);
                    MessageBoxW(
                        WndGetMainWindowHandle(),
                        wBuffer,
                        L"오류",
                        MB_ICONERROR | MB_OK
                    );
                    break;
                }
            }
            return TRUE;
        }

        /* 기록 불러오기 버튼 눌림 */
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
                        L"불러오기 성공", L"알림",
                        MB_OK | MB_ICONINFORMATION
                    );
                    break;
                }

                case EACCES:
                {
                    MessageBoxW(
                        WndGetMainWindowHandle(),
                        L"파일 열기를 실패했습니다. 권한이 부족합니다.",
                        L"오류",
                        MB_OK | MB_ICONERROR
                    );
                    break;
                }

                case RECORD_FILE_ERROR_INVALID_FILE_TYPE:
                {
                    MessageBoxW(
                        WndGetMainWindowHandle(),
                        L"불러오기를 실패했습니다. \r\n올바른 큐브 타이머 기록 파일 (.ctr) 이 아닙니다. \r\n파일이 손상되었을 수 있습니다.",
                        L"오류",
                        MB_OK | MB_ICONERROR
                    );
                    break;
                }

                case RECORD_ERROR_ALLOCATION_FAILED:
                {
                    MessageBoxW(
                        WndGetMainWindowHandle(),
                        L"메모리 할당을 실패했습니다. \r\n메모리가 손상되었거나 파일이 잘못되었을 수 있습니다.",
                        L"오류",
                        MB_OK | MB_ICONERROR
                    );
                    break;
                }

                case RECORD_ERROR_ADD_RECORD_FAILED:
                {
                    MessageBoxW(
                        WndGetMainWindowHandle(),
                        L"기록 추가를 실패했습니다. \r\n메모리가 손상되었거나 파일이 잘못되었을 수 있습니다.",
                        L"오류",
                        MB_OK | MB_ICONERROR
                    );
                    break;
                }

                case RECORD_FILE_ERROR_VALIDATION_CODE_NOT_EQUAL:
                {
                    MessageBoxW(
                        WndGetMainWindowHandle(),
                        L"기록 파일이 올바른 큐브 타이머 기록 파일 (*.ctr) 이 아닙니다.\r\n"
                        "파일이 손상된 것 같습니다.\r\n"
                        "파일이 0.9.1 이전의 버전에서 생성된 경우 이런 오류가 발생할 수 있습니다.\r\n"
                        "해당 버전 미만의 타이머에서 생성한 기록은 더이상 호환되지 않습니다.\r\n"
                        "기록이 불러와지긴 하였으나 문제가 있을 수 있습니다.",
                        L"오류",
                        MB_OK | MB_ICONERROR
                    );
                    break;
                }

                default:
                {
                    WCHAR wBuffer[256];
                    wsprintfW(wBuffer, L"파일 열기를 실패했습니다. 알 수 없는 오류가 발생했습니다. 오류 코드: %d", ret);
                    MessageBoxW(
                        WndGetMainWindowHandle(),
                        wBuffer,
                        L"오류",
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