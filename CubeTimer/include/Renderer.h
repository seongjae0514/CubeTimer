#ifndef TIMER_RENDERER_H
#define TIMER_RENDERER_H

#include <Windows.h>

/**
 * @brief 렌더러를 초기화합니다.
 * @return 성공하면 TRUE, 실패하면 FALSE를 반환합니다.
 */
extern BOOL RdInitializeRenderer(INT timerSizePx, INT recordSizePx);

/**
 * @brief 렌더러 초기화를 취소합니다.
 * @return 성공하면 TRUE, 실패하면 FALSE를 반환합니다.
 */
extern BOOL RdUninitializeRenderer(VOID);

/**
 * @brief 렌더를 합니다.
 * @param hDestDC 렌더할 목적지 디바이스 컨텍스트입니다.
 * @return 성공하면 TRUE, 실패하면 FALSE를 반환합니다.
 */
extern BOOL RdOnRender(HDC hDestDC);

#endif