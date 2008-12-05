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
bool bCheckCollision();
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
    u8 ii;
/*
    PA_OutputText(1, 0, 0, "plane: %03d, %03d", g_Plane.x, g_Plane.y);
    for (ii=0; ii<BULLET_MAX; ii+=2)
    {
        PA_OutputText(1, 0, 1+ii/2, "%03d, %03d     %03d, %03d",
                    g_bullet[ii].x, g_bullet[ii].y,
                    g_bullet[ii+1].x, g_bullet[ii+1].y);
    }
*/

    vMovePlane();
    vMoveBullet();
    if(bCheckCollision())
    {
        PA_SetSpriteAnim(g_screen, 0, 3);
        *gameState = Game_Pause;
    }

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
    handle plane moving, control the plane by Pad or Stylus
Arguments:
    void
Return Value:
    None
***********************************************************/
static void vMovePlane(void)
{
    s16 angle;

    g_Plane.x += Pad.Held.Right - Pad.Held.Left;
    g_Plane.y += Pad.Held.Down - Pad.Held.Up;

    //show animate
    if (Pad.Newpress.Left)
        PA_SetSpriteAnim(g_screen, 0, 1);
    else if (Pad.Released.Left)
        PA_SetSpriteAnim(g_screen, 0, 0);

    if (Pad.Newpress.Right)
        PA_SetSpriteAnim(g_screen, 0, 2);
    else if (Pad.Released.Right)
        PA_SetSpriteAnim(g_screen, 0, 0);

    // follow the Stylus
    if (Stylus.Held)
    {
        angle = PA_GetAngle(g_Plane.x+8, g_Plane.y+8, Stylus.X, Stylus.Y);
        g_Plane.x += (PA_Cos(angle)>>8);
        g_Plane.y -= (PA_Sin(angle)>>8);

        if ((PA_Cos(angle)>>8)> 0)// right, display 2
            PA_SetSpriteAnim(g_screen, 0, 2);
        else if ((PA_Cos(angle)>>8)< 0) // left, display 1
            PA_SetSpriteAnim(g_screen, 0, 1);
    }
    else if (Stylus.Released)
        PA_SetSpriteAnim(g_screen, 0, 0); // stop, display 0

    if (g_Plane.x <= 0)
        g_Plane.x = 0;
    else if ((g_Plane.x+ PLANE_W) > SCREEN_WIDTH)
        g_Plane.x = SCREEN_WIDTH - PLANE_W;

    if (g_Plane.y <= 0)
        g_Plane.y = 0;
    else if ((g_Plane.y+PLANE_H) > SCREEN_HEIGHT-1)
        g_Plane.y = SCREEN_HEIGHT - PLANE_H;

    PA_SetSpriteXY(g_screen, 0, g_Plane.x, g_Plane.y);
}

/********************************************
bool bCheckCollision()
Routine Description:
    check if the given bullent is collision with plane.
    Consider these spirites as circle.
Arguments:
    void
Return Value:
    bool, collision return true, no collision return false.
********************************************/
bool bCheckCollision()
{
    s16 ii;
    for (ii=0; ii<BULLET_MAX; ii++)
    {
        if (PA_Distance(g_Plane.x+8, g_Plane.y+8, g_bullet[ii].x+4, g_bullet[ii].y+4) < 8*4)
            return TRUE;
    }

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
        PA_CreateSprite(g_screen, uIndex+1, (void*)bullet_Sprite, OBJ_SIZE_8X8, 1, 1,
                            g_bullet[uIndex].x, g_bullet[uIndex].y);
    }
}

void vMoveBullet(void)
{
    u16 uIndex;

    for (uIndex=0; uIndex<BULLET_MAX; uIndex++)
    {
        if(g_bullet[uIndex].x >= SCREEN_WIDTH || g_bullet[uIndex].x <= 0
            || g_bullet[uIndex].y >= SCREEN_HEIGHT || g_bullet[uIndex].y <= 0)
            vBulletInit(uIndex);
        g_bullet[uIndex].x += g_bullet[uIndex].vx;
        g_bullet[uIndex].y += g_bullet[uIndex].vy;
        PA_SetSpriteXY(g_screen, uIndex+1, g_bullet[uIndex].x, g_bullet[uIndex].y);
    }
}
