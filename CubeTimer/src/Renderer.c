#include "Debug.h"
#include <Windows.h>
#include <strsafe.h>
#include "Timer.h"
#include "Window.h"
#include "Renderer.h"
#include "Record.h"
#include "Scramble.h"
#include "defs.h"

/* Global variables **************************************************************+*/

#define RECORD_POINT_X 20
#define RECORD_POINT_Y 20

HFONT TimerFont;
HFONT RecordFont;

INT   ScrollPx;

/* Private variables *************************************************************+*/

static BOOL RdpPaintTimer(HDC hDC)
{
    /* GDI 초기화 */

    HFONT    hOldFont     = SelectObject(hDC, TimerFont);
    INT      oldBkMode    = SetBkMode(hDC, TRANSPARENT);
    COLORREF oldTextColor;

    /* 현재 타이머 상태에 따라 텍스트 색상 선택 */

    switch (TmGetTimerStatus())
    {
        case TM_WAITING:
        {
            if (TmCanStart())
            {
                oldTextColor = SetTextColor(hDC, RGB(0, 255, 0));
            }
            else
            {
                oldTextColor = SetTextColor(hDC, RGB(255, 0, 0));
            }
            break;
        }
        default:
        {
            oldTextColor = SetTextColor(hDC, RGB(0, 0, 0));
        }
    }

    /* 그릴 문자열 생성 */

    WCHAR wBuffer[256];

    MillisecTime elapsed = TmGetElapsedTime();
    StringCchPrintfW(wBuffer, _countof(wBuffer),
                     L"%I64u.%03I64u",
                     elapsed / 1000, elapsed % 1000);

    /* 그려질 위치 얻기 */

    INT textX, textY;

    RECT windowRect;
    SIZE textSize;

    WndGetWindowRect(&windowRect);
    GetTextExtentPoint32W(hDC, wBuffer, lstrlenW(wBuffer), &textSize);

    textX = (windowRect.right - textSize.cx) / 2;
    textY = (windowRect.bottom - textSize.cy) / 2;

    /* hDC에 그리기 */

    TextOutW(hDC, textX, textY, wBuffer, lstrlenW(wBuffer));

    /* GDI 정리 및 반환 */

    SelectObject(hDC, hOldFont);
    SetBkMode(hDC, oldBkMode);
    SetTextColor(hDC, oldTextColor);

    return TRUE;
}

static BOOL RdpPaintRecords(HDC hDC)
{
    /* GDI 초기화 및 정보 얻기 */

    HFONT hOldFont  = SelectObject(hDC, RecordFont);
    INT   oldBkMode = SetBkMode(hDC, TRANSPARENT);
    UINT  uRecordCount = RcGetRecordCount();
    RECT  windowRect;
    WndGetWindowRect(&windowRect);

    /* 레코드 순회하면서 기록 그리기 */
    
    INT  x = 20, y = 20 + ScrollPx;

    for (UINT i = 0; i < uRecordCount; i++)
    {
        /* 그릴 문자열 생성 */

        const RecordStruct* record = RcGetRecordStructAddress(i);
        WCHAR wBuffer[256];
        StringCchPrintfW(
            wBuffer,
            _countof(wBuffer),
            record->bPlusTwo
            ? L"(%u) %llu.%03llu+ | %llu.%03llu | %llu.%03llu | %s" // 현재 기록에 +2가 되어있다면
            : L"(%u) %llu.%03llu | %llu.%03llu | %llu.%03llu | %s", // 아니라면
            i + 1,
            record->record / 1000, record->record % 1000,
            record->ao5    / 1000, record->ao5    % 1000,
            record->ao12   / 1000, record->ao12   % 1000,
            record->scramble
        );

        /* 그려질 위치 계산 */

        SIZE textSize;
        GetTextExtentPoint32W(hDC, wBuffer, lstrlenW(wBuffer), &textSize);

        if (y > windowRect.bottom)
        {
            break;
        }
        else if (y < 0 - textSize.cy)
        {
            goto cleanup;
        }

        /* 그리기 */

        TextOutW(hDC, 20, y, wBuffer, lstrlenW(wBuffer));

        /* y 증가 */

        cleanup:
        y += textSize.cy;
    }

    /* 정리 및 반환 */

    SelectObject(hDC, hOldFont);
    SetBkMode(hDC, oldBkMode);

    return TRUE;
}

/* Global functinos **************************************************************+*/

BOOL RdInitializeRenderer(INT timerSizePx, INT recordSizePx)
{
    TimerFont = CreateFontW(
        -timerSizePx, 0,
        0, 0,
        FW_BOLD,
        FALSE, FALSE, FALSE,
        DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,
        DEFAULT_PITCH,
        L"맑은 고딕"
    );

    RecordFont = CreateFontW(
        -recordSizePx, 0,
        0, 0,
        FW_MEDIUM,
        FALSE, FALSE, FALSE,
        DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,
        DEFAULT_PITCH,
        L"맑은 고딕"
    );

    if (!TimerFont)
    {
        return FALSE;
    }

    return TRUE;
}

BOOL RdUninitializeRenderer(VOID)
{
    DeleteObject(TimerFont);
    DeleteObject(RecordFont);

    TimerFont = NULL;
    RecordFont = NULL;

    return TRUE;
}

BOOL RdOnRender(HDC hDestDC)
{
    /* 리소스 준비 및 사용할 정보 받기 */

    RECT     windowRect;    WndGetWindowRect(&windowRect);
    INT      oldBkMode = SetBkMode(hDestDC, TRANSPARENT);
    COLORREF oldTextColor = SetTextColor(hDestDC, 0x00000000);
    HFONT    hOldFont = SelectObject(hDestDC, RecordFont);

    /* 타이머 그리기 */

    RdpPaintTimer(hDestDC);

    /* 기록 그리기 */

    RdpPaintRecords(hDestDC);

    /* 현재 스크램블 그리기 */

    {
        WCHAR wScrambleBuffer[256];
        ScScrambleToString(wScrambleBuffer, sizeof(wScrambleBuffer));

        SIZE textSize;
        GetTextExtentPoint32W(hDestDC, wScrambleBuffer, lstrlenW(wScrambleBuffer), &textSize);

        UINT X = (windowRect.right - textSize.cx) / 2;
        UINT Y = 20;

        TextOutW(hDestDC, X, Y, wScrambleBuffer, lstrlenW(wScrambleBuffer));
    }

    /* 정리 및 반환 */

    SelectObject(hDestDC, hOldFont);
    SetBkMode(hDestDC, oldBkMode);
    SetTextColor(hDestDC, oldTextColor);

    return TRUE;
}

BOOL RdScrollRecord(BOOL bForward, HWND hDestWindow)
{
    RECT windowRect;
    SIZE textSize;

    /* GDI 정보 얻기 */

    // 윈도우 크기
    GetWindowRect(hDestWindow, &windowRect);
    
    // 텍스트 크기
    HDC hDestWindowDC = GetDC(hDestWindow);
    HFONT hOldFont = SelectObject(hDestWindowDC, RecordFont);
    GetTextExtentPoint32W(hDestWindowDC, L"A", 1, &textSize);
    SelectObject(hDestWindowDC, hOldFont);
    ReleaseDC(hDestWindow, hDestWindowDC);
    
    /* 스크롤하고 가능한지 확인하고 되돌릴지 정하기 */

    // forward=TRUE 면 위로 스크롤, 아니면 아래로 스크롤

    if (bForward)
    {
        INT newScrollPx = ScrollPx - 20;
        INT y = RECORD_POINT_Y + newScrollPx + (textSize.cy * (INT)RcGetRecordCount());

        if (y <= 0)
        {
            // 스크롤 안함
            return FALSE;
        }
        else
        {
            // 스크롤 하기
            ScrollPx = newScrollPx;
            return TRUE;
        }
    }
    else
    {
        INT newScrollPx = ScrollPx + 20;
        INT y = RECORD_POINT_Y + newScrollPx + (textSize.cy);

        if (y >= windowRect.bottom)
        {
            // 스크롤 안함
            return FALSE;
        }
        else
        {
            // 스크롤 하기
            ScrollPx = newScrollPx;
            return TRUE;
        }
    }
}

BOOL RdInitializeScroll(VOID)
{
    ScrollPx = 0;
    return TRUE;
}