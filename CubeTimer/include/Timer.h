#ifndef TIMER_TIMER_H
#define TIMER_TIMER_H

#include <Windows.h>
#include "defs.h"

#define TIMER_MIN_WAIT_TIME 500

/**
 * @brief 현재 타이머의 상태에 대한 열거형입니다.
 */
typedef enum tagTimerStatus {
    TM_STOP, TM_WAITING, TM_RECORDING
} TimerStatus;

/**
 * @brief 타이머 모듈을 초기화합니다.
 * @return 성공하면 TRUE, 실패하면 FALSE를 반환합니다.
 */
extern BOOL TmInitializeTimer(VOID);

/**
 * @brief 타이머 모듈 초기화를 취소합니다.
 * @return 성공하면 TRUE, 실패하면 FALSE를 반환합니다.
 */
extern BOOL TmUninitializeTimer(VOID);

/**
 * @brief 트리거 함수입니다.
 *        첫 번째 호출하면 기다림이 시작되고
 *        TIMER_MIN_WAIT_TIME이 지난 후 한 번 더 호출하면 시간 측정이 시작됩니다.
 *        시간이 지나기 전 호출하면 측정이 취소됩니다.
 *        측정중에 호출하면 측정이 종료됩니다.
 * @return Trigger 후 타이머의 상태를 반환합니다.
 */
extern TimerStatus TmTrigger(VOID);

/**
 * @brief 타이머 상태가 TM_RECORDING이면 현재 측정한 시간을 반환하고, TM_STOP이면 직전 측정 시간을 반환합니다.
 */
extern MillisecTime TmGetElapsedTime(VOID);

/**
 * @brief 현재 타이머의 상태를 반환합니다.
 */
extern TimerStatus TmGetTimerStatus(VOID);

/**
 * @brief TM_WAITING일 때, 한 번 더 Trigger하면 측정을 시작할 수 있는지 확인하고 반환합니다.
 * @return 가능하면 TRUE, 불가능하면 FALSE를 반환합니다.
 */
extern BOOL TmCanStart(VOID);

#endif