#ifndef __HEADER_H__
#define __HEADER_H__

#include <PA9.h>

#if !defined(ALL_GFX_H)
#include "../source/gfx/all_gfx.h" // Gfx include for the header (.h)
#endif

// Macros in the header file
#define SCREENHOLE 0//Size of the space between the screens, We set 0 for this game
#define DUAL_SCREEN_HEIGHT SCREEN_HEIGHT*2+SCREENHOLE
#define DUAL_SCREEN_WIDTH SCREEN_WIDTH
#define BULLET_MAX 30

enum GameStatus{
    Menu_Init = 0,
    Menu_Show,
    Game_Init,
    Game_Play,
    Game_Statis,
    Game_Pause
};

extern u8 g_Select;

#endif //__HEADER_H__
