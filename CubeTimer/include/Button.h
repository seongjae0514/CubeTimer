#ifndef TIMER_BUTTON_H
#define TIMER_BUTTON_H

#include <Windows.h>
#include "Defs.h"

/**
 * @brief ��ư ����� �ʱ�ȭ�մϴ�.
 * @return �����ϸ� TRUE, �����ϸ� FALSE�� ��ȯ�մϴ�.
 */
BOOL BtnInitializeButton(VOID);

/**
 * @brief ��ư ����� �ʱ�ȭ�� ����մϴ�.
 * @return �����ϸ� TRUE, �����ϸ� FALSE�� ��ȯ�մϴ�.
 */
BOOL BtnUninitializeButton(VOID);

/**
 * @brief ��ư ��ġ�� �������մϴ�.
 * @return �����ϸ� TRUE, �����ϸ� FALSE�� ��ȯ�մϴ�.
 */
BOOL BtnResetButtonPos(VOID);

#endif // TIMER_BUTTON_H