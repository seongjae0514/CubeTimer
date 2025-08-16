#include "Debug.h"

#include <Windows.h>
#include <strsafe.h>

#include "Layout.h"

#include "Defs.h"
#include "Timer.h"
#include "Record.h"
#include "Window.h"
#include "Scramble.h"
#include "Button.h"
#include "Option.h"

/* Varialbles ************************************************************/

static HFONT TimerFontHandle;
static HFONT RecordFontHandle;
static HFONT ScrambleFontHandle;

static INT   RecordScrollPx;

/* Private functions *****************************************************/

static inline BOOL LopPaintTimer(HDC hDestDC, COLORREF textColor, INT textX, INT textY, MillisecTime time)
{
	COLORREF oldTextColor = SetTextColor(hDestDC, textColor);
	HFONT    hOldFont = SelectObject(hDestDC, TimerFontHandle);

	WCHAR wBuffer[256];
	StringCchPrintfW(
		wBuffer, _countof(wBuffer),
	    L"%llu.%03llu",
		time / 1000, time % 1000
	);

	BOOL ret = TextOutW(hDestDC, textX, textY, wBuffer, lstrlenW(wBuffer));

	SelectObject(hDestDC, hOldFont);
	SetTextColor(hDestDC, oldTextColor);

	return ret;
}

static inline BOOL LopPaintScramble(HDC hDestDC, INT textX, INT textY, LPCWSTR lpszScramble)
{
	HFONT hOldFont = SelectObject(hDestDC, ScrambleFontHandle);
	BOOL ret = TextOutW(hDestDC, textX, textY, lpszScramble, lstrlenW(lpszScramble));
	SelectObject(hDestDC, hOldFont);
	return ret;
}

static inline BOOL LopPaintRecords(HDC hDestDC, INT startX, INT startY)
{
	HFONT hOldFont = SelectObject(hDestDC, RecordFontHandle);

	INT x = startX, y = startY;
	UINT recordsCount = RcGetRecordCount();
	RECT windowRect;

	WndGetWindowRect(&windowRect);

	for (UINT i = 0; i < recordsCount; i++)
	{
		if (y >= windowRect.bottom)
		{
			break;
		}

		WCHAR wBuffer[256];
		const RecordStruct* record = RcGetRecordStructAddress(i);

		StringCchPrintfW(
			wBuffer, _countof(wBuffer),
			L"(%u) %llu.%03llu | %llu.%03llu | %llu.%03llu | %s",
			i + 1,
			record->record / 1000, record->record % 1000,
			record->ao5 / 1000, record->ao5 % 1000,
			record->ao12 / 1000, record->ao12 % 1000,
			record->scramble
		);

		TextOutW(hDestDC, x, y, wBuffer, lstrlenW(wBuffer));

		SIZE size;
		GetTextExtentPoint32W(hDestDC, wBuffer, lstrlenW(wBuffer), &size);

		y += size.cy;
	}

	SelectObject(hDestDC, hOldFont);

	return TRUE;
}

static inline BOOL LopGetTimerSize(HDC hDestDC, LPSIZE lpSize, MillisecTime time)
{
	HFONT hOldFont = SelectObject(hDestDC, TimerFontHandle);

	WCHAR wBuffer[256];
	StringCchPrintfW(
		wBuffer,
		_countof(wBuffer),
		L"%llu.%03llu",
		time / 1000, time % 1000
	);

	BOOL ret = GetTextExtentPoint32W(hDestDC, wBuffer, lstrlenW(wBuffer), lpSize);

	SelectObject(hDestDC, hOldFont);
	return ret;
}

static inline BOOL LopGetScrambleSize(HDC hDestDC, LPSIZE lpSize, LPCWSTR lpszScramble)
{
	HFONT hOldFont = SelectObject(hDestDC, ScrambleFontHandle);
	BOOL ret = GetTextExtentPoint32W(hDestDC, lpszScramble, lstrlenW(lpszScramble), lpSize);
	SelectObject(hDestDC, hOldFont);
	return TRUE;
}

static inline BOOL LopGetRecordsRect(HDC hDestDC, INT x, INT y, LPRECT lpRect)
{
	HFONT hOldFont = SelectObject(hDestDC, RecordFontHandle);

	/* 인자 유효성 검사 */

	if (!hDestDC || !lpRect)
	{
		return FALSE;
	}

	/* 지역변수 */
	
	UINT recordCount = RcGetRecordCount();
	const RecordStruct* record = RcGetRecordStructAddress(0);
	WCHAR wBuffer[256];
	SIZE  siz;

	if (!recordCount)
	{
		return FALSE;
	}


	/* 첫 번째 기록 먼저 계산 */

	StringCchPrintfW(
		wBuffer, _countof(wBuffer),
		L"(1) %llu.%02llu | %llu.%02llu | %llu.%02llu | %s",
		record->record / 1000, record->record % 1000,
		record->ao5 / 1000, record->ao5 % 1000,
		record->ao12 / 1000, record->ao12 % 1000,
		record->scramble
	);

	GetTextExtentPoint32W(hDestDC, wBuffer, lstrlenW(wBuffer), &siz);

	lpRect->left = x;
	lpRect->top = y;
	lpRect->right = siz.cx + x;
	lpRect->bottom = siz.cy * recordCount + y;

	SelectObject(hDestDC, RecordFontHandle);

	return TRUE;
}

BOOL LopIsRectContact(LPRECT lpRect1, LPRECT lpRect2)
{
	return !(lpRect1->right  <= lpRect2->left  ||
			 lpRect1->left   >= lpRect2->right ||
			 lpRect1->bottom <= lpRect2->top   ||
			 lpRect1->top    >= lpRect2->bottom);
}

/* Global functions ******************************************************/

BOOL LoInitialize(VOID)
{
	UINT_PTR value;

	OptGetKeyValue(OPTION_KEY_TIMER_SIZE, &value);
	TimerFontHandle = CreateFontW(
		(INT)value, 0,
		0, 0,
		FW_BOLD,
		FALSE, FALSE, FALSE,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH,
		L"맑은 고딕"
	);

	ScrambleFontHandle = CreateFontW(
		21, 0,
		0, 0,
		FW_BOLD,
		FALSE, FALSE, FALSE,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH,
		L"맑은 고딕"
	);

	OptGetKeyValue(OPTION_KEY_RECORD_SIZE, &value);
	RecordFontHandle = CreateFontW(
		(INT)value, 0,
		0, 0,
		FW_NORMAL,
		FALSE, FALSE, FALSE,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH,
		L"맑은 고딕"
	);

	if (!TimerFontHandle || !ScrambleFontHandle || !RecordFontHandle)
	{
		return FALSE;
	}

	RecordScrollPx = 0;

	BtnInitialize();

	return TRUE;
}

BOOL LoUninitialize(VOID)
{
	DeleteObject(TimerFontHandle);
	DeleteObject(ScrambleFontHandle);
	DeleteObject(RecordFontHandle);

	BtnUninitialize();
	
	return TRUE;
}

BOOL LoRenderAll(HDC hDestDC)
{
	/* 정보 얻기 */

	RECT         windowRect;
	MillisecTime time;
	WCHAR        wScramble[256];

	time = TmGetElapsedTime();
	WndGetWindowRect(&windowRect);
	ScScrambleToString(wScramble, _countof(wScramble));

	/* 기록들 */

	LopPaintRecords(hDestDC, DEFAULT_RECORD_X, DEFAULT_RECORD_Y + RecordScrollPx);

	/* 타이머, 스크램블, 버튼 */

	INT  timerX,    timerY;
	INT  scrambleX, scrambleY;
	INT  buttonX,   buttonY;
	SIZE timerSize, scrambleSize, buttonSize;

	// 정보 얻기

	LopGetTimerSize(hDestDC, &timerSize, time);
	LopGetScrambleSize(hDestDC, &scrambleSize, wScramble);
	BtnGetButtonDivisionSize(&buttonSize);

	// Width 가로 정렬

	timerX = (windowRect.right - timerSize.cx) / 2;
	scrambleX = (windowRect.right - scrambleSize.cx) / 2;
	buttonX = (windowRect.right - buttonSize.cx) / 2;

	// Height 가로 정렬

	timerY = (windowRect.bottom - (timerSize.cy + scrambleSize.cy + buttonSize.cy + 10))    / 2;
	scrambleY = (windowRect.bottom - (timerSize.cy + scrambleSize.cy + buttonSize.cy + 10)) / 2 + timerSize.cy;
	buttonY = (windowRect.bottom - (timerSize.cy + scrambleSize.cy + buttonSize.cy + 10)) / 2 + timerSize.cy + scrambleSize.cy + 10;

	// 타이머 상태에 따라 타이머 색 정하기

	COLORREF timerColor;

	if (TmGetTimerStatus() == TM_WAITING)
	{
		if (TmCanStart())
		{
			timerColor = RGB(255, 0, 0);
		}
		else
		{
			timerColor = RGB(0, 255, 0);
		}
	}
	else
	{
		timerColor = RGB(0, 0, 0);
	}

	// 타이머, 스크램블, 버튼 그리기

	LopPaintTimer(hDestDC, timerColor, timerX, timerY, time);
	LopPaintScramble(hDestDC, scrambleX, scrambleY, wScramble);

	BtnResetButtonDivisionPosition(buttonX, buttonY);
	BtnRenderButtons(hDestDC);

	return TRUE;
}

BOOL LoResetWindowSize(VOID)
{
	return TRUE;
}

BOOL LoResetMousePosition(LPPOINT lpMousePointer)
{
	return BtnResetMousePosition(lpMousePointer);
}

BOOL LoMouseClick(LPPOINT lpMousePointer)
{
	return BtnClick(lpMousePointer);
}

BOOL LoScrollRecords(BOOL bUpScroll)
{
	if (!RcGetRecordCount())
	{
		return FALSE;
	}

	RECT recordRect;
	RECT windowRect;
	HDC  hWndDC;
		
	hWndDC = GetDC(WndGetMainWindowHandle());
	LopGetRecordsRect(hWndDC, DEFAULT_RECORD_X, DEFAULT_RECORD_Y + RecordScrollPx + (bUpScroll ? 20 : -20), &recordRect);
	ReleaseDC(WndGetMainWindowHandle(), hWndDC);
	
	WndGetWindowRect(&windowRect);

	if (LopIsRectContact(&recordRect, &windowRect))
	{
		RecordScrollPx += bUpScroll ? 20 : -20;
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}