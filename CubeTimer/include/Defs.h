#ifndef TIMER_DEFS_H
#define TIMER_DEFS_H

/* ���� �ش� include */

#include <Windows.h>

/* Ÿ�� ���� */

#ifdef _WIN64
typedef ULONGLONG MillisecTime;
#else
typedef UINT MillisecTime;
#endif


#endif