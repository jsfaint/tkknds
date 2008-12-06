#ifndef __GAME_H__
#define __GAME_H__

#include <PA9.h>
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
#define BULLET_MAX 50

/*********** Function *************/
s32 iGameInit(u8 *gameState, u8 uLevel);
void vGamePlay(u8 *gameState);
void vGamePause(u8 *gameState);
void vDestructSprites(void);
#endif //__GAME_H__