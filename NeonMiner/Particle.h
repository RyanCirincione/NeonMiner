#pragma once

#include <SDL.h>

#include "LTexture.h"

class Particle {
public:
	Particle(LTexture* tx, int life, float pX, float pY, float vX, float vY);
	void update();
	void render(SDL_Renderer* gRenderer, SDL_Rect& camera);

	int lifetime;

private:
	LTexture* texture;
	float posX;
	float posY;
	float velX;
	float velY;
};