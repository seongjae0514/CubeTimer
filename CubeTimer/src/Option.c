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
			/* 콤보 박스 초기화 */
			
			HWND hCombo = GetDlgItem(hDlg, IDC_OPTIONDLG_COMBO);

			SendMessageW(hCombo, CB_ADDSTRING, 0, (LPARAM)L"초시계");
			SendMessageW(hCombo, CB_ADDSTRING, 0, (LPARAM)L"직접 입력");

			UINT_PTR timerMode;
			OptGetKeyValue(OPTION_KEY_TIMER_MODE, &timerMode);

			SendMessageW(hCombo, CB_SETCURSEL, 1, 0);

			/* 원래 값 불러와서 edit에 넣어놓기 */

			HWND  hEdit1 = GetDlgItem(hDlg, IDC_OPTIONDLG_EDIT1);
			HWND  hEdit2 = GetDlgItem(hDlg, IDC_OPTIONDLG_EDIT2);
			WCHAR wBuffer[256];

			// hEdit1 (타이머 크기)

			UINT_PTR timerSize;
			OptGetKeyValue(OPTION_KEY_TIMER_SIZE, &timerSize);
			StringCchPrintfW(
				wBuffer, _countof(wBuffer), L"%llu",
				timerSize
			);

			SetWindowTextW(hEdit1, wBuffer);

			// hEdit2 (기록 크기)

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
					/* 콤보박스 데이터 얻기 */

					HWND    hCombo = GetDlgItem(hDlg, IDC_OPTIONDLG_COMBO);
					WPARAM  idx    = SendMessageW(hCombo, CB_GETCURSEL, 0, 0);

					/* EDIT 데이터 얻기 */

					HWND hEdit1 = GetDlgItem(hDlg, IDC_OPTIONDLG_EDIT1);
					HWND hEdit2 = GetDlgItem(hDlg, IDC_OPTIONDLG_EDIT2);

					WCHAR wBuffer[256];

					// 타이머 크기

					GetWindowTextW(hEdit1, wBuffer, _countof(wBuffer));
					UINT_PTR timerSize = _wtoi(wBuffer);

					// 기록 크기

					GetWindowTextW(hEdit2, wBuffer, _countof(wBuffer));
					UINT_PTR recordSize = _wtoi(wBuffer);

					// 데이터 저장

					OptSetKeyValue(OPTION_KEY_TIMER_SIZE, timerSize);
					OptSetKeyValue(OPTION_KEY_RECORD_SIZE, recordSize);
					OptSetKeyValue(OPTION_KEY_TIMER_MODE, idx);

					// 종료

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
