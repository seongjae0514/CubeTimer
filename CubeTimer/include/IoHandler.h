#ifndef TIMER_IO_H
#define TIMER_IO_H

#include <Windows.h>

/**
 * @brief IoHandler ����� �ʱ�ȭ�մϴ�.
 * @return �����ϸ� TRUE, �����ϸ� FALSE�� ��ȯ�մϴ�.
 */
extern BOOL IoInitializeIoHandler(VOID);

/**
 * @brief IoHandler ��� �ʱ�ȭ�� ����մϴ�. ���ҽ��� �����մϴ�.
 * @return �����ϸ� TRUE, �����ϸ� FALSE�� ��ȯ�մϴ�.
 */
extern BOOL IoUninitializeIoHandler(VOID);

/**
 * @brief �����̽� Ű �ٿ� �̺�Ʈ�� ó���մϴ�.
 * @return �����ϸ� TRUE, �����ϸ� FALSE�� ��ȯ�մϴ�.
 */
extern BOOL IoHandleSpaceKeyDown(VOID);

/**
 * @brief �����̽� Ű �� �̺�Ʈ�� ó���մϴ�.
 * @return �����ϸ� TRUE, �����ϸ� FALSE�� ��ȯ�մϴ�.
 */
extern BOOL IoHandleSpaceKeyUp(VOID);

/**
 * @brief ��ư ���� �̺�Ʈ�� ó���մϴ�.
 * @return �����ϸ� TRUE, �����ϸ� FALSE�� ��ȯ�մϴ�.
 */
extern BOOL IoHandleButtonPress(WPARAM wParam);

#endif