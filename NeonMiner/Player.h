#pragma once

#include <SDL.h>
#include <vector>

#include "Tile.h"
#include "Projectile.h"

class Player
{
public:
	//Initializes the variables
	Player();

	//Takes key presses and adjusts the dot's velocity
	void handleEvent(SDL_Event& e, std::vector<Projectile*>* projectiles);

	//Moves the dot and check collision against tiles
	void move(Tile*** tiles, SDL_Rect & camera, std::vector<Item*> items);

	//Centers the camera over the dot
	void setCamera(SDL_Rect& camera);

	//Shows the dot on the screen
	void render(SDL_Renderer* gRenderer, SDL_Rect& camera, LTexture* gDotTexture);

	//Collision box of the dot
	float posX, posY;

	float rotation;

private:

	//The velocity of the dot
	float mVelX, mVelY;
	float dashCooldown, dashBoost;
};