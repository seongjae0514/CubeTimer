#ifndef TIMER_LAYOUT_H
#define TIMER_LAYOUT_H

#include <Windows.h>

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

#endif