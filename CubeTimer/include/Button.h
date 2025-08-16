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
 * @brief 버튼 모듈을 초기화합니다.
 * @return 성공하면 TRUE, 실패하면 FALSE를 반환합니다.
 */
BOOL BtnInitialize(VOID);

/**
 * @brief 버튼 모듈을 초기화를 취소합니다.
 * @return 성공하면 TRUE, 실패하면 FALSE를 반환합니다.
 */
BOOL BtnUninitialize(VOID);

/**
 * @brief 버튼 구역의 크기를 구해서 size 구조체에 저장합니다.
 * @return 성공하면 TRUE, 실패하면 FALSE를 반환합니다.
 */
BOOL BtnGetButtonDivisionSize(LPSIZE lpSize);

/**
 * @brief 버튼 구역의 위치를 x, y 로 재지정합니다.
 * @return 성공하면 TRUE, 실패하면 FALSE를 반환합니다.
 */
BOOL BtnResetButtonDivisionPosition(INT x, INT y);

/**
 * @brief 마우스의 위치를 업데이트합니다.
 * @return 성공하면 TRUE, 실패하면 FALSE를 반환합니다.
 */
BOOL BtnResetMousePosition(LPPOINT lpMousePointer);

/**
 * @brief 마우스가 클릭되었음을 전달하고, 만약 버튼이 클릭되었다면 메인 윈도우에 메시지를 보냅니다.
 * @return 성공하면 TRUE, 실패하면 FALSE를 반환합니다.
 */
BOOL BtnClick(LPPOINT lpMousePointer);

/**
 * @brief 버튼을 hDestDC에 그립니다.
 * @return 성공하면 TRUE, 실패하면 FALSE를 반환합니다.
 */
BOOL BtnRenderButtons(HDC hDestDC);

#endif // TIMER_BUTTON_H