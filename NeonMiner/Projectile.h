#pragma once

#include "Tile.h"
#include "Item.h"

class Projectile {
public:
	Projectile(int x, int y, int r);

	void update(Tile*** tiles, std::vector<Item*>* items);

	void render(SDL_Renderer* gRenderer, SDL_Rect& camera, LTexture* gProjectileTexture);

	bool remove;

private:
	SDL_Rect hitbox;

	int velX, velY;
	float rotation;
};