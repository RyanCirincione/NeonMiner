/*This source code copyrighted by Lazy Foo' Productions (2004-2020)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, strings, and file streams
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <fstream>

#include "Player.h"
#include "Tile.h"
#include "LTexture.h"

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//The dimensions of the level
const int LEVEL_WIDTH = 1280;
const int LEVEL_HEIGHT = 960;

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia(Tile* tiles[]);

//Frees media and shuts down SDL
void close(Tile* tiles[]);

//Sets tiles from tile map
bool setTiles(Tile *tiles[]);

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Scene textures
LTexture gDotTexture;
LTexture gTileTexture;
SDL_Rect gTileClips[TOTAL_TILE_SPRITES];

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
		return success;
	}
	//Set texture filtering to linear
	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
	{
		printf("Warning: Linear texture filtering not enabled!");
	}

	//Create window
	gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (gWindow == NULL)
	{
		printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
		success = false;
		return success;
	}

	//Create renderer for window
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (gRenderer == NULL)
	{
		printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
		success = false;
		return success;
	}

	//Initialize renderer color
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

	//Initialize PNG loading
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
		success = false;
	}

	return success;
}

bool loadMedia(Tile* tiles[])
{
	//Loading success flag
	bool success = true;

	//Load player texture
	if (!gDotTexture.loadFromFile(gRenderer, "assets/player.png"))
	{
		printf("Failed to load player texture!\n");
		success = false;
	}

	//Load tile texture
	if (!gTileTexture.loadFromFile(gRenderer, "assets/tiles.png"))
	{
		printf("Failed to load tile set texture!\n");
		success = false;
	}

	//Load tile map
	if (!setTiles(tiles))
	{
		printf("Failed to load tile set!\n");
		success = false;
	}

	return success;
}

void close(Tile* tiles[])
{
	//Deallocate tiles
	for (int i = 0; i < TOTAL_TILES; ++i)
	{
		if (tiles[i] != NULL)
		{
			delete tiles[i];
			tiles[i] = NULL;
		}
	}

	//Free loaded images
	gDotTexture.free();
	gTileTexture.free();

	//Destroy window	
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

bool setTiles(Tile* tiles[])
{
	//Success flag
	bool tilesLoaded = true;

	//The tile offsets
	int x = 0, y = 0;

	//Open the map
	std::ifstream map("assets/lazy.map");

	//If the map couldn't be loaded
	if (map.fail())
	{
		printf("Unable to load map file!\n");
		tilesLoaded = false;
	}
	else
	{
		//Initialize the tiles
		for (int i = 0; i < TOTAL_TILES; ++i)
		{
			//Determines what kind of tile will be made
			int tileType = -1;

			//Read tile from map file
			map >> tileType;

			//If the was a problem in reading the map
			if (map.fail())
			{
				//Stop loading map
				printf("Error loading map: Unexpected end of file!\n");
				tilesLoaded = false;
				break;
			}

			//If the number is a valid tile number
			if ((tileType >= 0) && (tileType < TOTAL_TILE_SPRITES))
			{
				tiles[i] = new Tile(x, y, tileType);
			}
			//If we don't recognize the tile type
			else
			{
				//Stop loading map
				printf("Error loading map: Invalid tile type at %d!\n", i);
				tilesLoaded = false;
				break;
			}

			//Move to next tile spot
			x += TILE_WIDTH;

			//If we've gone too far
			if (x >= LEVEL_WIDTH)
			{
				//Move back
				x = 0;

				//Move to the next row
				y += TILE_HEIGHT;
			}
		}

		//Clip the sprite sheet
		if (tilesLoaded)
		{
			gTileClips[TILE_RED].x = 0;
			gTileClips[TILE_RED].y = 0;
			gTileClips[TILE_RED].w = TILE_WIDTH;
			gTileClips[TILE_RED].h = TILE_HEIGHT;

			gTileClips[TILE_GREEN].x = 0;
			gTileClips[TILE_GREEN].y = 80;
			gTileClips[TILE_GREEN].w = TILE_WIDTH;
			gTileClips[TILE_GREEN].h = TILE_HEIGHT;

			gTileClips[TILE_BLUE].x = 0;
			gTileClips[TILE_BLUE].y = 160;
			gTileClips[TILE_BLUE].w = TILE_WIDTH;
			gTileClips[TILE_BLUE].h = TILE_HEIGHT;

			gTileClips[TILE_TOPLEFT].x = 80;
			gTileClips[TILE_TOPLEFT].y = 0;
			gTileClips[TILE_TOPLEFT].w = TILE_WIDTH;
			gTileClips[TILE_TOPLEFT].h = TILE_HEIGHT;

			gTileClips[TILE_LEFT].x = 80;
			gTileClips[TILE_LEFT].y = 80;
			gTileClips[TILE_LEFT].w = TILE_WIDTH;
			gTileClips[TILE_LEFT].h = TILE_HEIGHT;

			gTileClips[TILE_BOTTOMLEFT].x = 80;
			gTileClips[TILE_BOTTOMLEFT].y = 160;
			gTileClips[TILE_BOTTOMLEFT].w = TILE_WIDTH;
			gTileClips[TILE_BOTTOMLEFT].h = TILE_HEIGHT;

			gTileClips[TILE_TOP].x = 160;
			gTileClips[TILE_TOP].y = 0;
			gTileClips[TILE_TOP].w = TILE_WIDTH;
			gTileClips[TILE_TOP].h = TILE_HEIGHT;

			gTileClips[TILE_CENTER].x = 160;
			gTileClips[TILE_CENTER].y = 80;
			gTileClips[TILE_CENTER].w = TILE_WIDTH;
			gTileClips[TILE_CENTER].h = TILE_HEIGHT;

			gTileClips[TILE_BOTTOM].x = 160;
			gTileClips[TILE_BOTTOM].y = 160;
			gTileClips[TILE_BOTTOM].w = TILE_WIDTH;
			gTileClips[TILE_BOTTOM].h = TILE_HEIGHT;

			gTileClips[TILE_TOPRIGHT].x = 240;
			gTileClips[TILE_TOPRIGHT].y = 0;
			gTileClips[TILE_TOPRIGHT].w = TILE_WIDTH;
			gTileClips[TILE_TOPRIGHT].h = TILE_HEIGHT;

			gTileClips[TILE_RIGHT].x = 240;
			gTileClips[TILE_RIGHT].y = 80;
			gTileClips[TILE_RIGHT].w = TILE_WIDTH;
			gTileClips[TILE_RIGHT].h = TILE_HEIGHT;

			gTileClips[TILE_BOTTOMRIGHT].x = 240;
			gTileClips[TILE_BOTTOMRIGHT].y = 160;
			gTileClips[TILE_BOTTOMRIGHT].w = TILE_WIDTH;
			gTileClips[TILE_BOTTOMRIGHT].h = TILE_HEIGHT;
		}
	}

	//Close the file
	map.close();

	//If the map was loaded fine
	return tilesLoaded;
}

int main(int argc, char* args[])
{
	//Start up SDL and create window
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		//The level tiles
		Tile* tileSet[TOTAL_TILES];

		//Load media
		if (!loadMedia(tileSet))
		{
			printf("Failed to load media!\n");
		}
		else
		{
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//The player that will be moving around on the screen
			Player player;

			//Level camera
			SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

			//While application is running
			while (!quit)
			{
				//Handle events on queue
				while (SDL_PollEvent(&e) != 0)
				{
					//User requests quit
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}

					//Handle input for the player
					player.handleEvent(e);
				}

				//Move the player
				player.move(tileSet);
				player.setCamera(camera);

				//Clear screen
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);

				//Render level
				for (int i = 0; i < TOTAL_TILES; ++i)
				{
					tileSet[i]->render(gRenderer, camera, gTileTexture, gTileClips);
				}

				//Render player
				player.render(gRenderer, camera, gDotTexture);
				//SDL_RenderCopy(gRenderer, gDotTexture.mTexture, new SDL_Rect{ 0,0,20,20 }, new SDL_Rect{ 100,100,120,120 });

				//Update screen
				SDL_RenderPresent(gRenderer);
			}
		}

		//Free resources and close SDL
		close(tileSet);
	}

	return 0;
}