#ifndef TIMER_DIALOG_H
#define TIMER_DIALOG_H

#include <Windows.h>

#define INPUT_DIALOG_MAX_INPUT_SIZE 256

/**
 * @����� �ʱ�ȭ�մϴ�.
 * @return �����ϸ� TRUE, �����ϸ� FALSE�� ��ȯ�մϴ�.
 */
extern BOOL DlInitialize(VOID);

/**
 * @����� �ʱ�ȭ�մϴ�.
 * @return �����ϸ� TRUE, �����ϸ� FALSE�� ��ȯ�մϴ�.
 */
extern BOOL DlUninitialize(VOID);

/**
 * @brief ��ǲ ���̾�α׸� ǥ���մϴ�.
 * @return �����ϸ� TRUE, �����ϸ� FALSE�� ��ȯ�մϴ�.
 */
extern BOOL DlShowInputDialog(HWND hWnd, LPCWSTR lpszDialogTitle, LPCWSTR lpszDialogMessage, LPWSTR lpInputBuffer);

/**
 * @brief ���� ���� ��θ� ��ȯ�ϴ� ���̾�α׸� ǥ���մϴ�.
 * @return ���� �� TRUE, ��� �� FALSE�� ��ȯ�մϴ�.
 */
extern BOOL DlShowSaveFileDialog(HWND hWnd, LPWSTR lpPathBuffer, DWORD bufferSize, LPCWSTR lpstrFilter);

/**
 * @brief ���� ���� ��θ� ��ȯ�ϴ� ���̾�α׸� ǥ���մϴ�.
 * @return ���� �� TRUE, ��� �� FALSE�� ��ȯ�մϴ�.
 */
extern BOOL DlShowOpenFileDialog(HWND hWnd, LPWSTR lpPathBuffer, DWORD bufferSize, LPCWSTR lpstrFilter);

#endif