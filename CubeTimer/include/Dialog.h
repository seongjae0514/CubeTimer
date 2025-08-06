#ifndef TIMER_DIALOG_H
#define TIMER_DIALOG_H

#include <Windows.h>

#define INPUT_DIALOG_MAX_INPUT_SIZE 256

/**
 * @����� �ʱ�ȭ�մϴ�.
 * @return �����ϸ� TRUE, �����ϸ� FALSE�� ��ȯ�մϴ�.
 */
extern BOOL DlInitializeDialog(VOID);

/**
 * @����� �ʱ�ȭ�մϴ�.
 * @return �����ϸ� TRUE, �����ϸ� FALSE�� ��ȯ�մϴ�.
 */
extern BOOL DlUninitializeDialog(VOID);

/**
 * @brief ��ǲ ���̾�α׸� ǥ���մϴ�.
 * @return �����ϸ� TRUE, �����ϸ� FALSE�� ��ȯ�մϴ�.
 */
extern BOOL DlShowInputDialog(HWND hWnd, LPCWSTR lpszDialogTitle, LPCWSTR lpszDialogMessage, LPWSTR lpInputBuffer);

#endif