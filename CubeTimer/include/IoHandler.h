#ifndef TIMER_IO_H
#define TIMER_IO_H

#include <Windows.h>

extern BOOL IoInitialize(VOID);

extern BOOL IoUninitialize(VOID);

extern BOOL IoHandleSpaceKeyDown(VOID);

extern BOOL IoHandleSpaceKeyUp(VOID);

extern BOOL IoHandleButtonPress(WPARAM wParam);

extern BOOL IoHandleMouseWheel(WPARAM wParam, LPARAM lParam);

#endif