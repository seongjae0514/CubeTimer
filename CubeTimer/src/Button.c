#include "Debug.h"

#include <Windows.h>

#include "Button.h"
#include "Window.h"

/* Defines *****************************************************************/

typedef struct tagButtonStruct {
	RECT     rect;
	LPCWSTR  lpszText;
	BOOL     bHovered;
} ButtonStruct;

/* Variables ***************************************************************/

ButtonStruct Buttons[BUTTON_COUNT] = { 0 };
LPCWSTR      ButtonTexts[BUTTON_COUNT] = { L"초기화", L"+2", L"불러오기", L"저장" };

HBRUSH       hDefaultButtonBrush;
HBRUSH       hHoverButtonBrush;
HFONT        hButtonFont;

/* Global functions ********************************************************/

BOOL BtnInitialize(VOID)
{
	for (int i = 0; i < BUTTON_COUNT; i++)
	{
		Buttons[i].lpszText = ButtonTexts[i];
		Buttons[i].bHovered = FALSE;
	}

	hDefaultButtonBrush = CreateSolidBrush(RGB(230, 230, 230));
	hHoverButtonBrush = CreateSolidBrush(RGB(200, 200, 200));
	hButtonFont = CreateFontW(
		18, 0,
		0, 0,
		FW_MEDIUM,
		FALSE, FALSE, FALSE,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH,
		L"맑은 고딕"
	);

	BtnResetButtonDivisionPosition(0, 0);

	return TRUE;
}

BOOL BtnUninitialize(VOID)
{
	DeleteObject(hDefaultButtonBrush);
	DeleteObject(hHoverButtonBrush);
	DeleteObject(hButtonFont);

	return TRUE;
}

BOOL BtnGetButtonDivisionSize(LPSIZE lpSize)
{
	if (!lpSize)
	{
		return FALSE;
	}

	lpSize->cy = BUTTON_HEIGHT;
	lpSize->cx = BUTTON_WIDTH * BUTTON_COUNT + BUTTON_PADDING * (BUTTON_COUNT - 1);
	return TRUE;
}

BOOL BtnResetButtonDivisionPosition(INT x, INT y)
{
	Buttons[0].rect.left = x;
	Buttons[0].rect.right = x + BUTTON_WIDTH;
	Buttons[0].rect.top = y;
	Buttons[0].rect.bottom = y + BUTTON_HEIGHT;

	for (int i = 1; i < BUTTON_COUNT; i++)
	{
		Buttons[i].rect.left = Buttons[i - 1].rect.right + BUTTON_PADDING;
		Buttons[i].rect.right = Buttons[i].rect.left + BUTTON_WIDTH;
		Buttons[i].rect.top = y;
		Buttons[i].rect.bottom = y + BUTTON_HEIGHT;
	}

	return TRUE;
}

BOOL BtnResetMousePosition(LPPOINT lpMousePointer)
{
	if (!lpMousePointer)
	{
		return FALSE;
	}

	for (int i = 0; i < BUTTON_COUNT; i++)
	{
		Buttons[i].bHovered = PtInRect(&(Buttons[i].rect), *lpMousePointer);
	}

	return TRUE;
}

BOOL BtnClick(LPPOINT lpMousePointer)
{
	for (UINT i = 0; i < BUTTON_COUNT; i++)
	{
		if (Buttons[i].bHovered)
		{
			PostMessageW(WndGetMainWindowHandle(), WM_USER_BUTTON_CLICKED, (WPARAM)i, 0);
			break;
		}
	}
	return TRUE;
}

BOOL BtnRenderButtons(HDC hDestDC)
{
	HPEN hOldPen = SelectObject(hDestDC, GetStockObject(NULL_PEN));
	HBRUSH hOldBrush = SelectObject(hDestDC, hDefaultButtonBrush);
	HFONT hOldFont = SelectObject(hDestDC, hButtonFont);
	INT oldBkMode = SetBkMode(hDestDC, TRANSPARENT);

	for (UINT i = 0; i < BUTTON_COUNT; i++)
	{
		if (Buttons[i].bHovered)
		{
			SelectObject(hDestDC, hHoverButtonBrush);
		}
		else
		{
			SelectObject(hDestDC, hDefaultButtonBrush);
		}

		RECT rect = Buttons[i].rect;
		RoundRect(hDestDC, rect.left, rect.top, rect.right, rect.bottom, 20, 20);

		SIZE size;
		GetTextExtentPoint32W(hDestDC, Buttons[i].lpszText, lstrlenW(Buttons[i].lpszText), &size);

		INT x, y;
		x = (rect.right - rect.left - size.cx) / 2 + rect.left;
		y = (rect.bottom - rect.top - size.cy) / 2 + rect.top;

		TextOutW(hDestDC, x, y, Buttons[i].lpszText, lstrlenW(Buttons[i].lpszText));
	}

	SelectObject(hDestDC, hOldPen);
	SelectObject(hDestDC, hOldBrush);
	SelectObject(hDestDC, hOldFont);
	SetBkMode(hDestDC, oldBkMode);

	return TRUE;
}
