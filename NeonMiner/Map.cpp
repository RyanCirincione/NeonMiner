#include "Map.h"
#include "TextureManager.h"

const int PIXELS_IN_TILE = 16;

Map::Map() {
	wall = TextureManager::LoadTexture("assets/wall.png");
	space = TextureManager::LoadTexture("assets/space.png");

	// LoadMap(defaultMap)

	src.x = 0;
	src.y = 0;
	src.w = PIXELS_IN_TILE;
	src.h = PIXELS_IN_TILE;

	dest.x = 0;
	dest.y = 0;
	dest.w = src.w;
	dest.h = src.h;
}

void Map::LoadMap(int arr[MAP_W][MAP_H]) {
	for (int row = 0; row < MAP_W; row++) {
		for (int col = 0; col < MAP_H; col++) {
			map[row][col] = arr[row][col];
		}
	}
}

void Map::DrawMap() {
	for (int row = 0; row < MAP_W; row++) {
		for (int col = 0; col < MAP_H; col++) {
			dest.x = col * PIXELS_IN_TILE;
			dest.y = row * PIXELS_IN_TILE;

			switch (map[row][col]) {
			case 0: // Space
				TextureManager::Draw(space, src, dest);
				break;
			case 1: // Wall
				TextureManager::Draw(wall, src, dest);
			default:
				break;
			}
		}
	}
}
