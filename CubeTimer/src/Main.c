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
#include "Layout.h"
#include "Option.h"

INT WINAPI wWinMain(_In_     HINSTANCE hInstance,
                    _In_opt_ HINSTANCE hPrevInstance,
                    _In_     LPWSTR    lpCmdLine,
                    _In_     INT       nCmdShow)
{
    /* ������ ���� */

    if (!WndCreateMainWindow(L"CubeTimer", 1600, 1000, MsgGetWindowProc()))
    {
        MessageBoxW(NULL, L"������ ������ �����߽��ϴ�.", L"����", MB_OK | MB_ICONERROR);
        return -1;
    }

    /* ��� �ʱ�ȭ */

	INITIALIZE_DEBUG_STREAM();

    TmInitializeTimer();
    LoInitialize();
    IoInitializeIoHandler();
    ScInitializeScramble();
    RcInitialize();
	DlInitializeDialog();
    OptInitialize();

    // ù ��° ��ũ���� ����
    ScGenerateScramble(SC_CUBE_33, 15);

    /* �޽��� ���� ���� */

    WndShowMainWindow(TRUE);
    INT ret = MsgStartMessageLoop();

    /* ���� �� ��ȯ */

    TmUninitializeTimer();
    LoUninitialize();
    IoUninitializeIoHandler();
    ScUninitializeScramble();
    RcUninitialize();
    DlUninitializeDialog();

    PRINT_DEBUG_MEMORY_CHECK();

    return ret;
}