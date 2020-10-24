#pragma once

#include <SDL.h>
#include <vector>

#include "Constants.h"
#include "LTexture.h"

class Tile
{
public:
	//Initializes position and type
	Tile(int x, int y, int tileType);

	//Shows the tile
	void render(SDL_Renderer* gRenderer, SDL_Rect& camera, LTexture* gTileTexture, SDL_Rect gTileClips[]);

	//Get the tile type
	int getType();
	void setType(int type);

	//Get the collision box
	SDL_Rect getBox();

	static bool checkCollision(SDL_Rect a, SDL_Rect b);
	static bool touchesWall(SDL_Rect box, Tile*** tiles);

private:
	//The attributes of the tile
	SDL_Rect mBox;

	//The tile type
	int mType;
};