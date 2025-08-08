#ifndef TIMER_RECORD_H
#define TIMER_RECORD_H

#include <Windows.h>
#include "Defs.h"


/**
 * @brief ����� ��� ����ü�Դϴ�.
 */
typedef struct tagRecordStruct {
    LPWSTR       scramble;   // ��ũ����
    MillisecTime record;     // ���
    MillisecTime ao5;        // 5ȸ �������
    MillisecTime ao12;       // 12ȸ �������
    BOOL         bPlusTwo;   // +2 �Ǿ��ִ��� ����
} RecordStruct;

/**
 * @brief Record ����� �ʱ�ȭ�մϴ�.
 * @return �����ϸ� TRUE, �����ϸ� FALSE�� ��ȯ�մϴ�.
 */
extern BOOL RcInitialize(VOID);

/**
 * @brief Record ��� �ʱ�ȭ�� ����մϴ�.
 * @return �����ϸ� TRUE, �����ϸ� FALSE�� ��ȯ�մϴ�.
 */
extern BOOL RcUninitialize(VOID);

/**
 * @brief ����� �߰��մϴ�.
 * @return �����ϸ� TRUE, �����ϸ� FALSE�� ��ȯ�մϴ�.
 */
extern BOOL RcAddRecord(MillisecTime record, LPCWSTR lpScramble);

/**
 * @brief �ش� �ε����� ��Ͽ� +2�� ����մϴ�.
 * @return �����ϸ� TRUE, �����ϸ� FALSE�� ��ȯ�մϴ�.
 */
extern BOOL RcTogglePlusTwo(UINT uIndex);

/**
 * @brief �ش� �ε����� ��� ����ü�� �����͸� ��ȯ�մϴ�.
 * @return ��� ����ü�� �ּ��Դϴ�.
 */
extern const RecordStruct* RcGetRecordStructAddress(UINT uIndex);

/**
 * @brief ���ڵ� ���̺��� ���ڵ� ������ ��ȯ�մϴ�.
 */
extern UINT RcGetRecordCount(VOID);

#endif