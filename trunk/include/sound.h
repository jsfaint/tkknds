#ifndef __SOUND_H__
#define __SOUND_H__

#if !defined(_PA_MAIN)
#include <PA9.h>
#endif

/*********** Function *************/
void vSoundInitial(void);
void vSoundPlayExplode(void);
void vSoundPlayBgm(void);
//void vSoundPlayOp(void);
#endif //__SOUND_H__