#ifndef TIMER_MESSAGE_H
#define TIMER_MESSAGE_H

#include <Windows.h>

/**
 * @brief ������ ���ν����� �ּҸ� ����ϴ�.
 */
WNDPROC MsgGetWindowProc(VOID);

/**
 * @brief �޽��� ������ �����մϴ�.
 * @return �޽��� ������ ��ȯ �ڵ带 ��ȯ�մϴ�. (msg.wParam)
 */
INT MsgStartMessageLoop(VOID);

#endif