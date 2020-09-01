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

void Projectile::update(Tile * tiles[LEVEL_WIDTH / TILE_WIDTH][LEVEL_HEIGHT / TILE_HEIGHT], std::vector<Item>* items) {
	hitbox.x += velX;
	hitbox.y += velY;

	if (Tile::touchesWall(hitbox, tiles))
	{
		for (int i = -1; i <= 1; i++) {
			for (int j = -1; j <= 1; j++) {
				tiles[hitbox.x / TILE_WIDTH + i][hitbox.y / TILE_HEIGHT + j]->setType(TILE_SPACE); // TODO verify on edge of screen
				if (tiles[hitbox.x / TILE_WIDTH + i][hitbox.y / TILE_HEIGHT + j]->tags.size() > 0) {
					LTexture* texture;
					switch (tiles[hitbox.x / TILE_WIDTH + i][hitbox.y / TILE_HEIGHT + j]->tags.at(0)) // TODO properly read ORE tag
					{
					RED_ORE:
						texture = RED_ORE_TXT;
						break;
					GREEN_ORE:
						texture = GREEN_ORE_TXT;
						break;
					BLUE_ORE:
						texture = BLUE_ORE_TXT;
						break;
					default:
						texture = RED_ORE_TXT;
						break;
					}
					items->push_back(*new Item(texture, (hitbox.x / TILE_WIDTH + i) * TILE_WIDTH + rand() % (TILE_WIDTH * 2) - TILE_WIDTH / 2,
						(hitbox.y / TILE_HEIGHT + j) * TILE_HEIGHT + rand() % (TILE_HEIGHT * 2) - TILE_HEIGHT / 2));
					tiles[hitbox.x / TILE_WIDTH + i][hitbox.y / TILE_HEIGHT + j]->tags.pop_back(); // TODO properly remove the ORE tag
				}
			}
		}

		remove = true;
	}
}

void Projectile::render(SDL_Renderer* gRenderer, SDL_Rect& camera, LTexture* gProjectileTexture) {
	gProjectileTexture->render(gRenderer, hitbox.x - camera.x, hitbox.y - camera.y, new SDL_Rect{ 0, 0, PROJECTILE_WIDTH, PROJECTILE_HEIGHT }, rotation);
}


