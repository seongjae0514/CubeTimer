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
		/* 다이얼로그 생성됨, 다이얼로그의 데이터 설정 */
	
		case WM_INITDIALOG:
		{
			InputDialogData* data = (InputDialogData*)lParam;

			/* 다이얼로그 제목 설정 */
			SetWindowTextW(hDlg, data->lpszTitle);
			
			/* 다이얼로그 텍스트 설정 */
			SetDlgItemTextW(hDlg, IDC_INPUTDLG_STATIC, data->lpszText);

			/* 다이얼로그 데이터를 다이얼로그에 저장 */
			SetWindowLongPtrW(hDlg, GWLP_USERDATA, lParam);

			return TRUE;
		}
		
		/* 버튼 눌림 이벤트 처리 */
		
		case WM_COMMAND:
		{
			switch (LOWORD(wParam))
			{
				case IDC_INPUTDLG_OK:
				{
					/* 다이얼로그에서 버퍼 주소 얻어서 버퍼에 입력값 저장 */

					LPWSTR lpBuffer = ((InputDialogData*)GetWindowLongPtrW(hDlg, GWLP_USERDATA))->lpBuffer;
					GetDlgItemTextW(hDlg, IDC_INPUTDLG_EDIT, lpBuffer, INPUT_DIALOG_MAX_INPUT_SIZE);

					/* 다이얼로그 닫기 */

					EndDialog(hDlg, IDOK);
					return TRUE;
				}
				case IDC_INPUTDLG_CANCEL:
				{
					/* 다이얼로그 닫기 */

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