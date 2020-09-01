#include "Item.h"

#include <cmath>

Item::Item(LTexture* tx, float pX, float pY) {
	texture = tx;
	posX = pX;
	posY = pY;

	remove = false;
}

void Item::update(Tile* tiles[LEVEL_WIDTH / TILE_WIDTH][LEVEL_HEIGHT / TILE_HEIGHT]) {
}

void Item::render(SDL_Renderer* gRenderer, SDL_Rect& camera) {
	texture->render(gRenderer, posX - camera.x, posY - camera.y);
}