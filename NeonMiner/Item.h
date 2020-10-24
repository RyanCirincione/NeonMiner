#pragma once

#include <SDL.h>

#include "Constants.h"
#include "LTexture.h"
#include "Tile.h"

class Item {
public:
	Item(int t, float pX, float pY);
	void update(Tile*** tiles);
	void render(SDL_Renderer* gRenderer, SDL_Rect& camera);

	bool remove;
	float posX;
	float posY;
	int type;
};