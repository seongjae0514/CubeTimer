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
    HFONT    hOldFont = SelectObject(hDestDC, TimerFont);

    /* 타이머 그리기 */

    {
        /* 타이머 텍스트 설정 */

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

        /* 그릴 텍스트 문자열 생성 */

        MillisecTime time = TmCanStart() ? 0 : TmGetElapsedTime();
        WCHAR        buffer[256];

        wsprintfW(buffer, L"%d.%02d", time / 1000, time / 10 % 100);

        /* 텍스트가 그려질 위치 얻기 */

        INT textX, textY;

        SIZE textSize;
        GetTextExtentPoint32W(hDestDC, buffer, lstrlenW(buffer), &textSize);

        textX = (windowRect.right - textSize.cx) / 2;
        textY = (windowRect.bottom - textSize.cy) / 2;

        /* 텍스트 그리기 */

        TextOutW(hDestDC, textX, textY, buffer, lstrlenW(buffer));
    }

    /* 기록 그리기 */

    {
        /* 기록 폰트 설정 */

        SelectObject(hDestDC, RecordFont);
        SetTextColor(hDestDC, 0x00000000);

        /* 그리기 반복 시작 */

        BOOL bNode = RcGoToFirstRecordNode();
        UINT X = 20;
        UINT Y = 20;
        UINT idx = 0;

        while (bNode)
        {
            /* 기록 얻기 */

            const RecordStruct* currentRecord = RcGetCurrentNodeRecord();

            if (!currentRecord)
            {
                break;
            }

            /* 기록 문자열 생성 */

            WCHAR wBuffer[512] = { 0 };
            wsprintfW(wBuffer,
                      RcIsCurrentNodePlusTwo() ? L"(%d) %d.%02d+ | %d.%02d | %d.%02d | %s" : L"(%d) %d.%02d | %d.%02d | %d.%02d | %s",
                      idx + 1,
                      currentRecord->record / 1000, currentRecord->record / 10 % 100,
                      currentRecord->ao5 / 1000, currentRecord->ao5 / 10 % 100,
                      currentRecord->ao12 / 1000, currentRecord->ao12 / 10 % 100,
                      currentRecord->scramble);

            /* 기록 그리기 */

            TextOutW(hDestDC, X, Y, wBuffer, lstrlenW(wBuffer));

            /* Y 증가 */

            SIZE textSize;
            GetTextExtentPoint32W(hDestDC, wBuffer, lstrlenW(wBuffer), &textSize);

            Y += textSize.cy;

            bNode = RcGoToNextRecordNode(1);

            idx++;
        }
    }

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