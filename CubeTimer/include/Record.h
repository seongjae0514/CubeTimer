#ifndef TIMER_RECORD_H
#define TIMER_RECORD_H

#include <Windows.h>
#include "Defs.h"

/* Defines */

/**
 * @brief ����� ��� ����ü�Դϴ�.
 */
typedef struct tagRecordStruct {
    LPWSTR       scramble;
    MillisecTime record;
    MillisecTime ao5;
    MillisecTime ao12;
} RecordStruct;

/* Initialize ***************************************************************/

/**
 * @brief Record ����� �ʱ�ȭ�մϴ�.
 * @return �����ϸ� TRUE, �����ϸ� FALSE�� ��ȯ�մϴ�.
 */
extern BOOL RcInitializeRecordTable(VOID);

/**
 * @brief Record ��� �ʱ�ȭ�� ����մϴ�.
 * @return �����ϸ� TRUE, �����ϸ� FALSE�� ��ȯ�մϴ�.
 */
extern BOOL RcUninitializeRecordTable(VOID);

/* Node Control *************************************************************/

/**
 * @brief ����� �߰��մϴ�.
 * @param record �߰��� ����Դϴ�.
 * @return �����ϸ� TRUE, �����ϸ� FALSE�� ��ȯ�մϴ�.
 */
extern BOOL RcAddRecord(MillisecTime record, LPCWSTR lpszScramble);

/**
 * @brief ���� ���� �̵��մϴ�.
 * @param index �̵��� ����� �����Դϴ�.
 * @return �����ϸ� TRUE, �����ϸ� FALSE�� ��ȯ�մϴ�.
 */
extern BOOL RcGoToNextRecordNode(UINT index);

/**
 * @brief ù ��° ���� �̵��մϴ�.
 * @return �����ϸ� TRUE, �����ϸ� FALSE�� ��ȯ�մϴ�.
 */
extern BOOL RcGoToFirstRecordNode(VOID);

/**
 * @brief ���� ����� ����� ��ȯ�մϴ�.
 * @return ��� ����ü�� ���� �����͸� ��ȯ�մϴ�. ��� ���� ����ƽ ������ �ּҸ� ��ȯ�մϴ�.
 */
extern const RecordStruct* RcGetCurrentNodeRecord(VOID);

/**
 * @brief ���� ��忡 +2�� �մϴ�.
 * @return �����ϸ� TRUE, �����ϸ� FALSE�� ��ȯ�մϴ�.
 */
extern BOOL RcPlusTwoCurrentNodeRecord(VOID);

/* Ao and Best record *******************************************************/

/**
 * @brief �ְ� ����� ��ȯ�մϴ�.
 * @return ����� ��ȯ�մϴ�.
 */
extern MillisecTime RcGetBestRecord(VOID);

/**
 * @brief ���� ��尡 +2 �Ǿ��ִ��� ���θ� ��ȯ�մϴ�.
 */
extern BOOL RcIsCurrentNodePlusTwo(VOID);

#endif