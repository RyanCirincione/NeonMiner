#pragma once

//Tile constants
const int TILE_WIDTH = 16;
const int TILE_HEIGHT = 16;

//Screen dimension constants
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 960;

//The dimensions of the level
const int LEVEL_WIDTH = 16 * 64 * TILE_WIDTH;
const int LEVEL_HEIGHT = 16 * 64 * TILE_HEIGHT;

//The dimensions of the hub
const int HUB_WIDTH = 67 * TILE_WIDTH;
const int HUB_HEIGHT = 62 * TILE_HEIGHT;

//Hard coded portal placement
const float HUB_PORTAL_X = 57.5 * TILE_WIDTH;
const float HUB_PORTAL_Y = 8.5 * TILE_HEIGHT;
const float WORLD_PORTAL_X = LEVEL_WIDTH / 2;
const float WORLD_PORTAL_Y = LEVEL_HEIGHT / 2;
const int PORTAL_WIDTH = TILE_WIDTH * 3;
const int PORTAL_HEIGHT = TILE_HEIGHT * 3;

//Diamond-Square wall threshold
const float WORLDGEN_THRESHOLD = 0.5f;

//Ore Gen
const int ORE_PATCH_ATTEMPTS = 200;
const int ORE_ATTEMPTS_PER_PATCH = 50;
const int ORE_PATCH_RANGE = 6;

//Chunk constants
const int TILES_PER_CHUNK = 16;

//The center of the world
const int CENTER_X = LEVEL_WIDTH / 2;
const int CENTER_Y = LEVEL_HEIGHT / 2;

//The dimensions of the player
const int PLAYER_WIDTH = 32;
const int PLAYER_HEIGHT = 32;

//Maximum axis velocity of the player
const float PLAYER_VEL = 0.6f;
const float FRICTION = 1.09f;

const float DASH_VEL = 10.0f;
const float DASH_BONUS_FACTOR = 0.1f;
const float DASH_BONUS_DECAY = 0.005f;
const float MIN_TIME_TO_DASH = 0.9f;
const float MIN_TIME_TO_DASH_BONUS = 1.3f;
const float MAX_TIME_TO_DASH_BONUS = 1.6f;
const float MAX_TIME_TO_BONUS_DECAY = 2.0f;

const int PROJECTILE_WIDTH = 16;
const int PROJECTILE_HEIGHT = 16;
const int PROJECTILE_SPEED = 12;

//The different tiles
const int TILE_SPACE = 0;
const int TILE_WALL = 1;

const int TILE_RED_ORE = 2;
const int TILE_GREEN_ORE = 3;
const int TILE_BLUE_ORE = 4;

//Items
const int ITEM_RED_ORE = 0;
const int ITEM_GREEN_ORE = 1;
const int ITEM_BLUE_ORE = 2;

//Clock segment labels
const int CLOCK_CALM = 0;
const int CLOCK_RISING = 1;
const int CLOCK_DANGER = 2;
