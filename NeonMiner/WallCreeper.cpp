#include "WallCreeper.h"

WallCreeper::WallCreeper(float x, float y, Player player)
{
	//Initialize the collision box
	posX = x;
	posY = y;

	//Initialize the velocity
	float dist = sqrt(pow(player.posX - x, 2) + pow(player.posY - y, 2));
	mVelX = (player.posX - x) * 5 / dist;
	mVelY = (player.posY - y) * 5 / dist;

	rotation = 0;

	age = 0;

	remove = false;
}

const int PURSUE_TIME = 240;
void WallCreeper::update(Tile*** tiles, Player player) {
	age++;

	float dist = sqrt(pow(player.posX - posX, 2) + pow(player.posY - posY, 2));
	if (age < PURSUE_TIME) {
		mVelX = (player.posX - posX) * 6 / dist * (1 - (float)age / PURSUE_TIME);
		mVelY = (player.posY - posY) * 6 / dist * (1 - (float)age / PURSUE_TIME);
	}
	else {
		mVelX = (posX - player.posX) * 7 / dist * ((float)age / PURSUE_TIME - 1);
		mVelY = (posY - player.posY) * 7 / dist * ((float)age / PURSUE_TIME - 1);
	}

	posX += mVelX;
	posY += mVelY;

	rotation = atan((float)(player.posY - posY) / (player.posX - posX)) / M_PI * 180;
	if (player.posX < posX) {
		rotation += 180;
	}
	if (age >= PURSUE_TIME) {
		rotation += 180;
	}

	if (age > PURSUE_TIME && Tile::touchesWall(*new SDL_Rect{ (int)posX, (int)posY, PLAYER_WIDTH, PLAYER_HEIGHT }, tiles)) {
		remove = true;
	}
}

void WallCreeper::render(SDL_Renderer* gRenderer, SDL_Rect& camera) {
	WALL_CREEPER_TXT->render(gRenderer, posX - camera.x, posY - camera.y, new SDL_Rect{ 0, 0, PLAYER_WIDTH, PLAYER_HEIGHT }, rotation);
}