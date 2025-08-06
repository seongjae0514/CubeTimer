#include "Debug.h"

#include <Windows.h>
#include "Dialog.h"
#include "../resource/resource.h"

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

BOOL DlInitializeDialog(VOID)
{
	return TRUE;
}

BOOL DlUninitializeDialog(VOID)
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