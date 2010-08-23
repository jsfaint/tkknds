#ifndef __SAVE_H__
#define __SAVE_H__

#include "header.h"

#define SUCCESS 0
#define FAILED  -1
/************ Macro *************/
#define MAX_SAVE_ITEM     10
#define MAX_NAME_LEN    32
#define SAVE_SIGNATURE "TKKNDS"
#define FILENAME "fat:/TKKNDS.dat"

/************* Structure ************/
typedef struct {
	unsigned char szUsername[MAX_NAME_LEN];
	unsigned long count;
	unsigned char bulletNum;
} Score, *PSCore;

typedef struct {
	unsigned char sound_enable;
	unsigned char music_enable;
} Option, *POption;

typedef struct {
	Score  top[MAX_SAVE_ITEM];
	Option option;
} Save, *PSave;

/*************Function**************/
int iCheckData(void); //if return failed, default the data.
int getPosition(long count, int bulletNum);
int iLoadData(PSave psData);
int iSaveData(Score score);

int iGetOption(POption pOption);
int iSetOption(Option option);
#endif //__SAVE_H__
