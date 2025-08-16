#ifndef TIMER_OPTION_H
#define TIMER_OPTION_H

#include <Windows.h>

typedef enum tagOptionKey {
	OPTION_KEY_TIMER_SIZE, OPTION_KEY_RECORD_SIZE,
	OPTION_KEY_TIMER_MODE,
	COUNT_OPTION_KEY
} OptionKey;

#define OPTION_VALUE_TIMER_TYPE_DEFAULT 0
#define OPTION_VALUE_TIMER_TYPE_INPUT 1

extern BOOL OptInitialize(VOID);
extern BOOL OptUninitialize(VOID);

extern BOOL OptGetKeyValue(OptionKey key, UINT_PTR* pValue);
extern BOOL OptSetKeyValue(OptionKey key, UINT_PTR value);

extern BOOL OptSetOptions(HWND hwndParent);

#endif