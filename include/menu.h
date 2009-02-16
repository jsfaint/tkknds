#ifndef __MENU_H__
#define __MENU_H__

#include <PA9.h>
#if !defined(__HEADER_H__)
#include "header.h"
#endif

extern u8 g_Select;

//menu.c
void vMenuInit(u8 *gameState);
void vMenuShow(u8 *gameState);
void vSplashScreen(void);

#endif // __MENU_H__
