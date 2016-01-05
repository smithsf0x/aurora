# aurora

####A TileEngine made with glib2D for the PlayStation Portable
[![Build Status](https://travis-ci.org/smithsf0x/aurora.svg?branch=master)](https://travis-ci.org/smithsf0x/aurora)

##Features
- up to 3 Layers per Map
- Universal Player/NPC class
- DeltaTime

##Folder structures

Example: 

```c
|-- EBOOT.PBP
|-- maps/
	|-- island/
		|-- island.png
		|-- island_l1.csv
		|-- island_l2.csv
	|-- house_1/
		|-- house_1.png
		|-- house_1.csv
|-- assets/
	|-- player/
		|-- player.png
	|-- npc/
		|-- guard/
			|-- guard.png
		|-- witch/
			|-- witch.png
			|-- witch_2.png
|-- sounds/
```

###Maps
Path: `maps/nameOfMap`

Every map has an `.png` and at least one `.csv` file. The name of the tileset and data file is just the name of the map e.g: `nameOfMap.png` and `nameOfMap_l1.csv`, `nameOfMap_l2.csv`,`nameOfMap_l3.csv` ( layer 2 and 3 are optional). 

###Character


##Usage

Example: 

```c
int main(){

	auraInit();
	
	setupExitCallback();
	
	_aura_map* mapIsland = auraLoadMap("island", 2, 40, 20, 16);
	_aura_character* player = malloc(sizeof(_aura_character));
	
	curMap = mapIsland;
	
	player->texture = malloc(sizeof(g2dTexture));    
	
	while (isRunning()){
		auraStartCycle();
		
		auraGetInput();
		
		g2dClear(WHITE);
		
		// Draw Map Layer 1
		auraDrawMap(curMap, 1, 0, 0);
		// Draw Map Layer 2
		auraDrawMap(curMap, 2, 0, 0);
		
		auraMoveChar( player);
		auraDrawChar( player);
		    
		g2dEnd();
		
		g2dFlip(G2D_VSYNC);
		
		auraEndCycle();
	}
	
	g2dTerm();
	sceKernelExitGame();
	return 0;
}
```

##Bugs
- Diagonal player movement only right/down and up/left


##Building
- Copy the aurora-folder to your source directory
-  Add ` aurora/aurora.o` and `glib2d/glib2d.o` to your objects and link `-lpng -ljpeg -lz -lpspgu -lm -lpspvram` in your Makefile

##License

Using gLib2D by Geecko
