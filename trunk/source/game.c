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
void vBulletInit(u16 uIndex);
void vMoveBullet(void);
void vBulletInitAll(void);

s32 iGameInit(u8 *gameState, u8 uLevel)
{
    // load backgroud
    PA_EasyBgLoad(0, 0, bg);
    //PA_EasyBgLoad(1, 0, bg);

    vPlaneInit();

    memset(g_bullet, 0, sizeof(Bullet)*BULLET_MAX);
    vBulletInitAll();

    *gameState = Game_Play;
    return 0;
}

void vGamePlay(u8 *gameState)
{
    PA_OutputText(1, 3, 0, "planeX = %03d", g_Plane.x);
    PA_OutputText(1, 3, 1, "planeY = %03d", g_Plane.y);

    vMovePlane();
    vMoveBullet();
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
    g_Plane.y = SCREEN_HEIGHT/2;

    PA_LoadSpritePal(g_screen, 0, (void*)plane_Pal);
    PA_CreateSprite(g_screen, 0, (void*)plane_Sprite, OBJ_SIZE_16X16, 1, 0,
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
        g_Plane.y = PA_MovedSprite.Y - PLANE_H/2;
    }

    if (g_Plane.x <= 0)
        g_Plane.x = 0;
    else if ((g_Plane.x+ PLANE_W) > SCREEN_WIDTH)
        g_Plane.x = SCREEN_WIDTH - PLANE_W;

    if (g_Plane.y <= 0)
        g_Plane.y = 0;
    else if ((g_Plane.y+PLANE_H) > SCREEN_HEIGHT-1)
        g_Plane.y = SCREEN_HEIGHT - PLANE_H;

    //show animate
    if (Pad.Newpress.Left)
        PA_SetSpriteAnim(g_screen, 0, 1);
    else if (Pad.Released.Left)
        PA_SetSpriteAnim(g_screen, 0, 0);

    if (Pad.Newpress.Right)
        PA_SetSpriteAnim(g_screen, 0, 2);
    else if (Pad.Released.Right)
        PA_SetSpriteAnim(g_screen, 0, 0);

    PA_SetSpriteXY(g_screen, 0, g_Plane.x, g_Plane.y);
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
    if (PA_Distance(g_Plane.x+8, g_Plane.y+8, g_bullet[ii].x+2, g_bullet[ii].y+2) < 8*2)
        return TRUE;
    else
        return FALSE;
}

/********************************************
    void vBulletInit(Bullet *pBullet)
Routine Description:
    initial a bullet

Arguments:
    u16 uIndex; the index of bullet.
Return Value:
    none
********************************************/
void vBulletInit(u16 uIndex)
{
    u8 tmp; // which side, 0: up, 1: left, 2: down, 3:right
    s16 angle;
    Bullet *pBullet = &g_bullet[uIndex];

    //PA_InitRand();
    tmp = PA_RandMinMax(0,3);

    switch(tmp)
    {
        case 0: // y = 0
            pBullet->y = 0;
            pBullet->x = PA_RandMinMax(0, SCREEN_WIDTH);
            break;
        case 1: // x = 0
            pBullet->x = 0;
            pBullet->y = PA_RandMinMax(0, SCREEN_HEIGHT);
            break;
        case 2: // y = max
            pBullet->x = PA_RandMinMax(0, SCREEN_WIDTH);
            pBullet->y = SCREEN_HEIGHT - 4;
            break;
        case 3: // x = max
            pBullet->x = SCREEN_WIDTH - 4;
            pBullet->y = PA_RandMinMax(0, SCREEN_HEIGHT);
            break;
        default:
            break;
    }

    angle = PA_GetAngle(pBullet->x, pBullet->y, g_Plane.x, g_Plane.y);
    pBullet->vx = PA_Cos(angle)>>8;
    pBullet->vy = -PA_Sin(angle)>>8;

    pBullet = NULL;
}

void vBulletInitAll(void)
{
    u16 uIndex;
    
    for (uIndex = 0; uIndex <= BULLET_MAX; uIndex++)
    {
        vBulletInit(uIndex);

        PA_LoadSpritePal(g_screen, uIndex+1, (void*)bullet_Pal);
        PA_CreateSprite(g_screen, uIndex+1, (void*)bullet_Pal, OBJ_SIZE_8X8, 1, 0,
                            g_bullet[uIndex].x, g_bullet[uIndex].y);
    }
}

void vMoveBullet(void)
{
    u16 uIndex;
    s16 angle;
    
    for (uIndex=0; uIndex<BULLET_MAX; uIndex++)
    {
        if(g_bullet[uIndex].x > SCREEN_WIDTH || g_bullet[uIndex].x < 0
            || g_bullet[uIndex].y > SCREEN_HEIGHT || g_bullet[uIndex].y < 0)
            vBulletInit(uIndex);
        g_bullet[uIndex].x += g_bullet[uIndex].vx;
        g_bullet[uIndex].y += g_bullet[uIndex].vy;
        PA_SetSpriteXY(g_screen, uIndex+1, g_bullet[uIndex].x, g_bullet[uIndex].y);
    }
}
