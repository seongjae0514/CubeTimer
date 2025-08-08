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
    /* ��� ���̺��� ��ϸ� �� �迭�� ���� */

    MillisecTime* array;    
    if (!(array = (MillisecTime*)malloc(sizeof(MillisecTime) * RecordCount)))
    {
        return FALSE;
    }

    for (UINT i = 0; i < RecordCount; i++)
    {
        array[i] = RecordTable[i].record;
    }

    /* Ao5 ���ؼ� ���� */

    if (RecordCount >= 5)
    {
        for (UINT i = 0; i <= RecordCount - 5; i++)
        {
            RecordTable[i + 4].ao5 = RcpGetAoRecord(5, array + i);;
        }
    }

    /* Ao12 ���ؼ� ���� */

    if (RecordCount >= 12)
    {
        for (UINT i = 0; i <= RecordCount - 12; i++)
        {
            RecordTable[i + 11].ao12 = RcpGetAoRecord(12, array + i);
        }
    }

    /* �迭 ���� */

    free(array);

    /* ��ȯ */

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
    /* ��ũ���� ���� ���� */

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
    /* ��� �߰� */

    if (!RcpAllocateTable())
    {
        return FALSE;
    }
    RecordStruct* currentNode = &RecordTable[RecordCount];
    RecordCount++;

    /* ��� �Ҵ� */

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

    /* ��ȯ */

    return TRUE;
}

static INT RcpSaveRecordToFile(LPCWSTR lpszFilePath)
{
    /* ���� ���� */

    FILE* file = _wfopen(lpszFilePath, L"wb");

    if (!file)
    {
        return errno;
    }

    /* ���� ��� ���� */

    RecordFileHeaderStruct fileHeader;
    fileHeader.type  = RECORD_FILE_TYPE_DEFAULT_FILE_WIN64;
    fileHeader.count = RecordCount;
    fileHeader.time  = time(NULL);

    fwrite(&fileHeader, sizeof(fileHeader), 1, file);                             // ���� ��� �ۼ�

    /* ��ϰ� ��ũ���� ���� */

    for (UINT i = 0; i < RecordCount; i++)
    {
        SIZE_T len = lstrlenW(RecordTable[i].scramble) + 1;                       // ���ڿ� ũ�� ��� (NULL���� ����)
        fwrite(&(RecordTable[i].record), sizeof(RecordTable[i].record), 1, file); // ��� �ۼ�
        fwrite(&len, sizeof(len), 1, file);                                       // ��ũ���� ũ�� �ۼ�
        fwrite(RecordTable[i].scramble, sizeof(WCHAR), len, file);                // ��ũ���� ���ڿ� �ۼ�
    }
    
    /* ���� �� ��ȯ */

    fclose(file);
    return 0;
}

static INT RcpLoadRecordFromFile(LPCWSTR lpszFilePath)
{
    INT returnValue = 0;

    /* ���� ���� */

    FILE* file = _wfopen(lpszFilePath, L"rb");

    if (!file)
    {
        return errno;
    }

    /* ���� ��� �б� */

    RecordFileHeaderStruct fileHeader;
    fread(&fileHeader, sizeof(fileHeader), 1, file);

    /* ���� ��� �˻� */

    if (fileHeader.type != RECORD_FILE_TYPE_DEFAULT_FILE_WIN64)
    {
        return -1;
    }

    /* ��� �а� ���� */

    for (UINT i = 0; i < fileHeader.count; i++)
    {
        // ��� �б�

        MillisecTime record;
        fread(&record, sizeof(MillisecTime), 1, file);

        // ��ũ���� ���� �б�

        SIZE_T len;
        fread(&len, sizeof(SIZE_T), 1, file);

        // ��ũ���� �б�

        LPWSTR lpBuffer = (LPWSTR)malloc(sizeof(WCHAR) * len);
        
        if (!lpBuffer)
        {
            returnValue = RECORD_ERROR_ALLOCATION_FAILED;
            goto cleanup;
        }

        fread(lpBuffer, sizeof(WCHAR), len, file);

        // �ι��� ���� (Ȥ�� �� �߸��� ���ڿ��� ����ϱ� ����)

        lpBuffer[len - 1] = 0;

        // ��� ����

        if (!RcpAddRecord(record, lpBuffer))
        {
            return RECORD_ERROR_ADD_RECORD_FAILED;
        }

        // �޸� �����ϰ� ������

        free(lpBuffer);
    }

    /* ���� �� ��ȯ */

cleanup:
    fclose(file);
    return returnValue;
}

/* Global functions *****************************************************************/

BOOL RcInitialize(VOID)
{
    /* ���̺� �Ҵ� */
    
    if (!RcpAllocateTable())
    {
        return FALSE;
    }

    /* ���� �� ��ȯ */

    return TRUE;
}

BOOL RcUninitialize(VOID)
{
    /* ���̺� ���� */

    RcpDestroyTable();

    /* ���� �� ��ȯ */
    
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