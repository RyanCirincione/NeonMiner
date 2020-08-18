#include  "GameObject.h"
#include "TextureManager.h"

GameObject::GameObject(const char *textureSheet, int x, int y) {
	texture = TextureManager::LoadTexture(textureSheet);

	xpos = x;
	ypos = y;
}

void GameObject::Update() {


	srcRect.h = 32;
	srcRect.w = 32;
	srcRect.x = 0;
	srcRect.y = 0;

	destRect.x = xpos;
	destRect.y = ypos;
	destRect.w = srcRect.w;
	destRect.h = srcRect.h;
}

void GameObject::Render() {
	SDL_RenderCopy(Game::renderer, texture, &srcRect, &destRect);
}
