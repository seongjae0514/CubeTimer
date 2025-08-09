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
    /* GDI �ʱ�ȭ */

    HFONT    hOldFont     = SelectObject(hDC, TimerFont);
    INT      oldBkMode    = SetBkMode(hDC, TRANSPARENT);
    COLORREF oldTextColor;

    /* ���� Ÿ�̸� ���¿� ���� �ؽ�Ʈ ���� ���� */

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

    /* �׸� ���ڿ� ���� */

    WCHAR wBuffer[256];

    MillisecTime elapsed = TmGetElapsedTime();
    StringCchPrintfW(wBuffer, _countof(wBuffer),
                     L"%I64u.%03I64u",
                     elapsed / 1000, elapsed % 1000);

    /* �׷��� ��ġ ��� */

    INT textX, textY;

    RECT windowRect;
    SIZE textSize;

    WndGetWindowRect(&windowRect);
    GetTextExtentPoint32W(hDC, wBuffer, lstrlenW(wBuffer), &textSize);

    textX = (windowRect.right - textSize.cx) / 2;
    textY = (windowRect.bottom - textSize.cy) / 2;

    /* hDC�� �׸��� */

    TextOutW(hDC, textX, textY, wBuffer, lstrlenW(wBuffer));

    /* GDI ���� �� ��ȯ */

    SelectObject(hDC, hOldFont);
    SetBkMode(hDC, oldBkMode);
    SetTextColor(hDC, oldTextColor);

    return TRUE;
}

static BOOL RdpPaintRecords(HDC hDC)
{
    /* GDI �ʱ�ȭ �� ���� ��� */

    HFONT hOldFont  = SelectObject(hDC, RecordFont);
    INT   oldBkMode = SetBkMode(hDC, TRANSPARENT);
    UINT  uRecordCount = RcGetRecordCount();
    RECT  windowRect;
    WndGetWindowRect(&windowRect);

    /* ���ڵ� ��ȸ�ϸ鼭 ��� �׸��� */
    
    INT  x = 20, y = 20 + ScrollPx;

    for (UINT i = 0; i < uRecordCount; i++)
    {
        /* �׸� ���ڿ� ���� */

        const RecordStruct* record = RcGetRecordStructAddress(i);
        WCHAR wBuffer[256];
        StringCchPrintfW(
            wBuffer,
            _countof(wBuffer),
            record->bPlusTwo
            ? L"(%u) %llu.%03llu+ | %llu.%03llu | %llu.%03llu | %s" // ���� ��Ͽ� +2�� �Ǿ��ִٸ�
            : L"(%u) %llu.%03llu | %llu.%03llu | %llu.%03llu | %s", // �ƴ϶��
            i + 1,
            record->record / 1000, record->record % 1000,
            record->ao5    / 1000, record->ao5    % 1000,
            record->ao12   / 1000, record->ao12   % 1000,
            record->scramble
        );

        /* �׷��� ��ġ ��� */

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

        /* �׸��� */

        TextOutW(hDC, 20, y, wBuffer, lstrlenW(wBuffer));

        /* y ���� */

        cleanup:
        y += textSize.cy;
    }

    /* ���� �� ��ȯ */

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
        L"���� ���"
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
        L"���� ���"
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
    /* ���ҽ� �غ� �� ����� ���� �ޱ� */

    RECT     windowRect;    WndGetWindowRect(&windowRect);
    INT      oldBkMode = SetBkMode(hDestDC, TRANSPARENT);
    COLORREF oldTextColor = SetTextColor(hDestDC, 0x00000000);
    HFONT    hOldFont = SelectObject(hDestDC, RecordFont);

    /* Ÿ�̸� �׸��� */

    RdpPaintTimer(hDestDC);

    /* ��� �׸��� */

    RdpPaintRecords(hDestDC);

    /* ���� ��ũ���� �׸��� */

    {
        WCHAR wScrambleBuffer[256];
        ScScrambleToString(wScrambleBuffer, sizeof(wScrambleBuffer));

        SIZE textSize;
        GetTextExtentPoint32W(hDestDC, wScrambleBuffer, lstrlenW(wScrambleBuffer), &textSize);

        UINT X = (windowRect.right - textSize.cx) / 2;
        UINT Y = 20;

        TextOutW(hDestDC, X, Y, wScrambleBuffer, lstrlenW(wScrambleBuffer));
    }

    /* ���� �� ��ȯ */

    SelectObject(hDestDC, hOldFont);
    SetBkMode(hDestDC, oldBkMode);
    SetTextColor(hDestDC, oldTextColor);

    return TRUE;
}

BOOL RdScrollRecord(BOOL bForward, HWND hDestWindow)
{
    RECT windowRect;
    SIZE textSize;

    /* GDI ���� ��� */

    // ������ ũ��
    GetWindowRect(hDestWindow, &windowRect);
    
    // �ؽ�Ʈ ũ��
    HDC hDestWindowDC = GetDC(hDestWindow);
    HFONT hOldFont = SelectObject(hDestWindowDC, RecordFont);
    GetTextExtentPoint32W(hDestWindowDC, L"A", 1, &textSize);
    SelectObject(hDestWindowDC, hOldFont);
    ReleaseDC(hDestWindow, hDestWindowDC);
    
    /* ��ũ���ϰ� �������� Ȯ���ϰ� �ǵ����� ���ϱ� */

    // forward=TRUE �� ���� ��ũ��, �ƴϸ� �Ʒ��� ��ũ��

    if (bForward)
    {
        INT newScrollPx = ScrollPx - 20;
        INT y = RECORD_POINT_Y + newScrollPx + (textSize.cy * (INT)RcGetRecordCount());

        if (y <= 0)
        {
            // ��ũ�� ����
            return FALSE;
        }
        else
        {
            // ��ũ�� �ϱ�
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
            // ��ũ�� ����
            return FALSE;
        }
        else
        {
            // ��ũ�� �ϱ�
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