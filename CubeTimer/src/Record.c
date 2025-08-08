#include "Debug.h"

#include <Windows.h>
#include <stdio.h>
#include <wchar.h>
#include <time.h>
#include <string.h>

#include "Record.h"

/* Defines *********************************************************************/

#define TABLE_ALLOCATE_SIZE 5

typedef struct tagRecordFileHeaderStruct {
    DWORD  type;
    time_t time;
    UINT   count;
} RecordFileHeaderStruct;

#define RECORD_FILE_TYPE_DEFAULT_FILE_WIN64 0x00000001

/* Global variables ************************************************************/

static RecordStruct* RecordTable     = NULL;
static UINT          RecordCount     = 0;
static UINT          RecordTableSize = 0;

/* Private functions ****************************************************************/

static MillisecTime RcpGetAoRecord(UINT count, MillisecTime* array)
{
    if (!array)
    {
        return FALSE;
    }

    MillisecTime great = array[0], less = array[0];
    MillisecTime sum   = 0;

    for (UINT i = 0; i < count; i++)
    {
        if (array[i] > great)
        {
            great = array[i];
        }
        else if (array[i] < less)
        {
            less = array[i];
        }
        sum += array[i];
    }

    sum -= great + less;

    return sum / (count - 2);
}

static BOOL RcpUpdateAoRecord(VOID)
{
    /* 기록 테이블에서 기록만 얻어서 배열에 저장 */

    MillisecTime* array;    
    if (!(array = (MillisecTime*)malloc(sizeof(MillisecTime) * RecordCount)))
    {
        return FALSE;
    }

    for (UINT i = 0; i < RecordCount; i++)
    {
        array[i] = RecordTable[i].record;
    }

    /* Ao5 구해서 저장 */

    if (RecordCount >= 5)
    {
        for (UINT i = 0; i <= RecordCount - 5; i++)
        {
            RecordTable[i + 4].ao5 = RcpGetAoRecord(5, array + i);;
        }
    }

    /* Ao12 구해서 저장 */

    if (RecordCount >= 12)
    {
        for (UINT i = 0; i <= RecordCount - 12; i++)
        {
            RecordTable[i + 11].ao12 = RcpGetAoRecord(12, array + i);
        }
    }

    /* 배열 해제 */

    free(array);

    /* 반환 */

    return TRUE;
}

static BOOL RcpAllocateTable(VOID)
{
    if (RecordCount >= RecordTableSize)
    {
        RecordStruct* ptr = realloc(RecordTable, sizeof(RecordStruct) * (RecordTableSize + TABLE_ALLOCATE_SIZE));
        if (!ptr)
        {
            return FALSE;
        }
        else
        {
            RecordTableSize += TABLE_ALLOCATE_SIZE;
            RecordTable = ptr;
        }
    }
    return TRUE;
}

static BOOL RcpDestroyTable(VOID)
{
    /* 스크램블 버퍼 해제 */

    if (!RecordTable)
    {
        return TRUE;
    }

    for (UINT i = 0; i < RecordCount; i++)
    {
        free(RecordTable[i].scramble);
    }

    free(RecordTable);

    RecordTable = NULL;
    RecordCount = 0;
    RecordTableSize = 0;

    return TRUE;
}

static BOOL RcpAddRecord(MillisecTime record, LPCWSTR lpszScramble)
{
    /* 기록 추가 */

    if (!RcpAllocateTable())
    {
        return FALSE;
    }
    RecordStruct* currentNode = &RecordTable[RecordCount];
    RecordCount++;

    /* 기록 할당 */

    currentNode->record = record;
    currentNode->ao5 = 0;
    currentNode->ao12 = 0;
    currentNode->bPlusTwo = FALSE;
    currentNode->scramble = (LPWSTR)malloc(sizeof(WCHAR) * lstrlenW(lpszScramble) + sizeof(WCHAR));

    if (!currentNode->scramble)
    {
        return FALSE;
    }

    lstrcpyW(currentNode->scramble, lpszScramble);

    /* 반환 */

    return TRUE;
}

static INT RcpSaveRecordToFile(LPCWSTR lpszFilePath)
{
    /* 파일 열기 */

    FILE* file = _wfopen(lpszFilePath, L"wb");

    if (!file)
    {
        return errno;
    }

    /* 파일 헤더 쓰기 */

    RecordFileHeaderStruct fileHeader;
    fileHeader.type  = RECORD_FILE_TYPE_DEFAULT_FILE_WIN64;
    fileHeader.count = RecordCount;
    fileHeader.time  = time(NULL);

    fwrite(&fileHeader, sizeof(fileHeader), 1, file);                             // 파일 헤더 작성

    /* 기록과 스크램블 쓰기 */

    for (UINT i = 0; i < RecordCount; i++)
    {
        SIZE_T len = lstrlenW(RecordTable[i].scramble) + 1;                       // 문자열 크기 얻기 (NULL문자 포함)
        fwrite(&(RecordTable[i].record), sizeof(RecordTable[i].record), 1, file); // 기록 작성
        fwrite(&len, sizeof(len), 1, file);                                       // 스크램블 크기 작성
        fwrite(RecordTable[i].scramble, sizeof(WCHAR), len, file);                // 스크램블 문자열 작성
    }
    
    /* 정리 및 반환 */

    fclose(file);
    return 0;
}

static INT RcpLoadRecordFromFile(LPCWSTR lpszFilePath)
{
    INT returnValue = 0;

    /* 파일 열기 */

    FILE* file = _wfopen(lpszFilePath, L"rb");

    if (!file)
    {
        return errno;
    }

    /* 파일 헤더 읽기 */

    RecordFileHeaderStruct fileHeader;
    fread(&fileHeader, sizeof(fileHeader), 1, file);

    /* 파일 헤더 검사 */

    if (fileHeader.type != RECORD_FILE_TYPE_DEFAULT_FILE_WIN64)
    {
        return -1;
    }

    /* 기록 읽고 저장 */

    for (UINT i = 0; i < fileHeader.count; i++)
    {
        // 기록 읽기

        MillisecTime record;
        fread(&record, sizeof(MillisecTime), 1, file);

        // 스크램블 길이 읽기

        SIZE_T len;
        fread(&len, sizeof(SIZE_T), 1, file);

        // 스크램블 읽기

        LPWSTR lpBuffer = (LPWSTR)malloc(sizeof(WCHAR) * len);
        
        if (!lpBuffer)
        {
            returnValue = RECORD_ERROR_ALLOCATION_FAILED;
            goto cleanup;
        }

        fread(lpBuffer, sizeof(WCHAR), len, file);

        // 널문자 쓰기 (혹시 모를 잘못된 문자열을 대비하기 위함)

        lpBuffer[len - 1] = 0;

        // 기록 저장

        if (!RcpAddRecord(record, lpBuffer))
        {
            return RECORD_ERROR_ADD_RECORD_FAILED;
        }

        // 메모리 해제하고 끝내기

        free(lpBuffer);
    }

    /* 정리 및 반환 */

cleanup:
    fclose(file);
    return returnValue;
}

/* Global functions *****************************************************************/

BOOL RcInitialize(VOID)
{
    /* 테이블 할당 */
    
    if (!RcpAllocateTable())
    {
        return FALSE;
    }

    /* 정리 및 반환 */

    return TRUE;
}

BOOL RcUninitialize(VOID)
{
    /* 테이블 제거 */

    RcpDestroyTable();

    /* 정리 및 반환 */
    
    return TRUE;
}

BOOL RcAddRecord(MillisecTime record, LPCWSTR lpScramble)
{
    if (!RcpAddRecord(record, lpScramble))
    {
        return FALSE;
    }

    if (!RcpUpdateAoRecord())
    {
        return FALSE;
    }

    return TRUE;
}

BOOL RcTogglePlusTwo(UINT uIndex)
{
    if (uIndex >= RecordCount)
    {
        return FALSE;
    }

    if (RecordTable[uIndex].bPlusTwo)
    {
        RecordTable[uIndex].bPlusTwo = FALSE;
        RecordTable[uIndex].record   -= 2000;
    }
    else
    {
        RecordTable[uIndex].bPlusTwo = TRUE;
        RecordTable[uIndex].record   += 2000;
    }

    RcpUpdateAoRecord();

    return TRUE;
}

const RecordStruct* RcGetRecordStructAddress(UINT uIndex)
{
    return &RecordTable[uIndex];
}

UINT RcGetRecordCount(VOID)
{
    return RecordCount;
}

INT RcLoadRecordFromFile(LPCWSTR lpszFilePath)
{
    return RcpLoadRecordFromFile(lpszFilePath);
}

INT RcSaveRecordToFile(LPCWSTR lpszFilePath)
{
    return RcpSaveRecordToFile(lpszFilePath);
}