#ifndef AURA_H
#define AURA_H

#include "../include.h"

/*

	PSP_CTRL_SELECT
    PSP_CTRL_START
    PSP_CTRL_LEFT

    PSP_CTRL_LTRIGGER
    PSP_CTRL_RTRIGGER

 */

SceCtrlData auraPadInput;

#define auraButton(x) (auraPadInput.Buttons & x)

#define SCREEN_H	272
#define SCREEN_W	480


typedef struct{
	int w;
	int h;
	int tilesize;
	g2dTexture* tileset;
	int*	map;
	int* 	mapl2;
	int*	mapl3;
} _aura_map;

typedef struct{
	int w;
	int h;
	float x;
	float y;
	float x_old;
	float y_old;
	g2dTexture* texture;
	int facing;
	int state;
	int frame;
	int frames;
	int speed;
	int positionx;
	int positiony;
	int positionx_old;
	int positiony_old;
} _aura_character;

typedef enum{
	TRIGGER,
	LIGHT,
	DOOR,
	ITEM
}_aura_tile_type;

typedef struct{
	_aura_tile_type type;
	int x;
	int y;
} _aura_tile;

_aura_map* curMap;

float dt, dt_old;
pspTime* time;
int microseconds_old;

int auraCamX, auraCamY;

int auraInit();

void auraStartCycle();
void auraEndCycle();

_aura_map* auraLoadMap(char* mapName, int layer, int mapW, int mapH, int tilesize);
void auraGetInput();
void auraDrawMap(_aura_map* mapToDraw, int layer, int x, int y);
void auraMovePlayer(_aura_character* curChar);
void auraMoveChar(_aura_character* curChar, int destx, int desty, float eta);
void auraDrawChar(_aura_character* curChar);
void auraGameCycle();



#endif
