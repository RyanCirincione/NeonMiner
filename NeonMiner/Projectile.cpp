#include "Projectile.h"
#include "Constants.h"

#include <cmath>

Projectile::Projectile(int x, int y, int r) {
	hitbox.x = x;
	hitbox.y = y;
	hitbox.w = PROJECTILE_WIDTH;
	hitbox.h = PROJECTILE_HEIGHT;

	rotation = r;

	velX = cos(rotation * M_PI / 180.0) * PROJECTILE_SPEED;
	velY = sin(rotation * M_PI / 180.0) * PROJECTILE_SPEED;

	remove = false;
}

void Projectile::update(Tile * tiles[LEVEL_WIDTH / TILE_WIDTH][LEVEL_HEIGHT / TILE_HEIGHT], std::vector<Item*>* items) {
	hitbox.x += velX;
	hitbox.y += velY;

	if (Tile::touchesWall(hitbox, tiles))
	{
		for (int i = -1; i <= 1; i++) {
			for (int j = -1; j <= 1; j++) { // TODO verify on edge of screen
				Tile* tile = tiles[hitbox.x / TILE_WIDTH + i][hitbox.y / TILE_HEIGHT + j];
				int itemType;

				switch (tile->getType()) {
				case TILE_RED_ORE:
					itemType = ITEM_RED_ORE;
					break;
				case TILE_GREEN_ORE:
					itemType = ITEM_GREEN_ORE;
					break;
				case TILE_BLUE_ORE:
					itemType = ITEM_BLUE_ORE;
					break;
				default:
					itemType = -1;
					break;
				}
				tile->setType(TILE_SPACE);
			}
		}

		remove = true;
	}
}

void Projectile::render(SDL_Renderer* gRenderer, SDL_Rect& camera, LTexture* gProjectileTexture) {
	gProjectileTexture->render(gRenderer, hitbox.x - camera.x, hitbox.y - camera.y, new SDL_Rect{ 0, 0, PROJECTILE_WIDTH, PROJECTILE_HEIGHT }, rotation);
}


