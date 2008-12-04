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

// Function: main()
int main(int argc, char ** argv)
{
    u8 gameState = Menu_Init;
    
    PA_Init();    // Initializes PA_Lib
    PA_InitVBL(); // Initializes a standard VBL
    PA_SetScreenSpace(SCREENHOLE);
    
    // Game Splash Screens
    vSplashScreen();
    
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
        
        PA_WaitForVBL();
    }
    
    return 0;
} // End of main()
