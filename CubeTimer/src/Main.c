#include "Debug.h"
#include <Windows.h>
#include "Window.h"
#include "Timer.h"
#include "Message.h"
#include "Renderer.h"
#include "Scramble.h"
#include "Record.h"
#include "IoHandler.h"
#include "Dialog.h"
#include "Button.h"

INT WINAPI wWinMain(_In_     HINSTANCE hInstance,
                    _In_opt_ HINSTANCE hPrevInstance,
                    _In_     LPWSTR    lpCmdLine,
                    _In_     INT       nCmdShow)
{
    /* 윈도우 생성 */

    if (!WndCreateMainWindow(L"CubeTimer", 1600, 1000, MsgGetWindowProc()))
    {
        MessageBoxW(NULL, L"윈도우 생성을 실패했습니다.", L"오류", MB_OK | MB_ICONERROR);
        return -1;
    }

    /* 모듈 초기화 */

	INITIALIZE_DEBUG_STREAM();

    TmInitializeTimer();
    RdInitializeRenderer(60, 18);
    IoInitializeIoHandler();
    ScInitializeScramble();
    RcInitializeRecordTable();
	DlInitializeDialog();
	BtnInitializeButton();

    // 첫 번째 스크램블 생성
    ScGenerateScramble(SC_CUBE_33, 15);

    /* 메시지 루프 시작 */

    WndShowMainWindow(TRUE);
    INT ret = MsgStartMessageLoop();

    /* 정리 및 반환 */

    TmUninitializeTimer();
    RdUninitializeRenderer();
    IoUninitializeIoHandler();
    ScUninitializeScramble();
    RcUninitializeRecordTable();

    PRINT_DEBUG_MEMORY_CHECK();

    return ret;
}