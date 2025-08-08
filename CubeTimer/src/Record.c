#include "Debug.h"
#include <Windows.h>
#include "Record.h"

/* Defines *********************************************************************/

#define TABLE_ALLOCATE_SIZE 5

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
    /* ��� �߰� */

    if (!RcpAllocateTable())
    {
        return FALSE;
    }
    RecordStruct* currentNode = &RecordTable[RecordCount];
    RecordCount++;

    /* ��� �Ҵ� */

    currentNode->record   = record;
    currentNode->ao5      = 0;
    currentNode->ao12     = 0;
    currentNode->bPlusTwo = FALSE;
    currentNode->scramble = (LPWSTR)malloc(sizeof(WCHAR) * lstrlenW(lpScramble) + sizeof(WCHAR));
    
    if (!currentNode->scramble)
    {
        return FALSE;
    }
    
    lstrcpyW(currentNode->scramble, lpScramble);

    /* ��� ���� */

    RcpUpdateAoRecord();

    /* ��ȯ */

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