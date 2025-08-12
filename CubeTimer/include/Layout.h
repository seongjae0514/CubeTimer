#ifndef TIMER_LAYOUT_H
#define TIMER_LAYOUT_H

#include <Windows.h>
#include "Button.h"

#define DEFAULT_RECORD_Y 20
#define DEFAULT_RECORD_X 20

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

/**
 * @brief �������� ũ�Ⱑ ����Ǿ����� ���̾ƿ� ��⿡ �˸��ϴ�.
 * @return �����ϸ� TRUE, �����ϸ� FALSE�� ��ȯ�մϴ�.
 */
BOOL LoResetWindowSize(VOID);

/**
 * @brief ���콺�� ��ġ�� �̵������� ���̾ƿ� ��⿡ �˸��ϴ�.
 * @return �����ϸ� TRUE, �����ϸ� FALSE�� ��ȯ�մϴ�.
 */
BOOL LoResetMousePosition(LPPOINT lpMousePointer);

/**
 * @brief ���콺�� Ŭ���Ǿ����� ���̾ƿ� ��⿡ �˸��ϴ�.
 * @return �����ϸ� TRUE, �����ϸ� FALSE�� ��ȯ�մϴ�.
 */
BOOL LoMouseClick(LPPOINT lpMousePointer);

/**
 * @brief ��ϵ��� �׷��� ��ġ�� ��ũ���մϴ�.
 * @return �����ϸ� TRUE, �����ϸ� FALSE�� ��ȯ�մϴ�.
 */
BOOL LoScrollRecords(BOOL bUpScroll);

#endif