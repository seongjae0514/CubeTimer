#ifndef TIMER_DIALOG_H
#define TIMER_DIALOG_H

#include <Windows.h>

#define INPUT_DIALOG_MAX_INPUT_SIZE 256

/**
 * @모듈을 초기화합니다.
 * @return 성공하면 TRUE, 실패하면 FALSE를 반환합니다.
 */
extern BOOL DlInitializeDialog(VOID);

/**
 * @모듈을 초기화합니다.
 * @return 성공하면 TRUE, 실패하면 FALSE를 반환합니다.
 */
extern BOOL DlUninitializeDialog(VOID);

/**
 * @brief 인풋 다이얼로그를 표시합니다.
 * @return 성공하면 TRUE, 실패하면 FALSE를 반환합니다.
 */
extern BOOL DlShowInputDialog(HWND hWnd, LPCWSTR lpszDialogTitle, LPCWSTR lpszDialogMessage, LPWSTR lpInputBuffer);

#endif