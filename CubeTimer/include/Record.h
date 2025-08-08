#ifndef TIMER_RECORD_H
#define TIMER_RECORD_H

#include <Windows.h>
#include "Defs.h"

#define RECORD_FILE_ERROR_INVALID_FILE_TYPE (-1)
#define RECORD_ERROR_ALLOCATION_FAILED      (-2)
#define RECORD_ERROR_ADD_RECORD_FAILED      (-3)

/**
 * @brief 기록을 담는 구조체입니다.
 */
typedef struct tagRecordStruct {
    LPWSTR       scramble;   // 스크램블
    MillisecTime record;     // 기록
    MillisecTime ao5;        // 5회 절단평균
    MillisecTime ao12;       // 12회 절단평균
    BOOL         bPlusTwo;   // +2 되어있는지 여부
} RecordStruct;

/**
 * @brief Record 모듈을 초기화합니다.
 * @return 성공하면 TRUE, 실패하면 FALSE를 반환합니다.
 */
extern BOOL RcInitialize(VOID);

/**
 * @brief Record 모듈 초기화를 취소합니다.
 * @return 성공하면 TRUE, 실패하면 FALSE를 반환합니다.
 */
extern BOOL RcUninitialize(VOID);

/**
 * @brief 기록을 추가합니다.
 * @return 성공하면 TRUE, 실패하면 FALSE를 반환합니다.
 */
extern BOOL RcAddRecord(MillisecTime record, LPCWSTR lpScramble);

/**
 * @brief 해당 인덱스의 기록에 +2를 토글합니다.
 * @return 성공하면 TRUE, 실패하면 FALSE를 반환합니다.
 */
extern BOOL RcTogglePlusTwo(UINT uIndex);

/**
 * @brief 해당 인덱스의 기록 구조체의 포인터를 반환합니다.
 * @return 기록 구조체의 주소입니다.
 */
extern const RecordStruct* RcGetRecordStructAddress(UINT uIndex);

/**
 * @brief 레코드 테이블의 레코드 갯수를 반환합니다.
 */
extern UINT RcGetRecordCount(VOID);

/**
 * @brief 레코드 파일을 lpszFilePath에 저장합니다.
 * @return 성공하면 0, 실패하면 해당하는 오류 코드를 반환합니다.
 */
extern INT RcLoadRecordFromFile(LPCWSTR lpszFilePath);

/**
 * @brief 레코드를 lpszFilePath에서 불러옵니다.
 * @return 성공하면 0, 실패하면 해당하는 오류 코드를 반환합니다.
 */
extern INT RcSaveRecordToFile(LPCWSTR lpszFilePath);

#endif