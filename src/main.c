/*
* @Author: ich
* @Date:   2015-12-25 02:10:03
* @Last Modified by:   smithsf0x
* @Last Modified time: 2016-01-18 21:41:33
*/

#include "main.h"


#define AURORA_VERSION    0 //Talk about this
#define AURORA_REVISION    1

PSP_MODULE_INFO("aurora ", 0, AURORA_VERSION, AURORA_REVISION);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER);
PSP_HEAP_SIZE_MAX();

int main(){

    auraInit();

	setupExitCallback();


    _aura_map* mapIsland = auraLoadMap("island", 2, 40, 20, 16);

    curMap = mapIsland;

    _aura_character* player = malloc(sizeof(_aura_character));

    g2dTexture* tex = malloc(sizeof(g2dTexture));
    player->texture = tex;
    player->x = curMap->tilesize;
    player->y = curMap->tilesize;
    player->w = curMap->tilesize;
    player->h = curMap->tilesize;

    float offx = 0;
    float offy = 0;

	while (isRunning()){

        auraStartCycle();

        auraGetInput();

        g2dClear(WHITE);

        auraMovePlayer( player);

        if(player->facing == 1){
            player->x = floor(player->x);
        }
        if(player->facing == 2){
            player->x = ceil(player->x);
        }
        if(player->facing == 3){
            player->y = floor(player->y);
        }
        if(player->facing == 4){
            player->y = ceil(player->y);
        }



        if(auraPadInput.Ly < 64){
            // UP
            //printf("--> UP\n");
            offy += 2 * dt;
        }
        if(auraPadInput.Ly > 192){
            // Down
            //printf("--> DOWN\n");
            offy -= 2 * dt;
        }
        if(auraPadInput.Lx < 64){
            // LEFT
            //printf("--> LEFT\n");
            offx += 2 * dt;
        }
        if(auraPadInput.Lx > 192){
            // RIGHT
            //printf("--> RIGHT\n");
            offx -= 2 * dt;
        }

        auraDrawMap(curMap, 1, offx, offy);
        auraDrawMap(curMap, 2, offx, offy);

        auraDrawChar( player);
        g2dEnd();

        g2dFlip(G2D_VSYNC);

        auraEndCycle();

    }

    g2dTerm();
	sceKernelExitGame();
	return 0;
}
