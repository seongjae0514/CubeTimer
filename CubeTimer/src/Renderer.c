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

HFONT TimerFont;
HFONT RecordFont;

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
    /* GDI �ʱ�ȭ */

    HFONT    hOldFont = SelectObject(hDC, RecordFont);
    INT      oldBkMode = SetBkMode(hDC, TRANSPARENT);

    /* ���ڵ� ��ȸ�ϸ鼭 ��� �׸��� */
    
    UINT uRecordCount = RcGetRecordCount();
    UINT y = 20;

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

        /* �׸��� */

        TextOutW(hDC, 20, y, wBuffer, lstrlenW(wBuffer));

        /* y ���� */

        SIZE textSize;
        GetTextExtentPoint32W(hDC, wBuffer, lstrlenW(wBuffer), &textSize);

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