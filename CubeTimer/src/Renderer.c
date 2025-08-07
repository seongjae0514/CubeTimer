#include "Debug.h"
#include <Windows.h>
#include "Timer.h"
#include "Window.h"
#include "Renderer.h"
#include "Record.h"
#include "Scramble.h"
#include "defs.h"

/* Global variables **************************************************************+*/

HFONT TimerFont;
HFONT RecordFont;

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
    HFONT    hOldFont = SelectObject(hDestDC, TimerFont);

    /* Ÿ�̸� �׸��� */

    {
        /* Ÿ�̸� �ؽ�Ʈ ���� */

        SelectObject(hDestDC, TimerFont);

        TimerStatus timerStatus = TmGetTimerStatus();
        switch (timerStatus)
        {
            case TM_RECORDING:
            case TM_STOP:
            {
                break;
            }
            case TM_WAITING:
            {
                SetTextColor(hDestDC, RGB(255, 0, 0));
                if (TmCanStart())
                {
                    SetTextColor(hDestDC, RGB(0, 255, 0));
                }
                break;
            }
        }

        /* �׸� �ؽ�Ʈ ���ڿ� ���� */

        MillisecTime time = TmCanStart() ? 0 : TmGetElapsedTime();
        WCHAR        buffer[256];

        wsprintfW(buffer, L"%d.%02d", time / 1000, time / 10 % 100);

        /* �ؽ�Ʈ�� �׷��� ��ġ ��� */

        INT textX, textY;

        SIZE textSize;
        GetTextExtentPoint32W(hDestDC, buffer, lstrlenW(buffer), &textSize);

        textX = (windowRect.right - textSize.cx) / 2;
        textY = (windowRect.bottom - textSize.cy) / 2;

        /* �ؽ�Ʈ �׸��� */

        TextOutW(hDestDC, textX, textY, buffer, lstrlenW(buffer));
    }

    /* ��� �׸��� */

    {
        /* ��� ��Ʈ ���� */

        SelectObject(hDestDC, RecordFont);
        SetTextColor(hDestDC, 0x00000000);

        /* �׸��� �ݺ� ���� */

        BOOL bNode = RcGoToFirstRecordNode();
        UINT X = 20;
        UINT Y = 20;
        UINT idx = 0;

        while (bNode)
        {
            /* ��� ��� */

            const RecordStruct* currentRecord = RcGetCurrentNodeRecord();

            if (!currentRecord)
            {
                break;
            }

            /* ��� ���ڿ� ���� */

            WCHAR wBuffer[512] = { 0 };
            wsprintfW(wBuffer,
                      RcIsCurrentNodePlusTwo() ? L"(%d) %d.%02d+ | %d.%02d | %d.%02d | %s" : L"(%d) %d.%02d | %d.%02d | %d.%02d | %s",
                      idx + 1,
                      currentRecord->record / 1000, currentRecord->record / 10 % 100,
                      currentRecord->ao5 / 1000, currentRecord->ao5 / 10 % 100,
                      currentRecord->ao12 / 1000, currentRecord->ao12 / 10 % 100,
                      currentRecord->scramble);

            /* ��� �׸��� */

            TextOutW(hDestDC, X, Y, wBuffer, lstrlenW(wBuffer));

            /* Y ���� */

            SIZE textSize;
            GetTextExtentPoint32W(hDestDC, wBuffer, lstrlenW(wBuffer), &textSize);

            Y += textSize.cy;

            bNode = RcGoToNextRecordNode(1);

            idx++;
        }
    }

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