#pragma once

#include <SDL.h>

#include "Constants.h"
#include "LTexture.h"
#include "Tile.h"

class Item {
public:
	Item(LTexture* tx, float pX, float pY);
	void update(Tile* tiles[LEVEL_WIDTH / TILE_WIDTH][LEVEL_HEIGHT / TILE_HEIGHT]);
	void render(SDL_Renderer* gRenderer, SDL_Rect& camera);

	bool remove;
	float posX;
	float posY;

private:
	LTexture* texture;
};