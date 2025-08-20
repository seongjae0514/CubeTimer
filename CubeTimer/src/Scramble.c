#include "Debug.h"

#include <Windows.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "Scramble.h"

/* Global variables *************************************************************/

static ScrambleStruct* Scramble;
static UINT            ScrambleLength;
static CubeType        ScrambleCubeType;

/* Private functinos ************************************************************/

BOOL ScpScrambleToString33(LPWSTR lpBuffer, SIZE_T bufferSize)
{
    /* 지역변수 선언 */

    SIZE_T bufferIndex = 0;          // 버퍼 크기 오버플로우 방지용 인덱스 카운트

    /* 버퍼와 스크램블 배열 순회하면서 저장 */

    for (UINT i = 0; i < ScrambleLength; i++)
    {
        /* 기호 쓰기 */

        if (bufferIndex >= bufferSize)
        {
            lpBuffer[bufferIndex - 1] = 0;
            break;
        }

        switch (Scramble[i].uType)
        {
        case R_33:
            lpBuffer[bufferIndex] = L'R';
            break;
        case L_33:
            lpBuffer[bufferIndex] = L'L';
            break;
        case U_33:
            lpBuffer[bufferIndex] = L'U';
            break;
        case D_33:
            lpBuffer[bufferIndex] = L'D';
            break;
        case B_33:
            lpBuffer[bufferIndex] = L'B';
            break;
        case F_33:
            lpBuffer[bufferIndex] = L'F';
            break;
        }

        bufferIndex++;

        /* 숫자 (횟수) 쓰기 */

        if (bufferIndex >= bufferSize)
        {
            lpBuffer[bufferIndex - 1] = 0;
            break;
        }

        if (Scramble[i].uCount != 1)
        {
            lpBuffer[bufferIndex] = Scramble[i].uCount + L'0';
            bufferIndex++;
        }

        /* 방향 쓰기 */

        if (bufferIndex >= bufferSize)
        {
            break;
        }

        if (Scramble[i].bReversed)
        {
            lpBuffer[bufferIndex] = L'\'';
            bufferIndex++;
        }

        /* 공백문자 쓰기 (스크램블 기호마다 공백으로 구분해야함.) */

        if (bufferIndex >= bufferSize)
        {
            lpBuffer[bufferIndex - 1] = 0;
            break;
        }

        if (i + 1 != ScrambleLength) // 마지막 기호였다면 공백을 쓸 필요가 없음
        {
            lpBuffer[bufferIndex] = L' ';
            bufferIndex++;
        }
        else
        {
            lpBuffer[bufferIndex] = L'\0';
            bufferIndex++;
        }
    }

    return TRUE;
}

/* Global functinos *************************************************************/

BOOL ScInitialize(VOID)
{
    srand((unsigned int)time(NULL));
    Scramble = NULL;
    return TRUE;
}

VOID ScUninitialize(VOID)
{
    ScDeleteScramble();
}

BOOL ScGenerateScramble(CubeType cubeType, UINT length)
{
    Scramble = malloc(sizeof(ScrambleStruct) * length);
    ScrambleLength = length;
    ScrambleCubeType = cubeType;

    if (!Scramble)
    {
        return FALSE;
    }

    switch (cubeType)
    {
    case SC_CUBE_33:
    {
        Scramble33 prevScrambleType = INVALID_33;

        for (UINT i = 0; i < length;)
        {
            Scramble[i].uType = (rand() % 6) + 1;        // 1부터 6까지

            if (Scramble[i].uType == prevScrambleType)
            {
                continue;                                // 이전 스크램블과 같으면 다시 생성함
            }
            else
            {
                prevScrambleType = Scramble[i].uType;
            }

            Scramble[i].uCount = (rand() % 2) + 1;    // 1부터 2까지
            Scramble[i].bReversed = Scramble[i].uCount == 1 ? (rand() % 2) : FALSE;        // TRUE or FALSE

            i++;
        }

        break;
    }
    }

    return TRUE;
}

BOOL ScDeleteScramble(VOID)
{
    if (Scramble)
    {
        free(Scramble);
        Scramble = NULL;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

const ScrambleStruct* ScGetScramble(VOID)
{
    return Scramble;
}

BOOL ScScrambleToString(LPWSTR lpBuffer, SIZE_T bufferSize)
{
    if (!Scramble || !lpBuffer)
    {
        return FALSE;
    }

    switch (ScrambleCubeType)
    {
        case SC_CUBE_33:
        {
            return ScpScrambleToString33(lpBuffer, bufferSize);
        }
    }

    return FALSE;
}

