#include "Debug.h"
#include "Message.h"
#include "IoHandler.h"
#include "Renderer.h"
#include "Window.h"
#include "Button.h"
#include "Layout.h"
#include "Resource.h"

/* Private function *****************************************/

static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_DESTROY:
        {
            PostQuitMessage(0);
            return 0;
        }

        case WM_KEYDOWN:
        {
            if (wParam == VK_SPACE)
            {
                IoHandleSpaceKeyDown();
            }
            return 0;
        }

        case WM_KEYUP:
        {
            if (wParam == VK_SPACE)
            {
                IoHandleSpaceKeyUp();
            }
            return 0;
        }

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC         hWindowDC;

            hWindowDC = BeginPaint(hWnd, &ps);

            /* ���۹� ���� */

            RECT windowRect;
            WndGetWindowRect(&windowRect);

            HDC     hMemDC = CreateCompatibleDC(hWindowDC);
            HBITMAP hMemBitmap = CreateCompatibleBitmap(hWindowDC, windowRect.right, windowRect.bottom);
            HBITMAP hOldBitmap = SelectObject(hMemDC, hMemBitmap);
            FillRect(hMemDC, &windowRect, (HBRUSH)(COLOR_WINDOW + 1));

            /* �׸��� */

            LoRenderAll(hMemDC);

            /* ���۹� ���� */

            BitBlt(hWindowDC, 0, 0, windowRect.right, windowRect.bottom, hMemDC, 0, 0, SRCCOPY);
            SelectObject(hMemDC, hOldBitmap);
            DeleteObject(hMemBitmap);
            DeleteDC(hMemDC);

            /* ���� �� ��ȯ */

            EndPaint(hWnd, &ps);
            return 0;
        }

        case WM_SIZE:
        {
            WndRepaintMainWindow();
            return 0;
        }

        case WM_USER_BUTTON_CLICKED:
        {
            IoHandleButtonPress(wParam);
            WndRepaintMainWindow();
            return 0;
        }

        case WM_COMMAND:
        {
            if (LOWORD(wParam) >= IDM_INFO && LOWORD(wParam) <= IDM_END)
            {
                IoHandleMenuClick(LOWORD(wParam));
            }

            return 0;
        }

        case WM_MOUSEWHEEL:
        {
            IoHandleMouseWheel(wParam, lParam);
            return 0;
        }

        case WM_MOUSEMOVE:
        {
            POINT pt = {
                .x = LOWORD(lParam),
                .y = HIWORD(lParam)
            };
            LoResetMousePosition(&pt);
            WndRepaintMainWindow();
            return 0;
        }

        case WM_LBUTTONDOWN:
        {
            POINT pt = {
                .x = LOWORD(lParam),
                .y = HIWORD(lParam)
            };
            LoMouseClick(&pt);
            WndRepaintMainWindow();
            return 0;
        }
    }

    return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}

/* Global function *****************************************/

WNDPROC MsgGetWindowProc(VOID)
{
    return WindowProc;
}

INT MsgStartMessageLoop(VOID)
{
    MSG msg;
    while (GetMessageW(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    return (INT)msg.wParam;
}
