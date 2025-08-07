#ifndef TIMER_BUTTON_H
#define TIMER_BUTTON_H

#include <Windows.h>
#include "Defs.h"

#define BUTTON_ID_INIT     1001
#define BUTTON_ID_PLUSTWO  1002

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