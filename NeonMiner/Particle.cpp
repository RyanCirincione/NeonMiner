#include "Particle.h"

Particle::Particle(LTexture* tx, int life, float pX, float pY, float vX, float vY) {
	texture = tx;
	posX = pX;
	posY = pY;
	velX = vX;
	velY = vY;
	lifetime = life;
}

void Particle::update() {
	lifetime--;

	posX += velX;
	posY += velY;
}

void Particle::render(SDL_Renderer* gRenderer, SDL_Rect& camera) {
	texture->render(gRenderer, posX - camera.x, posY - camera.y);
}