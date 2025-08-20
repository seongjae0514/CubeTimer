#include "Debug.h"

#include <Windows.h>
#include <commdlg.h>

#include "Dialog.h"
#include "resource.h"

/* Variables **************************************************/

typedef struct tagInputDialogData {
	LPCWSTR lpszTitle, lpszText;
	LPWSTR  lpBuffer;
} InputDialogData;

/* Private functions ******************************************/

UINT_PTR DlpInputDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		/* ���̾�α� ������, ���̾�α��� ������ ���� */
	
		case WM_INITDIALOG:
		{
			InputDialogData* data = (InputDialogData*)lParam;

			/* ���̾�α� ���� ���� */
			SetWindowTextW(hDlg, data->lpszTitle);
			
			/* ���̾�α� �ؽ�Ʈ ���� */
			SetDlgItemTextW(hDlg, IDC_INPUTDLG_STATIC, data->lpszText);

			/* ���̾�α� �����͸� ���̾�α׿� ���� */
			SetWindowLongPtrW(hDlg, GWLP_USERDATA, lParam);

			return TRUE;
		}
		
		/* ��ư ���� �̺�Ʈ ó�� */
		
		case WM_COMMAND:
		{
			switch (LOWORD(wParam))
			{
				case IDC_INPUTDLG_OK:
				{
					/* ���̾�α׿��� ���� �ּ� �� ���ۿ� �Է°� ���� */

					LPWSTR lpBuffer = ((InputDialogData*)GetWindowLongPtrW(hDlg, GWLP_USERDATA))->lpBuffer;
					GetDlgItemTextW(hDlg, IDC_INPUTDLG_EDIT, lpBuffer, INPUT_DIALOG_MAX_INPUT_SIZE);

					/* ���̾�α� �ݱ� */

					EndDialog(hDlg, IDOK);
					return TRUE;
				}
				case IDC_INPUTDLG_CANCEL:
				{
					/* ���̾�α� �ݱ� */

					EndDialog(hDlg, IDCANCEL);
					return TRUE;
				}
			}
			break;
		}
	}
	return FALSE;
}

/* Global functions *******************************************/

BOOL DlInitialize(VOID)
{
	return TRUE;
}

BOOL DlUninitialize(VOID)
{
	return TRUE;
}

BOOL DlShowInputDialog(HWND hWnd, LPCWSTR lpszDialogTitle, LPCWSTR lpszDialogMessage, LPWSTR lpInputBuffer)
{
	InputDialogData data = {
		.lpszTitle = lpszDialogTitle,
		.lpszText = lpszDialogMessage,
		.lpBuffer = lpInputBuffer
	};

	INT_PTR result = DialogBoxParamW(GetModuleHandleW(NULL), MAKEINTRESOURCE(IDD_INPUTDLG), hWnd, DlpInputDialogProc, (LPARAM)&data);
	return result == IDOK;
}

BOOL DlShowSaveFileDialog(HWND hWnd, LPWSTR lpPathBuffer, DWORD bufferSize, LPCWSTR lpstrFilter)
{
	OPENFILENAME ofn = { 0 };

	ofn.lStructSize     = sizeof(ofn);
	ofn.hwndOwner       = hWnd;
	ofn.lpstrFile       = lpPathBuffer;
	ofn.nMaxFile        = (DWORD)bufferSize;
	ofn.lpstrFilter     = lpstrFilter;
	ofn.nFilterIndex    = 1;
	ofn.lpstrFileTitle  = NULL;
	ofn.nMaxFileTitle   = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags           = OFN_OVERWRITEPROMPT;

	return GetSaveFileNameW(&ofn);
}

BOOL DlShowOpenFileDialog(HWND hWnd, LPWSTR lpPathBuffer, DWORD bufferSize, LPCWSTR lpstrFilter)
{
	OPENFILENAME ofn = { 0 };

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFile = lpPathBuffer;
	ofn.nMaxFile = (DWORD)bufferSize;
	ofn.lpstrFilter = lpstrFilter;
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	return GetOpenFileNameW(&ofn);
}