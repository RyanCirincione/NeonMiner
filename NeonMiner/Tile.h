#pragma once

#include <SDL.h>

#include "LTexture.h"

//Tile constants
const int TILE_WIDTH = 80;
const int TILE_HEIGHT = 80;
const int TOTAL_TILES = 192;
const int TOTAL_TILE_SPRITES = 12;

//The different tile sprites
const int TILE_RED = 0;
const int TILE_GREEN = 1;
const int TILE_BLUE = 2;
const int TILE_CENTER = 3;
const int TILE_TOP = 4;
const int TILE_TOPRIGHT = 5;
const int TILE_RIGHT = 6;
const int TILE_BOTTOMRIGHT = 7;
const int TILE_BOTTOM = 8;
const int TILE_BOTTOMLEFT = 9;
const int TILE_LEFT = 10;
const int TILE_TOPLEFT = 11;

class Tile
{
public:
	//Initializes position and type
	Tile(int x, int y, int tileType);

	//Shows the tile
	void render(SDL_Renderer* gRenderer, SDL_Rect& camera, LTexture gTileTexture, SDL_Rect gTileClips[]);

	//Get the tile type
	int getType();

	//Get the collision box
	SDL_Rect getBox();

	static bool checkCollision(SDL_Rect a, SDL_Rect b);
	static bool touchesWall(SDL_Rect box, Tile* tiles[]);

private:
	//The attributes of the tile
	SDL_Rect mBox;

	//The tile type
	int mType;
};