#pragma once

#include "Game.h"

const int MAP_W = 200, MAP_H = 200;

class Map {
public:
	Map();
	~Map();

	void LoadMap(int arr[MAP_W][MAP_H]);
	void DrawMap();

private:
	SDL_Rect src, dest;
	SDL_Texture *wall;
	SDL_Texture *space;

	int map[MAP_W][MAP_H];

};
