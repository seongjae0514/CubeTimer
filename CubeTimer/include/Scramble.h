#ifndef TIMER_SCRAMBLE_H
#define TIMER_SCRAMBLE_H

#include <Windows.h>

/**
 * @brief 큐브의 타입을 나타냅니다.
 */
typedef enum tagCubeType {
    SC_CUBE_33 /** 33큐브 타입입니다. */
} CubeType;

/**
 * @brief 33큐브의 회전 기호를 나타냅니다.
 */
typedef enum tagScramble33 {
    INVALID_33, R_33, L_33, U_33, D_33, B_33, F_33
} Scramble33;

typedef struct tagScrambleStruct {
    INT  uType; /* 스크램블의 동작 타입입니다. (예: Scramble33) */
    UINT uCount; /* 스크램블의 동작 횟수입니다. */
    BOOL bReversed; /* 스크램블의 방향입니다. TRUE이면 역방향, FALSE이면 정방향을 나타냅니다. */
} ScrambleStruct;

/**
 * @brief Scramble 모듈을 초기화합니다.
 * @return 성공시 TRUE, 실패시 FALSE를 반환합니다.
 */
extern BOOL ScInitialize(VOID);

/**
 * @brief Scramble 모듈의 초기화를 취소합니다. 모든 정리 작업을 자동으로 수행합니다.
 */
extern VOID ScUninitialize(VOID);

/**
 * @brief 스크램블을 생성합니다.
 * @param cubeType 생성할 스크램블의 큐브 타입입니다.
 * @param length 스크램블의 길이입니다.
 * @return 성공시 TRUE, 실패시 FALSE를 반환합니다.
 */
extern BOOL ScGenerateScramble(CubeType cubeType, UINT length);

/**
 * @brief ScGenerateScramble() 로 생성된 스크램블을 얻습니다. 스크램블이 이미 있다면 ScDeleteScramble로 정리해야합니다.
 * @return 스크램블 주소를 반환합니다. Scramble 모듈의 주소를 반환하므로 편집할 수 없습니다.
 */
extern const ScrambleStruct* ScGetScramble(VOID);

/**
 * @brief 스크램블을 문자열로 변환하여 저장합니다.
 * @param lpBuffer 스크램블을 저장할 버퍼입니다.
 * @param bufferSize 스크램블을 저장할 버퍼의 크기입니다.
 * @return 성공시 TRUE, 실패시 FALSE를 반환합니다.
 */
extern BOOL ScScrambleToString(LPWSTR lpBuffer, SIZE_T bufferSize);

/**
 * @brief 생성된 스크램블을 삭제합니다.
 * @return 성공시 TRUE, 실패시 FALSE를 반환합니다.
 */
extern BOOL ScDeleteScramble(VOID);

#endif