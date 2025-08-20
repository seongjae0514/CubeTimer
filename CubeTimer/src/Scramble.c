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
    /* �������� ���� */

    SIZE_T bufferIndex = 0;          // ���� ũ�� �����÷ο� ������ �ε��� ī��Ʈ

    /* ���ۿ� ��ũ���� �迭 ��ȸ�ϸ鼭 ���� */

    for (UINT i = 0; i < ScrambleLength; i++)
    {
        /* ��ȣ ���� */

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

        /* ���� (Ƚ��) ���� */

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

        /* ���� ���� */

        if (bufferIndex >= bufferSize)
        {
            break;
        }

        if (Scramble[i].bReversed)
        {
            lpBuffer[bufferIndex] = L'\'';
            bufferIndex++;
        }

        /* ���鹮�� ���� (��ũ���� ��ȣ���� �������� �����ؾ���.) */

        if (bufferIndex >= bufferSize)
        {
            lpBuffer[bufferIndex - 1] = 0;
            break;
        }

        if (i + 1 != ScrambleLength) // ������ ��ȣ���ٸ� ������ �� �ʿ䰡 ����
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
            Scramble[i].uType = (rand() % 6) + 1;        // 1���� 6����

            if (Scramble[i].uType == prevScrambleType)
            {
                continue;                                // ���� ��ũ����� ������ �ٽ� ������
            }
            else
            {
                prevScrambleType = Scramble[i].uType;
            }

            Scramble[i].uCount = (rand() % 2) + 1;    // 1���� 2����
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

