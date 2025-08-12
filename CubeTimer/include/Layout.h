#ifndef TIMER_LAYOUT_H
#define TIMER_LAYOUT_H

#include <Windows.h>
#include "Button.h"

#define DEFAULT_RECORD_Y 20
#define DEFAULT_RECORD_X 20

/**
 * @brief 모듈을 초기화합니다.
 * @return 성공하면 TRUE, 실패하면 FALSE를 반환합니다.
 */
BOOL LoInitialize(VOID);

/**
 * @brief 모듈 초기화를 해제합니다.
 * @return 성공하면 TRUE, 실패하면 FALSE를 반환합니다.
 */
BOOL LoUninitialize(VOID);

/**
 * @brief 화면에 레이아웃의 모든 내용을 그립니다.
 * @return 성공하면 TRUE, 실패하면 FALSE를 반환합니다.
 */
BOOL LoRenderAll(HDC hDestDC);

/**
 * @brief 윈도우의 크기가 변경되었음을 레이아웃 모듈에 알립니다.
 * @return 성공하면 TRUE, 실패하면 FALSE를 반환합니다.
 */
BOOL LoResetWindowSize(VOID);

/**
 * @brief 마우스의 위치가 이동했음을 레이아웃 모듈에 알립니다.
 * @return 성공하면 TRUE, 실패하면 FALSE를 반환합니다.
 */
BOOL LoResetMousePosition(LPPOINT lpMousePointer);

/**
 * @brief 마우스가 클릭되었음을 레이아웃 모듈에 알립니다.
 * @return 성공하면 TRUE, 실패하면 FALSE를 반환합니다.
 */
BOOL LoMouseClick(LPPOINT lpMousePointer);

/**
 * @brief 기록들이 그려질 위치를 스크롤합니다.
 * @return 성공하면 TRUE, 실패하면 FALSE를 반환합니다.
 */
BOOL LoScrollRecords(BOOL bUpScroll);

#endif