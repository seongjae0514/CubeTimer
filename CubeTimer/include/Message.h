#ifndef TIMER_MESSAGE_H
#define TIMER_MESSAGE_H

#include <Windows.h>

/**
 * @brief 윈도우 프로시져의 주소를 얻습니다.
 */
WNDPROC MsgGetWindowProc(VOID);

/**
 * @brief 메시지 루프를 시작합니다.
 * @return 메시지 루프의 반환 코드를 반환합니다. (msg.wParam)
 */
INT MsgStartMessageLoop(VOID);

#endif