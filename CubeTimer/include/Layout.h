#ifndef TIMER_LAYOUT_H
#define TIMER_LAYOUT_H

#include <Windows.h>
#include "Button.h"

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

#endif