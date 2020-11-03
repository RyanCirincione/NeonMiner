/*This source code copyrighted by Lazy Foo' Productions (2004-2020)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, strings, and file streams
#include <algorithm>
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

#include "WallCreeper.h"
#include "Chunk.h"
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
bool loadMedia(Tile*** tiles);

//Frees media and shuts down SDL
void close(Tile*** tiles);

//Sets tiles from tile map
bool setTiles(Tile*** tiles);

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

void getPixel(SDL_Surface *surface, int x, int y, Uint8* r, Uint8* g, Uint8* b)
{
	// This arcane magic comes from stack overflow

	int bpp = surface->format->BytesPerPixel;
	/* Here p is the address to the pixel we want to retrieve */
	Uint32 result = 0;
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

	switch (bpp)
	{
	case 1:
		result = *p;
		break;

	case 2:
		result = *(Uint16 *)p;
		break;

	case 3:
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
			result = p[0] << 16 | p[1] << 8 | p[2];
		else
			result = p[0] | p[1] << 8 | p[2] << 16;
		break;

	case 4:
		result = *(Uint32 *)p;
		break;

	default:
		result = 0;
	}

	SDL_GetRGB(result, surface->format, r, g, b);
}

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

bool loadMedia(Tile*** tiles)
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

void close(Tile*** tiles)
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
		delete tiles[i];
	}
	delete tiles;

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

void diamondStep(float** heightMap, int x, int y, int radius, float randomVariation) {
	float a = x - radius < 0 || y - radius < 0 ? 0 : heightMap[x - radius][y - radius];
	float b = x + radius >= LEVEL_WIDTH / TILE_WIDTH || y - radius < 0 ? 0 : heightMap[x + radius][y - radius];
	float c = x + radius >= LEVEL_WIDTH / TILE_WIDTH || y + radius >= LEVEL_HEIGHT / TILE_HEIGHT ? 0 : heightMap[x + radius][y + radius];
	float d = x - radius < 0 || y + radius >= LEVEL_WIDTH / TILE_HEIGHT ? 0 : heightMap[x - radius][y + radius];
	float midpoint = (a + b + c + d) / 4 + ((rand() % 1000) / 1000.0 - 0.5) * randomVariation;
	heightMap[x][y] = std::min(1.0f, std::max(0.0f, midpoint));
}

void squareStep(float** heightMap, int x, int y, int radius, float randomVariation) {
	float a = y - radius < 0 ? 0 : heightMap[x][y - radius];
	float b = x + radius >= LEVEL_WIDTH / TILE_WIDTH ? 0 : heightMap[x + radius][y];
	float c = y + radius >= LEVEL_HEIGHT / TILE_HEIGHT ? 0 : heightMap[x][y + radius];
	float d = x - radius < 0 ? 0 : heightMap[x - radius][y];
	float midpoint = (a + b + c + d) / 4 + ((rand() % 1000) / 1000.0 - 0.5) * randomVariation;
	heightMap[x][y] = std::min(1.0f, std::max(0.0f, midpoint));
}

bool setTiles(Tile*** tiles)
{
	//Success flag
	bool tilesLoaded = true;

	//Initialize the tiles
	//The tile offsets
	int x = 0, y = 0;
	for (int i = 0; i < LEVEL_WIDTH / TILE_WIDTH; ++i)
	{
		for (int j = 0; j < LEVEL_HEIGHT / TILE_HEIGHT; j++) {
			float dist2FromCenter = pow(i - LEVEL_WIDTH / TILE_WIDTH / 2, 2) + pow(j - LEVEL_HEIGHT / TILE_HEIGHT / 2, 2);
			float chance = sqrt(dist2FromCenter) / sqrt(pow(LEVEL_WIDTH / TILE_WIDTH / 2, 2) + pow(LEVEL_HEIGHT / TILE_HEIGHT / 2, 2)) * 100;
			tiles[i][j] = new Tile(x, y, TILE_SPACE);

			//Move to next tile spot
			y += TILE_WIDTH;
		}
		//Move back
		y = 0;

		//Move to the next row
		x += TILE_HEIGHT;
	}

	//Use Diamond-Square with the painted map
	float** heightMap = (float**)malloc(LEVEL_WIDTH / TILE_WIDTH * sizeof(float*));
	for (int i = 0; i < LEVEL_WIDTH / TILE_WIDTH; i++) {
		heightMap[i] = (float*)malloc(LEVEL_HEIGHT / TILE_HEIGHT * sizeof(float));
	}

	SDL_Surface* mapSurface = IMG_Load("worldgen/maps/map1.png");
	SDL_Color pixelColor;
	for (int i = 0; i < 64; i++) {
		for (int j = 0; j < 64; j++) {
			getPixel(mapSurface, i, j, &pixelColor.r, &pixelColor.g, &pixelColor.b);
			heightMap[i*16][j*16] = pixelColor.r / 255.0;
		}
	}

	// Iteratively apply Diamond, then Square, then shrink and repeat
	for (int radius = 8; radius > 0; radius /= 2) {
		// Diamond
		for (int x = radius; x < LEVEL_WIDTH / TILE_WIDTH; x += radius * 2) {
			for (int y = radius; y < LEVEL_HEIGHT / TILE_HEIGHT; y += radius * 2) {
				diamondStep(heightMap, x, y, radius, radius / 8.0);
			}
		}
		// Square
		for (int x = radius; x < LEVEL_WIDTH / TILE_WIDTH; x += radius * 2) {
			for (int y = 0; y < LEVEL_HEIGHT / TILE_HEIGHT; y += radius * 2) {
				squareStep(heightMap, x, y, radius, radius / 8.0);
			}
		}
		for (int y = radius; y < LEVEL_HEIGHT / TILE_HEIGHT; y += radius * 2) {
			for (int x = 0; x < LEVEL_WIDTH / TILE_WIDTH; x += radius * 2) {
				squareStep(heightMap, x, y, radius, radius / 8.0);
			}
		}
	}

	// Convert the heightmap to the terrain
	for (int x = 0; x < LEVEL_WIDTH / TILE_WIDTH; x++) {
		for (int y = 0; y < LEVEL_HEIGHT / TILE_HEIGHT; y++) {
			tiles[x][y]->setType(heightMap[x][y] < WORLDGEN_THRESHOLD ? TILE_WALL : TILE_SPACE);
		}
	}

	// Smooth the noise
	for (int iterations = 0; iterations < 1; iterations++) {
		int** smoothTiles = (int**)malloc(LEVEL_WIDTH / TILE_WIDTH * sizeof(int*));

		for (int i = 0; i < LEVEL_WIDTH / TILE_WIDTH; ++i)
		{
			smoothTiles[i] = (int*)malloc(LEVEL_HEIGHT / TILE_HEIGHT * sizeof(int));

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


				smoothTiles[i][j] = space > wall + 1 ? TILE_SPACE : TILE_WALL;
			}
		}
		for (int i = 0; i < LEVEL_WIDTH / TILE_WIDTH; ++i)
		{
			for (int j = 0; j < LEVEL_HEIGHT / TILE_HEIGHT; j++) {
				tiles[i][j]->setType(smoothTiles[i][j]);
			}
			delete smoothTiles[i];
		}
		delete smoothTiles;
	}

	//Insert ore
	for (int i = 0; i < ORE_PATCH_ATTEMPTS; i++) {
		int centerX = rand() % (LEVEL_WIDTH / TILE_WIDTH), centerY = rand() % (LEVEL_HEIGHT / TILE_HEIGHT);
		int oreType = rand() % 3 + 2; // TODO Properly select from available ore types
		for (int j = 0; j < ORE_ATTEMPTS_PER_PATCH; j++) {
			int x = centerX + rand() % (ORE_PATCH_RANGE * 2 + 1) - ORE_PATCH_RANGE + rand() % (ORE_PATCH_RANGE * 2 + 1) - ORE_PATCH_RANGE;
			x = std::max(0, std::min(LEVEL_WIDTH / TILE_WIDTH - 1, x));
			int y = centerY + rand() % (ORE_PATCH_RANGE * 2 + 1) - ORE_PATCH_RANGE + rand() % (ORE_PATCH_RANGE * 2 + 1) - ORE_PATCH_RANGE;
			y = std::max(0, std::min(LEVEL_HEIGHT / TILE_HEIGHT - 1, y));
			if (tiles[x][y]->getType() == TILE_WALL) {
				tiles[x][y]->setType(oreType);
			}
		}
	}

	//If the map was loaded fine
	return tilesLoaded;
}

int getCurrentClockSegment(std::vector<std::pair<int, int>>* clockSegments, int time) {
	for (auto p : *clockSegments) {
		// sin wants radians
		time -= p.second;
		if (time < 0) {
			return p.first;
		}
	}

	printf("Bad input given to getCurrentClockSegment().");
	return -1;
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
		Tile*** tileSet = (Tile***)malloc(LEVEL_WIDTH / TILE_WIDTH * sizeof(Tile**));// [LEVEL_WIDTH / TILE_WIDTH][LEVEL_HEIGHT / TILE_HEIGHT];
		for (int i = 0; i < LEVEL_WIDTH / TILE_WIDTH; i++) {
			tileSet[i] = (Tile**)malloc(LEVEL_HEIGHT / TILE_HEIGHT * sizeof(Tile*));
			for (int j = 0; j < LEVEL_HEIGHT / TILE_HEIGHT; j++) {
				tileSet[i][j] = (Tile*)malloc(sizeof(Tile));
			}
		}

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
			std::vector<WallCreeper*> wallCreepers;

			int time = 2600, clockDuration = 0;
			std::vector<std::pair<int, int>> clockSegments;
			clockSegments.push_back(std::make_pair(CLOCK_CALM, 1800));
			clockSegments.push_back(std::make_pair(CLOCK_RISING, 900));
			clockSegments.push_back(std::make_pair(CLOCK_DANGER, 600));
			clockSegments.push_back(std::make_pair(CLOCK_RISING, 480));
			clockSegments.push_back(std::make_pair(CLOCK_CALM, 900));
			clockSegments.push_back(std::make_pair(CLOCK_RISING, 360));
			for (auto p : clockSegments) {
				clockDuration += p.second;
			}

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

				// Spawn Wall Creepers during danger phase
				if (getCurrentClockSegment(&clockSegments, time % clockDuration) == CLOCK_DANGER && rand() % 50 == 0) {
					int x = player.posX / TILE_WIDTH + rand() % 21 - 10;
					int y = player.posY / TILE_HEIGHT + rand() % 21 - 10;
					x = std::min(LEVEL_WIDTH / TILE_WIDTH, std::max(0, x));
					y = std::min(LEVEL_HEIGHT / TILE_HEIGHT, std::max(0, y));

					if (tileSet[x][y]->getType() != TILE_SPACE) {
						wallCreepers.push_back(new WallCreeper(x * TILE_WIDTH, y * TILE_HEIGHT, player));
					}
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

				//Update wall creepers
				for (auto c : wallCreepers) {
					c->update(tileSet, player);
				}

				projectiles.erase(std::remove_if(projectiles.begin(), projectiles.end(), [](Projectile* p) {return p->remove; }), projectiles.end());
				particles.erase(std::remove_if(particles.begin(), particles.end(), [](Particle* p) {return p->lifetime <= 0; }), particles.end());
				items.erase(std::remove_if(items.begin(), items.end(), [](Item* i) {return i->remove; }), items.end());
				wallCreepers.erase(std::remove_if(wallCreepers.begin(), wallCreepers.end(), [](WallCreeper* c) {return c->remove; }), wallCreepers.end());

				//Clear screen
				SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
				SDL_RenderClear(gRenderer);

				SDL_Rect tileBox;
				SDL_Rect tileClip = SDL_Rect{ 0, 0, 64, 64 };

				//Render level
				for (int i = camera.x / TILE_WIDTH - 8; i < (camera.x + camera.w) / TILE_WIDTH + 8; ++i)
				{
					for (int j = camera.y / TILE_HEIGHT - 8; j < (camera.y + camera.h) / TILE_HEIGHT + 8; j++) {
						tileBox = tileSet[i][j]->getBox();
						tileBox.x += tileBox.w / 2 - camera.x;
						tileBox.y += tileBox.h / 2 - camera.y;

						tileClip.x = 0;
						tileClip.x += tileSet[i][j]->getType() != TILE_SPACE ? 64 : 0;
						tileClip.x += (j + 1 >= LEVEL_HEIGHT / TILE_HEIGHT) || (tileSet[i][j + 1]->getType() != TILE_SPACE) ? 128 : 0;

						tileClip.y = 0;
						tileClip.y += (i + 1 >= LEVEL_WIDTH / TILE_WIDTH) || (j + 1 >= LEVEL_HEIGHT / TILE_HEIGHT) || (tileSet[i + 1][j + 1]->getType() != TILE_SPACE) ? 64 : 0;
						tileClip.y += (i + 1 >= LEVEL_WIDTH / TILE_WIDTH) || (tileSet[i + 1][j]->getType() != TILE_SPACE) ? 128 : 0;

						WALL_SPRITES_TXT->render(gRenderer, tileBox, &tileClip);

						//Add particles for ore tiles
						switch (tileSet[i][j]->getType()) {
						case TILE_RED_ORE:
							if (rand() % 17 <= 0) {
								particles.push_back(new Particle(RED_SPARKLE_TXT, rand() % 8 + 24,
									tileBox.x + camera.x, tileBox.y + camera.y, (rand() % 38) / 25.0 - 0.75, (rand() % 38) / 25.0 - 0.75));
							}
							break;
						case TILE_GREEN_ORE:
							if (rand() % 17 <= 0) {
								particles.push_back(new Particle(GREEN_SPARKLE_TXT, rand() % 8 + 24,
									tileBox.x + camera.x, tileBox.y + camera.y, (rand() % 38) / 25.0 - 0.75, (rand() % 38) / 25.0 - 0.75));
							}
							break;
						case TILE_BLUE_ORE:
							if (rand() % 17 <= 0) {
								particles.push_back(new Particle(BLUE_SPARKLE_TXT, rand() % 8 + 24,
									tileBox.x + camera.x, tileBox.y + camera.y, (rand() % 38) / 25.0 - 0.75, (rand() % 38) / 25.0 - 0.75));
							}
							break;
						case TILE_SPACE:
						case TILE_WALL:
						default:
							break;
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

				//Render wall creepers
				for (auto c : wallCreepers) {
					c->render(gRenderer, camera);
				}

				//Render player
				player.render(gRenderer, camera, PLAYER_TXT);

				//Draw the clock
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				int clockX = SCREEN_WIDTH - 60, clockY = 60, clockR = 40;
				int runningTotal = 0;
				for (auto p : clockSegments) {
					runningTotal += p.second;
					double angle = (double)runningTotal / clockDuration * 2 * M_PI;
					SDL_RenderDrawLine(gRenderer, clockX + clockR / 2 * cos(angle), clockY + clockR / 2 * sin(angle), clockX + clockR * cos(angle), clockY + clockR * sin(angle));
				}
				double timeAngle = (double)(time % clockDuration) / clockDuration * 2 * M_PI;
				SDL_RenderDrawLine(gRenderer, clockX, clockY, clockX + clockR * cos(timeAngle), clockY + clockR * sin(timeAngle));

				//Update screen
				SDL_RenderPresent(gRenderer);

				time++;
			}
		}

		//Free resources and close SDL
		close(tileSet);
	}

	return 0;
}