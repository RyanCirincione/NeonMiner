/*This source code copyrighted by Lazy Foo' Productions (2004-2020)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, strings, and file streams
#include <algorithm>
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <fstream>
#include <vector>

#include "Constants.h"
#include "Player.h"
#include "Tile.h"
#include "LTexture.h"
#include "Projectile.h"
#include "Particle.h"
#include "Item.h"

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia(Tile* tiles[LEVEL_WIDTH / TILE_WIDTH][LEVEL_HEIGHT / TILE_HEIGHT]);

//Frees media and shuts down SDL
void close(Tile* tiles[LEVEL_WIDTH / TILE_WIDTH][LEVEL_HEIGHT / TILE_HEIGHT]);

//Sets tiles from tile map
bool setTiles(Tile *tiles[LEVEL_WIDTH / TILE_WIDTH][LEVEL_HEIGHT / TILE_HEIGHT]);

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

bool init()
{
	//Initialization flag
	bool success = true;

	srand(time(NULL));

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

bool loadMedia(Tile* tiles[LEVEL_WIDTH / TILE_WIDTH][LEVEL_HEIGHT / TILE_HEIGHT])
{
	//Loading success flag
	bool success = true;

	if (!LTexture::loadTextures(gRenderer)) {
		return false;
	}

	//Load tile map
	if (!setTiles(tiles))
	{
		printf("Failed to load tile set!\n");
		success = false;
	}

	return success;
}

void close(Tile* tiles[LEVEL_WIDTH / TILE_WIDTH][LEVEL_HEIGHT / TILE_HEIGHT])
{
	//Deallocate tiles
	for (int i = 0; i < LEVEL_WIDTH / TILE_WIDTH; ++i)
	{
		for (int j = 0; j < LEVEL_HEIGHT / TILE_HEIGHT; j++) {
			if (tiles[i][j] != NULL)
			{
				delete tiles[i][j];
				tiles[i][j] = NULL;
			}
		}
	}



	//Free loaded images
	LTexture::freeTextures();

	//Destroy window	
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

bool setTiles(Tile* tiles[LEVEL_WIDTH / TILE_WIDTH][LEVEL_HEIGHT / TILE_HEIGHT])
{
	//Success flag
	bool tilesLoaded = true;

	//The tile offsets
	int x = 0, y = 0;

	//Initialize the tiles
	for (int i = 0; i < LEVEL_WIDTH / TILE_WIDTH; ++i)
	{
		for (int j = 0; j < LEVEL_HEIGHT / TILE_HEIGHT; j++) {
			float dist2FromCenter = pow(i - LEVEL_WIDTH / TILE_WIDTH / 2, 2) + pow(j - LEVEL_HEIGHT / TILE_HEIGHT / 2, 2);
			float chance = sqrt(dist2FromCenter) / sqrt(pow(LEVEL_WIDTH / TILE_WIDTH / 2, 2) + pow(LEVEL_HEIGHT / TILE_HEIGHT / 2, 2)) * 100;
			tiles[i][j] = new Tile(x, y, (rand() % 100 < chance) ? TILE_WALL : TILE_SPACE);

			//Move to next tile spot
			y += TILE_WIDTH;
		}
		//Move back
		y = 0;

		//Move to the next row
		x += TILE_HEIGHT;
	}

	//Smooth the noise
	for (int iterations = 0; iterations < 2; iterations++) {
		int smoothTiles[LEVEL_WIDTH / TILE_WIDTH][LEVEL_HEIGHT / TILE_HEIGHT];
		for (int i = 0; i < LEVEL_WIDTH / TILE_WIDTH; ++i)
		{
			for (int j = 0; j < LEVEL_HEIGHT / TILE_HEIGHT; j++) {
				int space = 0;
				int wall = 0;
				for (int x = -1; x <= 1; x++) {
					for (int y = -1; y <= 1; y++) {
						if (i + x < 0 || i + x >= LEVEL_WIDTH / TILE_WIDTH || j + y < 0 || j + y >= LEVEL_HEIGHT / TILE_HEIGHT) {
							wall++;
							continue;
						}

						if (tiles[i + x][j + y]->getType() == TILE_SPACE) {
							space++;
						}
						else if (tiles[i + x][j + y]->getType() == TILE_WALL) {
							wall++;
						}
					}
				}

				smoothTiles[i][j] = space > wall ? TILE_SPACE : TILE_WALL;
			}
		}
		for (int i = 0; i < LEVEL_WIDTH / TILE_WIDTH; ++i)
		{
			for (int j = 0; j < LEVEL_HEIGHT / TILE_HEIGHT; j++) {
				tiles[i][j]->setType(smoothTiles[i][j]);
			}
		}
	}

	//Insert ore
	for (int i = 0; i < LEVEL_WIDTH / TILE_WIDTH; ++i)
	{
		for (int j = 0; j < LEVEL_HEIGHT / TILE_HEIGHT; j++) {
			if (tiles[i][j]->getType() == TILE_WALL && rand() % 1000 < 3) {
				tiles[i][j]->tags.push_back(rand() % 3);
			}
		}
	}

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
		Tile* tileSet[LEVEL_WIDTH / TILE_WIDTH][LEVEL_HEIGHT / TILE_HEIGHT];

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

			std::vector<Projectile*> projectiles;
			std::vector<Particle*> particles;
			std::vector<Item*> items;

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
					player.handleEvent(e, &projectiles);
				}

				//Move the player
				player.move(tileSet, camera, items);
				player.setCamera(camera);

				//Update particles
				for (auto p : particles) {
					p->update();
				}

				//Update projectiles
				for (auto p : projectiles) {
					p->update(tileSet, &items);
				}

				//Update items
				for (auto i : items) {
					i->update(tileSet);
				}

				projectiles.erase(std::remove_if(projectiles.begin(), projectiles.end(), [](Projectile* p) {return p->remove; }), projectiles.end());
				particles.erase(std::remove_if(particles.begin(), particles.end(), [](Particle* p) {return p->lifetime <= 0; }), particles.end());
				items.erase(std::remove_if(items.begin(), items.end(), [](Item* i) {return i->remove; }), items.end());

				//Clear screen
				SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
				SDL_RenderClear(gRenderer);

				SDL_Rect tileBox;
				SDL_Rect tileClip = SDL_Rect{ 0, 0, 64, 64 };

				//Render level
				for (int i = 0; i < LEVEL_WIDTH / TILE_WIDTH; ++i)
				{
					for (int j = 0; j < LEVEL_HEIGHT / TILE_HEIGHT; j++) {
						//If the tile is on screen
						if (Tile::checkCollision(camera, tileSet[i][j]->getBox()))
						{
							tileBox = tileSet[i][j]->getBox();
							tileBox.x += tileBox.w / 2 - camera.x;
							tileBox.y += tileBox.h / 2 - camera.y;

							tileClip.x = 0;
							tileClip.x += tileSet[i][j]->getType() == 1 ? 64 : 0;
							tileClip.x += (j + 1 >= LEVEL_HEIGHT / TILE_HEIGHT) || (tileSet[i][j + 1]->getType() == 1) ? 128 : 0;

							tileClip.y = 0;
							tileClip.y += (i + 1 >= LEVEL_WIDTH / TILE_WIDTH) || (j + 1 >= LEVEL_HEIGHT / TILE_HEIGHT) || (tileSet[i + 1][j + 1]->getType() == 1) ? 64 : 0;
							tileClip.y += (i + 1 >= LEVEL_WIDTH / TILE_WIDTH) || (tileSet[i + 1][j]->getType() == 1) ? 128 : 0;

							WALL_SPRITES_TXT->render(gRenderer, tileBox, &tileClip);

							//Add particles for ore tiles
							if (rand() % 17 <= 0 && tileSet[i][j]->tags.size() > 0) {
								LTexture* texture;
								if (tileSet[i][j]->tags.at(0) == RED_ORE) {
									texture = RED_SPARKLE_TXT;
								}
								else if(tileSet[i][j]->tags.at(0) == GREEN_ORE) {
									texture = GREEN_SPARKLE_TXT;
								}
								else {//if(tileSet[i][j]->tags.at(0) == BLUE_ORE) {
									texture = BLUE_SPARKLE_TXT;
								}

								particles.push_back(new Particle(texture, rand() % 8 + 24, tileBox.x + camera.x, tileBox.y + camera.y, (rand() % 38) / 25.0 - 0.75, (rand() % 38) / 25.0 - 0.75));
							}
						}
					}
				}

				//Render particles
				for (auto p : particles) {
					p->render(gRenderer, camera);
				}

				//Render projectiles
				for (auto p : projectiles) {
					p->render(gRenderer, camera, PROJECTILE_TXT);
				}

				//Render items
				for (auto i : items) {
					i->render(gRenderer, camera);
				}

				//Render player
				player.render(gRenderer, camera, PLAYER_TXT);

				//Update screen
				SDL_RenderPresent(gRenderer);
			}
		}

		//Free resources and close SDL
		close(tileSet);
	}

	return 0;
}