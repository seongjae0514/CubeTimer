#ifndef TIMER_WINDOW_H
#define TIMER_WINDOW_H

#include <Windows.h>

/**
 * @brief 메인 윈도우를 생성합니다.
 * @return 성공하면 TRUE, 실패하면 FALSE를 반환합니다.
 */
extern BOOL WndCreateMainWindow(LPCWSTR lpWindowName, UINT uWIdth, UINT uHeight, WNDPROC proc);

/**
 * @brief 메인 윈도우에 다시 그리기 요청을 보냅니다. (InvalidateRect를 호출합니다.)
 * @return 성공하면 TRUE, 실패하면 FALSE를 반환합니다.
 */
extern BOOL WndRepaintMainWindow(VOID);

/**
 * @brief 메인 윈도우에 특정 메시지를 보냅니다.
 * @return 성공하면 TRUE, 실패하면 FALSE를 반환합니다.
 */
extern BOOL WndPostMessageMainWindow(UINT uMsg, WPARAM wParam, LPARAM lParam);

/**
 * @brief 메인 윈도우의 Rect를 구해서 lpRect에 저장합니다.
 * @return 성공하면 TRUE, 실패하면 FALSE를 반환합니다.
 */
extern BOOL WndGetWindowRect(LPRECT lpRect);

/**
 * @brief bShow에 때라 윈도우를 보이고 숨깁니다.
 * @return 이전의 윈도우의 상태를 반환합니다.
 */
extern BOOL WndShowMainWindow(BOOL bShow);

/**
 * @brief 윈도우의 메시지 큐에 WM_PAINT가 있다면 즉시 윈도우를 다시 그립니다.
 * @return 성공하면 TRUE, 실패하면 FALSE를 반환합니다.
 */
extern BOOL WndUpdateMainWindow(VOID);

#endif