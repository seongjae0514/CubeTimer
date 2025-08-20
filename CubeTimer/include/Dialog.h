#ifndef TIMER_DIALOG_H
#define TIMER_DIALOG_H

#include <Windows.h>

#define INPUT_DIALOG_MAX_INPUT_SIZE 256

/**
 * @모듈을 초기화합니다.
 * @return 성공하면 TRUE, 실패하면 FALSE를 반환합니다.
 */
extern BOOL DlInitialize(VOID);

/**
 * @모듈을 초기화합니다.
 * @return 성공하면 TRUE, 실패하면 FALSE를 반환합니다.
 */
extern BOOL DlUninitialize(VOID);

/**
 * @brief 인풋 다이얼로그를 표시합니다.
 * @return 성공하면 TRUE, 실패하면 FALSE를 반환합니다.
 */
extern BOOL DlShowInputDialog(HWND hWnd, LPCWSTR lpszDialogTitle, LPCWSTR lpszDialogMessage, LPWSTR lpInputBuffer);

/**
 * @brief 파일 저장 경로를 반환하는 다이얼로그를 표시합니다.
 * @return 성공 시 TRUE, 취소 시 FALSE를 반환합니다.
 */
extern BOOL DlShowSaveFileDialog(HWND hWnd, LPWSTR lpPathBuffer, DWORD bufferSize, LPCWSTR lpstrFilter);

/**
 * @brief 파일 열기 경로를 반환하는 다이얼로그를 표시합니다.
 * @return 성공 시 TRUE, 취소 시 FALSE를 반환합니다.
 */
extern BOOL DlShowOpenFileDialog(HWND hWnd, LPWSTR lpPathBuffer, DWORD bufferSize, LPCWSTR lpstrFilter);

#endif