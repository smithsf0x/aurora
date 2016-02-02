/*
* @Author: ich
* @Date:   2015-12-25 00:38:23
* @Last Modified by:   smithsf0x
* @Last Modified time: 2016-01-29 23:05:05
*/

#include "aurora.h"


int auraInit() {

    #ifdef DEBUG
    printf("--> auraInit() \n");
    #endif

    time = malloc(sizeof(pspTime));
    g2dInit();
    sceCtrlSetSamplingCycle(0);
    sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);

    return 0;
}

void auraStartCycle(){
    sceRtcGetCurrentClockLocalTime(time);

    //auraBeginCycle();
    dt = time->microseconds - microseconds_old;
    dt = dt / 1000000;

    if(dt > 0.1) dt = dt_old;
}

void auraEndCycle(){
    microseconds_old = time->microseconds;

        dt_old = dt;
}

_aura_map* auraLoadMap(char* mapName, int layer, int mapW, int mapH, int tilesize){

    #ifdef DEBUG
        printf("--> auraLoadMap(%s, %d, %d, %d, %d) \n", mapName, layer, mapW, mapH, tilesize);
    #endif

    _aura_map* tempMap = malloc(sizeof(_aura_map));

    char str[128];
    strcpy(str, "maps/");
    strcat(str, mapName);
    strcat(str, "/");
    strcat(str, mapName);
    strcat(str, ".png");
    tempMap->tileset = g2dTexLoad( str, G2D_SWIZZLE);
    str[0] = '\0';

    strcpy(str, "maps/");
    strcat(str, mapName);
    strcat(str, "/");
    strcat(str, mapName);
    strcat(str, "_l1.csv");
    tempMap->map = readCSV(str);
    str[0] = '\0';

    if(layer >= 2){
        strcpy(str, "maps/");
        strcat(str, mapName);
        strcat(str, "/");
        strcat(str, mapName);
        strcat(str, "_l2.csv");
        tempMap->mapl2 = readCSV(str);
        str[0] = '\0';
    }else{
        tempMap->mapl2 = NULL;
    }

    if(layer >= 3){
        strcpy(str, "maps/");
        strcat(str, mapName);
        strcat(str, "/");
        strcat(str, mapName);
        strcat(str, "_l3.csv");
        tempMap->mapl3 = readCSV(str);
        str[0] = '\0';
    }else{
        tempMap->mapl3 = NULL;
    }


    tempMap->w = mapW;
    tempMap->h = mapH;
    tempMap->tilesize = tilesize;

    #define CHECK_CSV_MAP
    #ifdef CHECK_CSV_MAP
    int i;
    for ( i = 0; i < (tempMap->w * tempMap->h); i++){
        if(tempMap->map[i] == -1){
            tempMap->map[i] = 84;
        }
    }
    if(tempMap->mapl2 != NULL){
        for ( i = 0; i < (tempMap->w * tempMap->h); i++){
            if(tempMap->mapl2[i] == -1){
                tempMap->mapl2[i] = 84;
            }
        }
    }
    if(tempMap->mapl3 != NULL){
        for ( i = 0; i < (tempMap->w * tempMap->h); i++){
            if(tempMap->mapl3[i] == -1){
                tempMap->mapl3[i] = 84;
            }
        }
    }
    #endif

    #ifdef DEBUG
        printf("--> auraLoadMap() END // OK\n");
    #endif

	return tempMap;
}

void auraGetInput(){
    sceCtrlPeekBufferPositive(&auraPadInput, 1);

}

void auraDrawMap(_aura_map* mapToDraw, int layer, int xOffset, int yOffset){
    int x, y;
    int curCropX, curCropY;
    int curTileNumber;
    if(layer == 2 && mapToDraw->mapl2 == NULL){
        #ifdef DEBUG
        printf("--> No second Map layer to draw\n");
        #endif
        return ;
    }
    if(layer == 3 && mapToDraw->mapl3 == NULL){
        #ifdef DEBUG
        printf("--> No third Map layer to draw\n");
        #endif
        return ;
    }

    for(x = 0; x < mapToDraw->w; x++){
        for(y = 0; y < mapToDraw->h; y++){
            g2dBeginRects(mapToDraw->tileset);
            g2dSetCropWH( mapToDraw->tilesize, mapToDraw->tilesize);

            if(layer == 1){
                curTileNumber = mapToDraw->map[x + (y * mapToDraw->w)];
            }else if (layer == 2){
                curTileNumber = mapToDraw->mapl2[x + (y * mapToDraw->w)];
            }else if (layer == 3){
                curTileNumber = mapToDraw->mapl3[x + (y * mapToDraw->w)];
            }

            while (curTileNumber * mapToDraw->tilesize >= mapToDraw->tileset->w){
                curTileNumber -= mapToDraw->tileset->w/mapToDraw->tilesize;
                curCropY++;
            }

            curCropX = curTileNumber ;
            g2dSetCoordXY(x * mapToDraw->tilesize + (xOffset * mapToDraw->tilesize),
                          y * mapToDraw->tilesize + (yOffset * mapToDraw->tilesize));

            g2dSetCropXY( curCropX* mapToDraw->tilesize,curCropY* mapToDraw->tilesize);
            g2dSetScaleWH( mapToDraw->tilesize,  mapToDraw->tilesize);
            g2dAdd();

            curTileNumber = 0;
            curCropX = 0;
            curCropY = 0;
        }
    }
}

void auraMovePlayer(_aura_character* curChar){
    int curTile = 0;

    int tleft, tright, ttop, tbottom;

    int tempx = 0, tempy = 0;

    if(auraButton(PSP_CTRL_LTRIGGER)){
        curChar->speed = 64;
    }else{
        curChar->speed = 32;
    }

    if(auraButton(PSP_CTRL_UP)){
        curChar->y -= (curChar->speed * dt);
        curChar->facing = 1;
    }
    if(auraButton(PSP_CTRL_DOWN)){
        curChar->y += (curChar->speed * dt);
        curChar->facing = 2;
    }
    if(auraButton(PSP_CTRL_LEFT)){
        curChar->x -= (curChar->speed * dt);
        curChar->facing = 3;
    }
    if(auraButton(PSP_CTRL_RIGHT)){
        curChar->x += (curChar->speed * dt);
        curChar->facing = 4;
    }

    if(curChar->x <= 0 ) curChar->x = 0;
    if(curChar->y <= 0 ) curChar->y = 0;
    if(curChar->x + curChar->w >= curMap->w * curMap->tilesize)
        curChar->x = curMap->w * curMap->tilesize - curChar->w;
    if(curChar->y + curChar->h >= curMap->h * curMap->tilesize)
        curChar->y = curMap->h * curMap->tilesize - curChar->h;


    curChar->positionx = ((curChar->x+(curChar->w/2))/curMap->tilesize);
    curChar->positiony = ((curChar->y+(curChar->h/2))/curMap->tilesize);

    curTile = curChar->positionx + (curChar->positiony * curMap->w);

    tleft = curTile - 1;
    tright = curTile + 1;
    ttop = curChar->positionx + ((curChar->positiony - 1) * curMap->w);
    tbottom  = curChar->positionx + ((curChar->positiony + 1) * curMap->w);

    if(curChar->positionx == 0) tleft = curTile;
    if(curChar->positionx == (curMap->w - 1)) tright = curTile;
    if(curChar->positiony == 0) ttop = curTile;
    if(curChar->positiony == (curMap->h - 1)) tbottom = curTile;

    if(curMap->map[tleft] == 3){
        tempy = 0; tempx = 0;
        while(tleft >= curMap->w){
            tleft -= curMap->w;
            tempy++;
        }
        tempx = tleft;
        tempx *= curMap->tilesize;
        tempy *= curMap->tilesize;
        if(curChar->x < (tempx+curMap->tilesize)){
            curChar->x = (tempx+curMap->tilesize);
        }
    }
    if(curMap->map[tright] == 3){
        g2dSetColor(YELLOW);
        tempy = 0; tempx = 0;
        while(tright >= curMap->w){
            tright -= curMap->w;
            tempy++;
        }
        tempx = tright;
        tempx *= curMap->tilesize;
        tempy *= curMap->tilesize;
        if((curChar->x + curChar->w) > tempx){
            curChar->x = tempx- curChar->w;
        }
    }
    if(curMap->map[ttop] == 3){
        tempy = 0; tempx = 0;
        while(ttop >= curMap->w){
            ttop -= curMap->w;
            tempy++;
        }
        tempx = ttop;
        tempx *= curMap->tilesize;
        tempy *= curMap->tilesize;
        if(curChar->y < (tempy + curMap->tilesize)){
            curChar->y = (tempy + curMap->tilesize);
        }
    }
    if(curMap->map[tbottom] == 3){
        tempy = 0; tempx = 0;
        while(tbottom >= curMap->w){
            tbottom -= curMap->w;
            tempy++;
        }
        tempx = tbottom;
        tempx *= curMap->tilesize;
        tempy *= curMap->tilesize;
        if((curChar->y + curChar->h) > tempy){
            curChar->y = tempy - curChar->h;
        }
    }

    if( curChar->positionx != curChar->positionx_old ||
        curChar->positiony != curChar->positiony_old  ){
        printf("--> posx: %d posy: %d || tleft: %d tright: %d ttop: %d tbottom: %d\n",
        curChar->positionx, curChar->positiony, curMap->map[tleft],
        curMap->map[tright], curMap->map[ttop], curMap->map[tbottom]);
    }

    curChar->positionx_old = curChar->positionx;
    curChar->positiony_old = curChar->positiony;
    curChar->x_old = curChar->x;
    curChar->y_old = curChar->y;
}

void auraDrawChar(_aura_character* curChar){
    g2dEnd();

    curChar->texture =g2dTexLoad("maps/player.png", G2D_SWIZZLE);
    g2dBeginRects(curChar->texture);
    g2dSetCropWH(16, 16);

    /*
        |-------|-------|
        |   up  |  left |
        |-------|-------|
        | right |  down |
        |-------|-------|
    */

    if(curChar->facing == 1){
        g2dSetCropXY(0,0);
    }
    if(curChar->facing == 2){
        g2dSetCropXY(16,16);
    }
    if(curChar->facing == 3){
        g2dSetCropXY(16, 0);
    }
    if(curChar->facing == 4){
        g2dSetCropXY(0, 16);
    }

    g2dSetScaleWH(curChar->w,curChar->h);
    g2dSetCoordXY(curChar->x,curChar->y);
    g2dAdd();
}

void auraGameCycle(){

    SceCtrlData auraPadInput;

    sceCtrlPeekBufferPositive(&auraPadInput,1);

}

void auraDrawGUI(){

}
