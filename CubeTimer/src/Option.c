#include "Debug.h"

#include <Windows.h>
#include "Option.h"
#include "Resource.h"
#include "StrSafe.h"

/* Variables ****************************************/

UINT_PTR OptionValues[COUNT_OPTION_KEY];

/* Private functions ****************************************/

INT_PTR CALLBACK OptpDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_INITDIALOG:
		{
			/* �޺� �ڽ� �ʱ�ȭ */
			
			HWND hCombo = GetDlgItem(hDlg, IDC_OPTIONDLG_COMBO);

			SendMessageW(hCombo, CB_ADDSTRING, 0, (LPARAM)L"�ʽð�");
			SendMessageW(hCombo, CB_ADDSTRING, 0, (LPARAM)L"���� �Է�");

			UINT_PTR timerMode;
			OptGetKeyValue(OPTION_KEY_TIMER_MODE, &timerMode);

			SendMessageW(hCombo, CB_SETCURSEL, 1, 0);

			/* ���� �� �ҷ��ͼ� edit�� �־���� */

			HWND  hEdit1 = GetDlgItem(hDlg, IDC_OPTIONDLG_EDIT1);
			HWND  hEdit2 = GetDlgItem(hDlg, IDC_OPTIONDLG_EDIT2);
			WCHAR wBuffer[256];

			// hEdit1 (Ÿ�̸� ũ��)

			UINT_PTR timerSize;
			OptGetKeyValue(OPTION_KEY_TIMER_SIZE, &timerSize);
			StringCchPrintfW(
				wBuffer, _countof(wBuffer), L"%llu",
				timerSize
			);

			SetWindowTextW(hEdit1, wBuffer);

			// hEdit2 (��� ũ��)

			UINT_PTR recordSize;
			OptGetKeyValue(OPTION_KEY_RECORD_SIZE, &recordSize);
			StringCchPrintfW(
				wBuffer, _countof(wBuffer), L"%llu",
				recordSize
			);

			SetWindowTextW(hEdit2, wBuffer);

			return TRUE;
		}
		case WM_COMMAND:
		{
			switch (LOWORD(wParam))
			{
				case IDC_OPTIONDLG_OK:
				{
					/* �޺��ڽ� ������ ��� */

					HWND    hCombo = GetDlgItem(hDlg, IDC_OPTIONDLG_COMBO);
					WPARAM  idx    = SendMessageW(hCombo, CB_GETCURSEL, 0, 0);

					/* EDIT ������ ��� */

					HWND hEdit1 = GetDlgItem(hDlg, IDC_OPTIONDLG_EDIT1);
					HWND hEdit2 = GetDlgItem(hDlg, IDC_OPTIONDLG_EDIT2);

					WCHAR wBuffer[256];

					// Ÿ�̸� ũ��

					GetWindowTextW(hEdit1, wBuffer, _countof(wBuffer));
					UINT_PTR timerSize = _wtoi(wBuffer);

					// ��� ũ��

					GetWindowTextW(hEdit2, wBuffer, _countof(wBuffer));
					UINT_PTR recordSize = _wtoi(wBuffer);

					// ������ ����

					OptSetKeyValue(OPTION_KEY_TIMER_SIZE, timerSize);
					OptSetKeyValue(OPTION_KEY_RECORD_SIZE, recordSize);
					OptSetKeyValue(OPTION_KEY_TIMER_MODE, idx);

					// ����

					EndDialog(hDlg, TRUE);
					return TRUE;
				}
				case IDC_OPTIONDLG_CANCEL:
				{
					EndDialog(hDlg, FALSE);
					return TRUE;
				}
			}
			break;
		}
	}
	return FALSE;
}

/* Global functions ****************************************/

BOOL OptInitialize(VOID)
{
	OptionValues[OPTION_KEY_TIMER_SIZE] = 80;
	OptionValues[OPTION_KEY_RECORD_SIZE] = 21;
	OptionValues[OPTION_KEY_TIMER_MODE] = 0;
	
	return TRUE;
}

BOOL OptUninitialize(VOID)
{
	return TRUE;
}

BOOL OptSetKeyValue(OptionKey key, UINT_PTR value)
{
	OptionValues[key] = value;
	return TRUE;
}

BOOL OptGetKeyValue(OptionKey key, UINT_PTR* pValue)
{
	if (!pValue || key >= COUNT_OPTION_KEY || key < 0)
	{
		return FALSE;
	}

	*pValue = OptionValues[key];

	return TRUE;
}

BOOL OptSetOptions(HWND hwndParent)
{
	return (BOOL)DialogBoxParamW(GetModuleHandleW(NULL),
								 MAKEINTRESOURCE(IDD_OPTIONDLG),
								 hwndParent,
								 OptpDialogProc,
								 0);
}
