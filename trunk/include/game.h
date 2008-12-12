#ifndef __GAME_H__
#define __GAME_H__
#if !defined(_PA_MAIN)
#include <PA9.h>
#endif
#if !defined(__HEADER_H__)
#include "header.h"
#endif

/************* Structure ************/
typedef struct{
	s16 x;
    s16 y;
} Plane;

typedef struct{
	s16 x;
    s16 y;
	s16 vx;
    s16 vy;
} Bullet;

/************ Macro *************/
//plane size
#define PLANE_W 16
#define PLANE_H 16

/*********** Function *************/
s32 iGameInit(u8 *pGameState);
void vGamePlay(u8 *pGameState);
void vGamePause(u8 *pGameState);
void vGameStatic(u8 *pGameState);
#endif //__GAME_H__