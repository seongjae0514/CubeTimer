#include "Debug.h"
#include <Windows.h>
#include "Window.h"

static LPCWSTR ClassName = L"CubeTimerMainWindow";
static HWND    MainWindow = NULL;

BOOL WndCreateMainWindow(LPCWSTR lpTitle, UINT uWidth, UINT uHeight, WNDPROC proc)
{
    if (MainWindow != NULL || lpTitle == NULL)
    {
        return FALSE;
    }

    HINSTANCE   hInstance = GetModuleHandleW(NULL);
    WNDCLASSEXW wc        = { 0 };

    wc.cbSize        = sizeof(WNDCLASSEXW);
    wc.hInstance     = hInstance;
    wc.lpfnWndProc   = proc;
    wc.lpszClassName = ClassName;
    wc.hCursor       = LoadCursorW(NULL, IDC_ARROW);

    if (!RegisterClassExW(&wc))
    {
        return FALSE;
    }

    MainWindow = CreateWindowExW(
        0,
        ClassName,
        lpTitle,
        WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
        CW_USEDEFAULT, CW_USEDEFAULT,
        uWidth, uHeight,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (!MainWindow)
    {
        return FALSE;
    }

    return TRUE;
}

BOOL WndRepaintMainWindow(VOID)
{
    return InvalidateRect(MainWindow, NULL, FALSE);
}

BOOL WndPostMessageMainWindow(UINT m, WPARAM w, LPARAM l)
{
    return PostMessageW(MainWindow, m, w, l);
}

BOOL WndGetWindowRect(LPRECT lpRect)
{
    return GetClientRect(MainWindow, lpRect);
}

BOOL WndShowMainWindow(BOOL bShow)
{
    return ShowWindow(MainWindow, bShow ? SW_SHOW : SW_HIDE);
}

BOOL WndUpdateMainWindow(VOID)
{
    return UpdateWindow(MainWindow);
}

HWND WndGetMainWindowHandle(VOID)
{
    return MainWindow;
}