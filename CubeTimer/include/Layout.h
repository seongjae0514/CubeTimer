#ifndef TIMER_LAYOUT_H
#define TIMER_LAYOUT_H

#include <Windows.h>

/**
 * @brief ����� �ʱ�ȭ�մϴ�.
 * @return �����ϸ� TRUE, �����ϸ� FALSE�� ��ȯ�մϴ�.
 */
BOOL LoInitialize(VOID);

/**
 * @brief ��� �ʱ�ȭ�� �����մϴ�.
 * @return �����ϸ� TRUE, �����ϸ� FALSE�� ��ȯ�մϴ�.
 */
BOOL LoUninitialize(VOID);

/**
 * @brief ȭ�鿡 ���̾ƿ��� ��� ������ �׸��ϴ�.
 * @return �����ϸ� TRUE, �����ϸ� FALSE�� ��ȯ�մϴ�.
 */
BOOL LoRenderAll(HDC hDestDC);

#endif