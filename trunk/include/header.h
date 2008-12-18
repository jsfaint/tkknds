#ifndef __HEADER_H__
#define __HEADER_H__

#if !defined(_PA_MAIN)
#include <PA9.h>
#endif
#if !defined(ALL_GFX_H)
#include "../source/gfx/all_gfx.h" // Gfx include for the header (.h)
#endif

#define STYLUS_CONTRL 1

#define BULLET_MIN 50
#define BULLET_MAX 100
#define BULLET_INCREASE_INTEVAL 3

enum GameStatus{
    Menu_Init = 0,
    Menu_Show,
    Game_Init,
    Game_Play,
    Game_Statis,
    Game_Pause
};


extern u8 g_screen;
extern s32 g_count;
extern u32 g_bulletNum;
extern u8 g_bLibfat;
#endif //__HEADER_H__
