#ifndef TIMER_SCRAMBLE_H
#define TIMER_SCRAMBLE_H

#include <Windows.h>

/**
 * @brief ť���� Ÿ���� ��Ÿ���ϴ�.
 */
typedef enum tagCubeType {
    SC_CUBE_33 /** 33ť�� Ÿ���Դϴ�. */
} CubeType;

/**
 * @brief 33ť���� ȸ�� ��ȣ�� ��Ÿ���ϴ�.
 */
typedef enum tagScramble33 {
    INVALID_33, R_33, L_33, U_33, D_33, B_33, F_33
} Scramble33;

typedef struct tagScrambleStruct {
    INT  uType; /* ��ũ������ ���� Ÿ���Դϴ�. (��: Scramble33) */
    UINT uCount; /* ��ũ������ ���� Ƚ���Դϴ�. */
    BOOL bReversed; /* ��ũ������ �����Դϴ�. TRUE�̸� ������, FALSE�̸� �������� ��Ÿ���ϴ�. */
} ScrambleStruct;

/**
 * @brief Scramble ����� �ʱ�ȭ�մϴ�.
 * @return ������ TRUE, ���н� FALSE�� ��ȯ�մϴ�.
 */
extern BOOL ScInitialize(VOID);

/**
 * @brief Scramble ����� �ʱ�ȭ�� ����մϴ�. ��� ���� �۾��� �ڵ����� �����մϴ�.
 */
extern VOID ScUninitialize(VOID);

/**
 * @brief ��ũ������ �����մϴ�.
 * @param cubeType ������ ��ũ������ ť�� Ÿ���Դϴ�.
 * @param length ��ũ������ �����Դϴ�.
 * @return ������ TRUE, ���н� FALSE�� ��ȯ�մϴ�.
 */
extern BOOL ScGenerateScramble(CubeType cubeType, UINT length);

/**
 * @brief ScGenerateScramble() �� ������ ��ũ������ ����ϴ�. ��ũ������ �̹� �ִٸ� ScDeleteScramble�� �����ؾ��մϴ�.
 * @return ��ũ���� �ּҸ� ��ȯ�մϴ�. Scramble ����� �ּҸ� ��ȯ�ϹǷ� ������ �� �����ϴ�.
 */
extern const ScrambleStruct* ScGetScramble(VOID);

/**
 * @brief ��ũ������ ���ڿ��� ��ȯ�Ͽ� �����մϴ�.
 * @param lpBuffer ��ũ������ ������ �����Դϴ�.
 * @param bufferSize ��ũ������ ������ ������ ũ���Դϴ�.
 * @return ������ TRUE, ���н� FALSE�� ��ȯ�մϴ�.
 */
extern BOOL ScScrambleToString(LPWSTR lpBuffer, SIZE_T bufferSize);

/**
 * @brief ������ ��ũ������ �����մϴ�.
 * @return ������ TRUE, ���н� FALSE�� ��ȯ�մϴ�.
 */
extern BOOL ScDeleteScramble(VOID);

#endif