#pragma once

#include "Tile.h"
#include "Player.h"

class WallCreeper
{
public:
	WallCreeper(float x, float y, Player player);

	void update(Tile*** tiles, Player player);

	void render(SDL_Renderer* gRenderer, SDL_Rect& camera);

	float posX, posY;

	float rotation;

	bool remove;

private:
	int age;
	float mVelX, mVelY;
};