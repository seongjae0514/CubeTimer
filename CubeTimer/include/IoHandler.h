#ifndef TIMER_IO_H
#define TIMER_IO_H

#include <Windows.h>

/**
 * @brief IoHandler 모듈을 초기화합니다.
 * @return 성공하면 TRUE, 실패하면 FALSE를 반환합니다.
 */
extern BOOL IoInitializeIoHandler(VOID);

/**
 * @brief IoHandler 모듈 초기화를 취소합니다. 리소스를 정리합니다.
 * @return 성공하면 TRUE, 실패하면 FALSE를 반환합니다.
 */
extern BOOL IoUninitializeIoHandler(VOID);

/**
 * @brief 스페이스 키 다운 이벤트를 처리합니다.
 * @return 성공하면 TRUE, 실패하면 FALSE를 반환합니다.
 */
extern BOOL IoHandleSpaceKeyDown(VOID);

/**
 * @brief 스페이스 키 업 이벤트를 처리합니다.
 * @return 성공하면 TRUE, 실패하면 FALSE를 반환합니다.
 */
extern BOOL IoHandleSpaceKeyUp(VOID);

/**
 * @brief 버튼 눌림 이벤트를 처리합니다.
 * @return 성공하면 TRUE, 실패하면 FALSE를 반환합니다.
 */
extern BOOL IoHandleButtonPress(WPARAM wParam);

#endif