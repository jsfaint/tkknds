#ifndef __GAME_H__
#define __GAME_H__

#if !defined(__HEADER_H__)
#include "header.h"
#endif

/************* Structure ************/
typedef struct{
	s32 x;
    s32 y;
} Plane;

typedef enum BULLETTYPE{
    normal,
    hunter,
    explode,
} BulletType;

typedef struct{
	s16 x;
    s16 y;
	s16 vx;
    s16 vy;
    BulletType bulletType;
} Bullet;

/************ Macro *************/
//plane size
#define PLANE_W 16
#define PLANE_H 16

/*********** Function *************/
s32 iGameInit(u8 *pGameState);
void vGamePlay(u8 *pGameState);
void vGamePause(u8 *pGameState);
void vGameStatistic(u8 *pGameState);
void vShowScore(void);

#endif //__GAME_H__
