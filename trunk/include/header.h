#ifndef __HEADER_H__
#define __HEADER_H__

#include <PA9.h>

#define VERSION "v0.91"
#define STYLUS_CONTRL
#define SAVE_DATA

#define BULLET_MIN 50
#define BULLET_MAX 100
#define BULLET_INCREASE_INTEVAL 3 // 3 seconds

enum GameStatus{
    Menu_Init = 0,
    Menu_Show,
    Game_Init,
    Game_Play,
    Game_Statistic,
    Game_Pause
};


extern u8 g_screen;
extern s32 g_count;
extern u32 g_bulletNum;
extern u8 g_bLibfat;
#endif //__HEADER_H__
