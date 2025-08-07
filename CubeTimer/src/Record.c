#include "Debug.h"
#include "Record.h"

/* Defines ****************************************************************/

typedef struct tagRecordNode {
    RecordStruct          record;
    BOOL                  bPlusTwo;
    struct tagRecordNode* next;
} RecordNode;

/* Global variables ************************************************************/

// Record Table

static RecordNode* RecordTableHead;
static UINT        RecordTableSize;

// Current Node

static RecordNode* CurrentNode;

/* Private functions ****************************************************************/

MillisecTime RcpGetAoRecord(UINT count, MillisecTime* array)
{
    if (count <= 2)
    {
        return 0ll;
    }

    MillisecTime sum = 0;
    MillisecTime less = array[0], great = array[0];
    UINT         lessIdx = 0, greatIdx = 0;

    for (UINT i = 0; i < count; i++)
    {
        if (array[i] < less)
        {
            less = array[i];
            lessIdx = i;
        }
        else if (array[i] > great)
        {
            great = array[i];
            greatIdx = i;
        }
    }

    for (UINT i = 0; i < count; i++)
    {
        if (i == lessIdx || i == greatIdx)
        {
            continue;
        }
        else
        {
            sum += array[i];
        }
    }

    sum = sum / (count - 2);

    return sum;
}

MillisecTime RcpGetAoRecordByNode(UINT count, RecordNode* node)
{
    RecordNode* currentNode = node;
    MillisecTime* records   = malloc(sizeof(MillisecTime) * count);

    if (!records || !node)
    {
        return 0;
    }

    for (UINT i = 0; i < count; i++)
    {
        if (!currentNode)
        {
            free(records);
            return 0;
        }
        records[i] = currentNode->record.record;
        currentNode = currentNode->next;
    }

    MillisecTime ao = RcpGetAoRecord(count, records);
    free(records);
    return ao;
}

BOOL RcpUpdateAoRecord(VOID)
{
    if (!RecordTableHead)
    {
        return FALSE;
    }
    if (RecordTableSize < 5)
    {
        return FALSE;
    }

    RecordNode* currentNode = RecordTableHead;

    for (UINT i = 0; i <= RecordTableSize - 5; i++)
    {
        if (i <= RecordTableSize - 5)
        {
            MillisecTime currentAo5 = RcpGetAoRecordByNode(5, currentNode);

            RecordNode* ao5Node = currentNode;
            for (int i = 0; i < 5 - 1; i++)
            {
                ao5Node = ao5Node->next;
            }

            ao5Node->record.ao5 = currentAo5;
        }

        if (i <= RecordTableSize - 12 && RecordTableSize >= 12)
        {
            MillisecTime currentAo12 = RcpGetAoRecordByNode(12, currentNode);

            RecordNode* ao12Node = currentNode;
            for (int i = 0; i < 12 - 1; i++)
            {
                ao12Node = ao12Node->next;
            }

            ao12Node->record.ao12 = currentAo12;
        }
        currentNode = currentNode->next;
    }

    return TRUE;
}

BOOL RcpDestroyRecordTable(VOID)
{
    RecordNode* currentNode = RecordTableHead;

    while (currentNode)
    {
        RecordNode* nextNode = currentNode->next;
        free(currentNode->record.scramble);
        free(currentNode);
        currentNode = nextNode;
    }

    RecordTableHead = NULL;

    return TRUE;
}

/* Global function *****************************************/

BOOL RcInitializeRecordTable(VOID)
{
    RecordTableHead = NULL;
    RecordTableSize = 0;
    return TRUE;
}

BOOL RcUninitializeRecordTable(VOID)
{
    RcpDestroyRecordTable();
    return TRUE;
}

BOOL RcAddRecord(MillisecTime record, LPCWSTR lpScramble)
{
    /* 할당 */
    RecordNode* newNode = calloc(1, sizeof(RecordNode));

    if (!newNode)
    {
        return FALSE;
    }

    /* 기록과 스크램블 담기 */

    newNode->record.record = record;

    newNode->record.scramble = malloc((lstrlenW(lpScramble) + 1) * sizeof(WCHAR));

    if (!newNode->record.scramble)
    {
        free(newNode);
        return FALSE;
    }

    lstrcpyW(newNode->record.scramble, lpScramble);

    /* 만든 노드를 연결하기 */

    if (RecordTableHead)
    {
        RecordNode* currentNode = RecordTableHead;

        /* 마지막 노드로 이동 */

        while (currentNode->next)
        {
            currentNode = currentNode->next;
        }

        /* 마지막 노드의 Next에 연결 */

        currentNode->next = newNode;
    }
    else
    {
        RecordTableHead = newNode;
    }

    RecordTableSize++;

    /* 평균 업데이트 */

    RcpUpdateAoRecord();

    /* 반환 */

    return TRUE;
}

BOOL RcGoToNextRecordNode(UINT index)
{
    if (!CurrentNode)
    {
        return FALSE;
    }

    for (UINT i = 0; (i < index) && (CurrentNode); i++)
    {
        CurrentNode = CurrentNode->next;
    }

    if (!CurrentNode)
    {
        return FALSE;
    }

    return TRUE;
}

BOOL RcGoToFirstRecordNode(VOID)
{
    CurrentNode = RecordTableHead;
    return TRUE;
}

const RecordStruct* RcGetCurrentNodeRecord(VOID)
{
    return CurrentNode ? &(CurrentNode->record) : NULL;
}

BOOL RcPlusTwoCurrentNodeRecord(VOID)
{
    if (CurrentNode)
    {
        CurrentNode->bPlusTwo = !CurrentNode->bPlusTwo;
        if (CurrentNode->bPlusTwo)
        {
            CurrentNode->record.record += 2000;
        }
        else
        {
            CurrentNode->record.record -= 2000;
        }
        RcpUpdateAoRecord();
    }
    else
    {
        return FALSE;
    }

    return TRUE;
}

BOOL RcIsCurrentNodePlusTwo(VOID)
{
    return CurrentNode->bPlusTwo;
}

