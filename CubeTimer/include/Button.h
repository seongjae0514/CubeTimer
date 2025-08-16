#ifndef TIMER_BUTTON_H
#define TIMER_BUTTON_H

#include <Windows.h>
#include "Defs.h"

#define BUTTON_ID_INIT          0
#define BUTTON_ID_PLUSTWO       1
#define BUTTON_ID_LOADRECORD    2
#define BUTTON_ID_SAVERECORD    3
#define BUTTON_ID_OPTION_CHANGE 4

#define BUTTON_COUNT            5

#define BUTTON_WIDTH   80
#define BUTTON_HEIGHT  30
#define BUTTON_PADDING 10

#define WM_USER_BUTTON_CLICKED WM_USER + 1

/**
 * @brief ��ư ����� �ʱ�ȭ�մϴ�.
 * @return �����ϸ� TRUE, �����ϸ� FALSE�� ��ȯ�մϴ�.
 */
BOOL BtnInitialize(VOID);

/**
 * @brief ��ư ����� �ʱ�ȭ�� ����մϴ�.
 * @return �����ϸ� TRUE, �����ϸ� FALSE�� ��ȯ�մϴ�.
 */
BOOL BtnUninitialize(VOID);

/**
 * @brief ��ư ������ ũ�⸦ ���ؼ� size ����ü�� �����մϴ�.
 * @return �����ϸ� TRUE, �����ϸ� FALSE�� ��ȯ�մϴ�.
 */
BOOL BtnGetButtonDivisionSize(LPSIZE lpSize);

/**
 * @brief ��ư ������ ��ġ�� x, y �� �������մϴ�.
 * @return �����ϸ� TRUE, �����ϸ� FALSE�� ��ȯ�մϴ�.
 */
BOOL BtnResetButtonDivisionPosition(INT x, INT y);

/**
 * @brief ���콺�� ��ġ�� ������Ʈ�մϴ�.
 * @return �����ϸ� TRUE, �����ϸ� FALSE�� ��ȯ�մϴ�.
 */
BOOL BtnResetMousePosition(LPPOINT lpMousePointer);

/**
 * @brief ���콺�� Ŭ���Ǿ����� �����ϰ�, ���� ��ư�� Ŭ���Ǿ��ٸ� ���� �����쿡 �޽����� �����ϴ�.
 * @return �����ϸ� TRUE, �����ϸ� FALSE�� ��ȯ�մϴ�.
 */
BOOL BtnClick(LPPOINT lpMousePointer);

/**
 * @brief ��ư�� hDestDC�� �׸��ϴ�.
 * @return �����ϸ� TRUE, �����ϸ� FALSE�� ��ȯ�մϴ�.
 */
BOOL BtnRenderButtons(HDC hDestDC);

#endif // TIMER_BUTTON_H