#ifndef TIMER_WINDOW_H
#define TIMER_WINDOW_H

#include <Windows.h>

/**
 * @brief ���� �����츦 �����մϴ�.
 * @return �����ϸ� TRUE, �����ϸ� FALSE�� ��ȯ�մϴ�.
 */
extern BOOL WndCreateMainWindow(LPCWSTR lpWindowName, UINT uWIdth, UINT uHeight, WNDPROC proc);

/**
 * @brief ���� �����쿡 �ٽ� �׸��� ��û�� �����ϴ�. (InvalidateRect�� ȣ���մϴ�.)
 * @return �����ϸ� TRUE, �����ϸ� FALSE�� ��ȯ�մϴ�.
 */
extern BOOL WndRepaintMainWindow(VOID);

/**
 * @brief ���� �����쿡 Ư�� �޽����� �����ϴ�.
 * @return �����ϸ� TRUE, �����ϸ� FALSE�� ��ȯ�մϴ�.
 */
extern BOOL WndPostMessageMainWindow(UINT uMsg, WPARAM wParam, LPARAM lParam);

/**
 * @brief ���� �������� Rect�� ���ؼ� lpRect�� �����մϴ�.
 * @return �����ϸ� TRUE, �����ϸ� FALSE�� ��ȯ�մϴ�.
 */
extern BOOL WndGetWindowRect(LPRECT lpRect);

/**
 * @brief bShow�� ���� �����츦 ���̰� ����ϴ�.
 * @return ������ �������� ���¸� ��ȯ�մϴ�.
 */
extern BOOL WndShowMainWindow(BOOL bShow);

/**
 * @brief �������� �޽��� ť�� WM_PAINT�� �ִٸ� ��� �����츦 �ٽ� �׸��ϴ�.
 * @return �����ϸ� TRUE, �����ϸ� FALSE�� ��ȯ�մϴ�.
 */
extern BOOL WndUpdateMainWindow(VOID);

#endif