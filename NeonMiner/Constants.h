#pragma once

//Screen dimension constants
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 960;

//The dimensions of the level
const int LEVEL_WIDTH = 2560;
const int LEVEL_HEIGHT = 1920;

//Tile constants
const int TILE_WIDTH = 16;
const int TILE_HEIGHT = 16;

//The dimensions of the player
const int PLAYER_WIDTH = 32;
const int PLAYER_HEIGHT = 32;

//Maximum axis velocity of the player
const int PLAYER_VEL = 6;

const int PROJECTILE_WIDTH = 16;
const int PROJECTILE_HEIGHT = 16;
const int PROJECTILE_SPEED = PLAYER_VEL * 2;

//The different tile sprites
const int TILE_SPACE = 0;
const int TILE_WALL = 1;

//The center of the world
const int CENTER_X = LEVEL_WIDTH / 2;
const int CENTER_Y = LEVEL_HEIGHT / 2;

//Ore tags
const int RED_ORE = 0;
const int GREEN_ORE = 1;
const int BLUE_ORE = 2;