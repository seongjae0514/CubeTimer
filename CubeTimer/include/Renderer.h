#ifndef TIMER_RENDERER_H
#define TIMER_RENDERER_H

#include <Windows.h>

/**
 * @brief �������� �ʱ�ȭ�մϴ�.
 * @return �����ϸ� TRUE, �����ϸ� FALSE�� ��ȯ�մϴ�.
 */
extern BOOL RdInitializeRenderer(INT timerSizePx, INT recordSizePx);

/**
 * @brief ������ �ʱ�ȭ�� ����մϴ�.
 * @return �����ϸ� TRUE, �����ϸ� FALSE�� ��ȯ�մϴ�.
 */
extern BOOL RdUninitializeRenderer(VOID);

/**
 * @brief ������ �մϴ�.
 * @param hDestDC ������ ������ ����̽� ���ؽ�Ʈ�Դϴ�.
 * @return �����ϸ� TRUE, �����ϸ� FALSE�� ��ȯ�մϴ�.
 */
extern BOOL RdOnRender(HDC hDestDC);

#endif