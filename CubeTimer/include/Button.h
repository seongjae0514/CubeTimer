#ifndef TIMER_BUTTON_H
#define TIMER_BUTTON_H

#include <Windows.h>
#include "Defs.h"

/**
 * @brief 버튼 모듈을 초기화합니다.
 * @return 성공하면 TRUE, 실패하면 FALSE를 반환합니다.
 */
BOOL BtnInitializeButton(VOID);

/**
 * @brief 버튼 모듈을 초기화를 취소합니다.
 * @return 성공하면 TRUE, 실패하면 FALSE를 반환합니다.
 */
BOOL BtnUninitializeButton(VOID);

/**
 * @brief 버튼 위치를 재지정합니다.
 * @return 성공하면 TRUE, 실패하면 FALSE를 반환합니다.
 */
BOOL BtnResetButtonPos(VOID);

#endif // TIMER_BUTTON_H