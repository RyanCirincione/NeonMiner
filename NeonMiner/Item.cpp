#include "Item.h"

#include <cmath>

Item::Item(int t, float pX, float pY) {
	type = t;
	posX = pX;
	posY = pY;

	remove = false;
}

void Item::update(Tile* tiles[LEVEL_WIDTH / TILE_WIDTH][LEVEL_HEIGHT / TILE_HEIGHT]) {
}

void Item::render(SDL_Renderer* gRenderer, SDL_Rect& camera) {
	LTexture* texture;
	switch (type)
	{
	case TILE_RED_ORE:
		texture = RED_ORE_TXT;
		break;
	case TILE_GREEN_ORE:
		texture = GREEN_ORE_TXT;
		break;
	case TILE_BLUE_ORE:
		texture = BLUE_ORE_TXT;
		break;
	default:
		texture = RED_ORE_TXT;
		break;
	}
	texture->render(gRenderer, posX - camera.x, posY - camera.y);
}