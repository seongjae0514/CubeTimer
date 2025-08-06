#ifndef TIMER_DEFS_H
#define TIMER_DEFS_H

/* 공통 해더 include */

#include <Windows.h>

/* 타입 정의 */

#ifdef _WIN64
typedef ULONGLONG MillisecTime;
#else
typedef UINT MillisecTime;
#endif


#endif