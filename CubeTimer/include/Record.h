#ifndef TIMER_RECORD_H
#define TIMER_RECORD_H

#include <Windows.h>
#include "Defs.h"

/* Defines */

/**
 * @brief 기록을 담는 구조체입니다.
 */
typedef struct tagRecordStruct {
    LPWSTR       scramble;
    MillisecTime record;
    MillisecTime ao5;
    MillisecTime ao12;
} RecordStruct;

/* Initialize ***************************************************************/

/**
 * @brief Record 모듈을 초기화합니다.
 * @return 성공하면 TRUE, 실패하면 FALSE를 반환합니다.
 */
extern BOOL RcInitializeRecordTable(VOID);

/**
 * @brief Record 모듈 초기화를 취소합니다.
 * @return 성공하면 TRUE, 실패하면 FALSE를 반환합니다.
 */
extern BOOL RcUninitializeRecordTable(VOID);

/* Node Control *************************************************************/

/**
 * @brief 기록을 추가합니다.
 * @param record 추가할 기록입니다.
 * @return 성공하면 TRUE, 실패하면 FALSE를 반환합니다.
 */
extern BOOL RcAddRecord(MillisecTime record, LPCWSTR lpszScramble);

/**
 * @brief 다음 노드로 이동합니다.
 * @param index 이동할 노드의 갯수입니다.
 * @return 성공하면 TRUE, 실패하면 FALSE를 반환합니다.
 */
extern BOOL RcGoToNextRecordNode(UINT index);

/**
 * @brief 첫 번째 노드로 이동합니다.
 * @return 성공하면 TRUE, 실패하면 FALSE를 반환합니다.
 */
extern BOOL RcGoToFirstRecordNode(VOID);

/**
 * @brief 현재 노드의 기록을 반환합니다.
 * @return 기록 구조체에 대한 포인터를 반환합니다. 모듈 내의 스태틱 변수의 주소를 반환합니다.
 */
extern const RecordStruct* RcGetCurrentNodeRecord(VOID);

/**
 * @brief 현재 노드에 +2를 합니다.
 * @return 성공하면 TRUE, 실패하면 FALSE를 반환합니다.
 */
extern BOOL RcPlusTwoCurrentNodeRecord(VOID);

/* Ao and Best record *******************************************************/

/**
 * @brief 최고 기록을 반환합니다.
 * @return 기록을 반환합니다.
 */
extern MillisecTime RcGetBestRecord(VOID);

/**
 * @brief 현재 노드가 +2 되어있는지 여부를 반환합니다.
 */
extern BOOL RcIsCurrentNodePlusTwo(VOID);

#endif