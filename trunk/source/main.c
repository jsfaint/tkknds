// Includes
#include <PA9.h>
#if !defined(__HEADER_H__)
#include "header.h"
#endif
#if !defined(__MENU_H__)
#include "menu.h"
#endif
#if !defined(__GAME_H__)
#include "game.h"
#endif
#if !defined(__SOUND_H__)
#include "sound.h"
#endif


u8  g_screen = 0;
s32 g_score = 0;

// Function: main()
int main(int argc, char ** argv)
{
    u8 gameState = Menu_Init;
    
    PA_Init();    // Initializes PA_Lib
    PA_InitVBL(); // Initializes a standard VBL
    
    PA_InitText(1,0); // On the top screen

    // Game Splash Screens
    //vSplashScreen();
    
    vSoundInitial();
    // Infinite loop to keep the program running
    while (1)
    {
        switch(gameState)
        {
        case Menu_Init:
            vMenuInit(&gameState);
            break;
        case Menu_Show:
            vMenuShow(&gameState);
            break;
        case Game_Init:
            iGameInit(&gameState, 0);
            break;
        case Game_Play:
            vGamePlay(&gameState);
            break;
        
        case Game_Pause:
            vGamePause(&gameState);
            break;
        
        case Game_Statis:
            
            break;
        default:
            break;
        }
        
        PA_OutputText(1, 0, 1, "%08ds %04dms", Tick(DEFAULT_TIMER_ID)/1000, Tick(DEFAULT_TIMER_ID)%1000);
        
        PA_WaitForVBL();
    }
    
    return 0;
} // End of main()
