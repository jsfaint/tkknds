/*++
Copyright ? 2008 Pesonal Usage
Module Name:
    game.c
Abstract:

Author:
    jason (jsfaint@gmail.com) 2008-11-12
--*/

#include "game.h"
#include "gfx/all_gfx.c"

Plane g_Plane;
Bullet g_bullet[BULLET_MAX];

/************** Function *****************/
static void vPlaneInit(void);
static void vMovePlane(void);
bool bCheckCollision(u8 ii);


s32 iGameInit(u8 *gameState, u8 uLevel)
{
    // load backgroud
	PA_EasyBgLoad(0, 0, bg);
	PA_EasyBgLoad(1, 0, bg);

    vPlaneInit();


    *gameState = Game_Play;
    return 0;
}

void vGamePlay(u8 *gameState)
{

    vMovePlane();
    if(Pad.Newpress.A || Pad.Newpress.B || Pad.Newpress.X || Pad.Newpress.Y)
        *gameState = Game_Pause;

}

void vGamePause(u8 *gameState)
{
    if(Pad.Newpress.A || Pad.Newpress.B || Pad.Newpress.X || Pad.Newpress.Y)
        *gameState = Game_Play;
    else
        *gameState = Game_Pause;
}

/*********************************************************
    void vPlaneInit(void)
Routine Description:
    initial plane.
Arguments:
    void
Return Value:
    None
***********************************************************/
static void vPlaneInit(void)
{
	// fix position with spirites size
	g_Plane.x = SCREEN_WIDTH/2;
	g_Plane.y = SCREEN_HEIGHT + SCREEN_HEIGHT/2;

    PA_DualLoadSpritePal(0, (void*)plane_Pal);
    PA_DualCreateSprite(0, (void*)plane_Sprite, OBJ_SIZE_16X16, 1, 0,
                        g_Plane.x - PLANE_W/2, g_Plane.y - PLANE_H/2);
}

/*********************************************************
    vMovePlane(void)
Routine Description:
    handle plane moving
Arguments:
    void
Return Value:
    None
***********************************************************/
static void vMovePlane(void)
{
	g_Plane.x += Pad.Held.Right - Pad.Held.Left;
	g_Plane.y += Pad.Held.Down - Pad.Held.Up;

	if (PA_MoveSprite(0)){
		g_Plane.x = PA_MovedSprite.X - PLANE_W/2;
		g_Plane.y = PA_MovedSprite.Y + SCREEN_HEIGHT - PLANE_H/2;
	}

	if (g_Plane.x <= 0)
		g_Plane.x = 0;
	else if ((g_Plane.x+ PLANE_W) > SCREEN_WIDTH)
		g_Plane.x = SCREEN_WIDTH - PLANE_W;

	if (g_Plane.y <= 0)
		g_Plane.y = 0;
	else if ((g_Plane.y+PLANE_H) > SCREEN_HEIGHT*2-1)
		g_Plane.y = SCREEN_HEIGHT*2-1 - PLANE_H;

	//for explode animate..
	if (Pad.Newpress.A)
		PA_DualSetSpriteAnim(0, 1);
	else if (Pad.Released.A)
		PA_DualSetSpriteAnim(0, 0);

    PA_DualSetSpriteXY(0,g_Plane.x, g_Plane.y);
}

/********************************************
bool bCheckCollision(u8 ii)
Routine Description:
    check if the given bullent is collision with plane.
Arguments:
    u8 ii, the index of bullet.
Return Value:
    bool, collision return true, no collision return false.
********************************************/
bool bCheckCollision(u8 ii)
{
    // Consider these spirites as circle.
    if (PA_Distance(g_Plane.x+8, g_Plane.y+8, g_bullet[ii].x+4, g_bullet[ii].y+4) < 8*4)
        return TRUE;
    else
        return FALSE;
}