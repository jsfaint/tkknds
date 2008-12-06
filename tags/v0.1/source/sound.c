/*++

Copyright ? 2008

Module Name:

    sound.c

Abstract:

    sound function

Author:

    jason (jsfaint@gmail.com) 2008-12-04

--*/

#include "sound.h"#include "explode.h"void vSoundInitial(void){    // Init AS_Lib for normal sound playback only    AS_Init(AS_MODE_SURROUND | AS_MODE_16CH);    AS_SetDefaultSettings(AS_PCM_8BIT, 11025, AS_SURROUND);}void vSoundPlayExplode(void){    AS_SoundQuickPlay(explode);    return;}