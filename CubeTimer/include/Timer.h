#ifndef TIMER_TIMER_H
#define TIMER_TIMER_H

#include <Windows.h>
#include "defs.h"

#define TIMER_MIN_WAIT_TIME 500

/**
 * @brief ���� Ÿ�̸��� ���¿� ���� �������Դϴ�.
 */
typedef enum tagTimerStatus {
    TM_STOP, TM_WAITING, TM_RECORDING
} TimerStatus;

/**
 * @brief Ÿ�̸� ����� �ʱ�ȭ�մϴ�.
 * @return �����ϸ� TRUE, �����ϸ� FALSE�� ��ȯ�մϴ�.
 */
extern BOOL TmInitializeTimer(VOID);

/**
 * @brief Ÿ�̸� ��� �ʱ�ȭ�� ����մϴ�.
 * @return �����ϸ� TRUE, �����ϸ� FALSE�� ��ȯ�մϴ�.
 */
extern BOOL TmUninitializeTimer(VOID);

/**
 * @brief Ʈ���� �Լ��Դϴ�.
 *        ù ��° ȣ���ϸ� ��ٸ��� ���۵ǰ�
 *        TIMER_MIN_WAIT_TIME�� ���� �� �� �� �� ȣ���ϸ� �ð� ������ ���۵˴ϴ�.
 *        �ð��� ������ �� ȣ���ϸ� ������ ��ҵ˴ϴ�.
 *        �����߿� ȣ���ϸ� ������ ����˴ϴ�.
 * @return Trigger �� Ÿ�̸��� ���¸� ��ȯ�մϴ�.
 */
extern TimerStatus TmTrigger(VOID);

/**
 * @brief Ÿ�̸� ���°� TM_RECORDING�̸� ���� ������ �ð��� ��ȯ�ϰ�, TM_STOP�̸� ���� ���� �ð��� ��ȯ�մϴ�.
 */
extern MillisecTime TmGetElapsedTime(VOID);

/**
 * @brief ���� Ÿ�̸��� ���¸� ��ȯ�մϴ�.
 */
extern TimerStatus TmGetTimerStatus(VOID);

/**
 * @brief TM_WAITING�� ��, �� �� �� Trigger�ϸ� ������ ������ �� �ִ��� Ȯ���ϰ� ��ȯ�մϴ�.
 * @return �����ϸ� TRUE, �Ұ����ϸ� FALSE�� ��ȯ�մϴ�.
 */
extern BOOL TmCanStart(VOID);

#endif