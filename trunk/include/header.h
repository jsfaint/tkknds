#ifndef __HEADER_H__
#define __HEADER_H__

#include <PA9.h>

#if !defined(ALL_GFX_H)
#include "../source/gfx/all_gfx.h" // Gfx include for the header (.h)
#endif

#undef STYLUS_CONTRL

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
#endif //__HEADER_H__
