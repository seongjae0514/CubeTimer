#include "Debug.h"

#include <Windows.h>
#include "Button.h"
#include "Window.h"

/* Defs ********************************************************************************/

#define BUTTON_WIDTH  150
#define BUTTON_HEIGHT 20

/* Variables ***************************************************************************/

HWND InitButtonHandle;
HWND PlusTwoButtonHandle;

/* Private functions *******************************************************************/

HWND BtnpCreateButton(LPCWSTR lpszButtonText, UINT width, UINT height, INT x, INT y, WORD buttonEventId)
{
	HWND hButton = CreateWindowExW(
		0,
		L"BUTTON",
		lpszButtonText,
		WS_CHILD | WS_VISIBLE | BS_FLAT | BS_PUSHBUTTON,
		x, y,
		width, height,
		WndGetMainWindowHandle(),
		(HMENU)(UINT_PTR)buttonEventId,
		GetModuleHandleW(NULL),
		NULL
	);

	return hButton;
}

BOOL BtnpMoveButton(HWND hButton, INT x, INT y, UINT width, UINT height, BOOL bRepaint)
{
	return MoveWindow(hButton, x, y, width, height, bRepaint);
}

/* Global functions ********************************************************************/

BOOL BtnInitializeButton(VOID)
{
	RECT windowRect;
	WndGetWindowRect(&windowRect);

	InitButtonHandle = BtnpCreateButton(
		L"√ ±‚»≠",
		BUTTON_WIDTH, BUTTON_HEIGHT,
		windowRect.right - 20 - BUTTON_WIDTH, 20,
		BUTTON_ID_INIT
	);

	if (!InitButtonHandle)
	{
		return FALSE;
	}

	PlusTwoButtonHandle = BtnpCreateButton(
		L"+2",
		BUTTON_WIDTH, BUTTON_HEIGHT,
		windowRect.right - 20 - BUTTON_WIDTH, 20 + BUTTON_HEIGHT + 10, 
		BUTTON_ID_PLUSTWO
	);

	if (!PlusTwoButtonHandle)
	{
		return FALSE;
	}

	BtnResetButtonPos();

	return TRUE;
}

BOOL BtnUninitializeButton(VOID)
{
	return TRUE;
}

BOOL BtnResetButtonPos(VOID)
{
	if (!InitButtonHandle || !PlusTwoButtonHandle)
	{
		return FALSE;
	}

	RECT clientRect;
	WndGetWindowRect(&clientRect);

	BtnpMoveButton(
		InitButtonHandle,
		clientRect.right - BUTTON_WIDTH - 20, 20,
		BUTTON_WIDTH, BUTTON_HEIGHT, TRUE
	);

	BtnpMoveButton(
		PlusTwoButtonHandle,
		clientRect.right - BUTTON_WIDTH - 20, 20 + BUTTON_HEIGHT + 10,
		BUTTON_WIDTH, BUTTON_HEIGHT, TRUE
	);

	return TRUE;
}
