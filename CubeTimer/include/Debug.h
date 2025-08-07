#ifndef TIMER_DEBUG_H
#define TIMER_DEBUG_H

#ifdef _DEBUG

#include <Windows.h>
#include <stdio.h>

#endif

#ifdef _DEBUG
#define INITIALIZE_DEBUG_STREAM() \
        do { \
            FILE* newStream; \
            AllocConsole(); \
            freopen_s(&newStream, "CONOUT$", "w", stdout); \
        } while(0)

#define PRINT_DEBUG_STRING(args, ...) printf(args, ##__VA_ARGS__)
#define PRINT_DEBUG_MEMORY_CHECK() _CrtDumpMemoryLeaks()
#define PRINT_FUNCTION_NAME() printf("%s\n", __func__)
#define _CRTDBG_MAP_ALLOC
#include <CRTDBG.H>
#else
#define INITIALIZE_DEBUG_STREAM()
#define PRINT_DEBUG_STRING(args, ...)
#define PRINT_FUNCTION_NAME()
#define PRINT_DEBUG_MEMORY_CHECK()
#endif

#endif