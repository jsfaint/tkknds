#ifndef __GAME_H__
#define __GAME_H__

#include "header.h"
#ifdef SAVE_DATA
#include "save.h"
#endif

/************* Structure ************/
typedef struct{
	s32 x;
	s32 y;
} Plane;

//type of bullet, normal, hunter, explode, and etc.
typedef enum BULLETTYPE{
	Bullet_Normal = 0,
	Bullet_Hunter,
	Bullet_Explode,
} BulletType;

typedef struct{
	s32 x;
	s32 y;
	s16 vx;
	s16 vy;
	BulletType type;
} Bullet;

extern Option g_option;

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
