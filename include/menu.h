#ifndef __MENU_H__
#define __MENU_H__

#if !defined(_PA_MAIN)
#include <PA9.h>
#endif
#if !defined(__HEADER_H__)
#include "header.h"
#endif

//menu.c
void vMenuInit(u8 *gameState);
void vMenuShow(u8 *gameState);
void vSplashScreen(void);

#endif // __MENU_H__
