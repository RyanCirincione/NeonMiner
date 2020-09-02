#include <cmath>

#include "Constants.h"
#include "Player.h"
#include "Tile.h"
#include "LTexture.h"

Player::Player()
{
	//Initialize the collision box
	mBox.x = CENTER_X;
	mBox.y = CENTER_Y;
	mBox.w = PLAYER_WIDTH;
	mBox.h = PLAYER_HEIGHT;

	//Initialize the velocity
	mVelX = 0;
	mVelY = 0;
	rotation = 0;
}

void Player::handleEvent(SDL_Event& e, std::vector<Projectile*>* projectiles)
{
	//If a key was pressed
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
	{
		//Adjust the velocity
		switch (e.key.keysym.sym)
		{
		case SDLK_w: mVelY -= PLAYER_VEL; break;
		case SDLK_s: mVelY += PLAYER_VEL; break;
		case SDLK_a: mVelX -= PLAYER_VEL; break;
		case SDLK_d: mVelX += PLAYER_VEL; break;
		}
	}
	//If a key was released
	else if (e.type == SDL_KEYUP && e.key.repeat == 0)
	{
		//Adjust the velocity
		switch (e.key.keysym.sym)
		{
		case SDLK_w: mVelY += PLAYER_VEL; break;
		case SDLK_s: mVelY -= PLAYER_VEL; break;
		case SDLK_a: mVelX += PLAYER_VEL; break;
		case SDLK_d: mVelX -= PLAYER_VEL; break;
		}
	}
	else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON(SDL_BUTTON_LEFT)) {
		Projectile* p = new Projectile(mBox.x + PLAYER_WIDTH / 2 - PROJECTILE_WIDTH / 2, mBox.y + PROJECTILE_HEIGHT / 2, rotation);
		projectiles->push_back(p);
	}
}

void Player::move(Tile *tiles[LEVEL_WIDTH / TILE_WIDTH][LEVEL_HEIGHT / TILE_HEIGHT], SDL_Rect& camera, std::vector<Item*> items)
{
	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);
	int diffX = mouseX - mBox.x + camera.x - PLAYER_WIDTH / 2;
	int diffY = mouseY - mBox.y + camera.y - PLAYER_HEIGHT / 2;

	if (mouseY - mBox.y == 0) {
		rotation = diffX > 0 ? 0 : 180;
	}
	else {
		rotation = atan((float) diffY / diffX) / M_PI * 180;
		rotation += diffX > 0 ? 0 : 180;
	}

	//Move the dot left or right
	mBox.x += mVelX;

	//If the dot went too far to the left or right or touched a wall
	if ((mBox.x < 0) || (mBox.x + PLAYER_WIDTH > LEVEL_WIDTH) || Tile::touchesWall(mBox, tiles))
	{
		//move back
		mBox.x -= mVelX;
	}

	//Move the dot up or down
	mBox.y += mVelY;

	//If the dot went too far up or down or touched a wall
	if ((mBox.y < 0) || (mBox.y + PLAYER_HEIGHT > LEVEL_HEIGHT) || Tile::touchesWall(mBox, tiles))
	{
		//move back
		mBox.y -= mVelY;
	}

	//Move items closer to player
	for (auto i : items) {
		float dist = sqrt(pow(i->posX - mBox.x, 2) + pow(i->posY - mBox.y, 2));
		if (dist < 100) {
			i->posX += (mBox.x - i->posX) * 5 / dist;
			i->posY += (mBox.y - i->posY) * 5 / dist;
		}
	}
}

void Player::setCamera(SDL_Rect& camera)
{
	//Center the camera over the dot
	camera.x = (mBox.x + PLAYER_WIDTH / 2) - SCREEN_WIDTH / 2;
	camera.y = (mBox.y + PLAYER_HEIGHT / 2) - SCREEN_HEIGHT / 2;

	//Keep the camera in bounds
	if (camera.x < 0)
	{
		camera.x = 0;
	}
	if (camera.y < 0)
	{
		camera.y = 0;
	}
	if (camera.x > LEVEL_WIDTH - camera.w)
	{
		camera.x = LEVEL_WIDTH - camera.w;
	}
	if (camera.y > LEVEL_HEIGHT - camera.h)
	{
		camera.y = LEVEL_HEIGHT - camera.h;
	}
}

void Player::render(SDL_Renderer* gRenderer, SDL_Rect& camera, LTexture* gDotTexture)
{
	//Show the dot
	gDotTexture->render(gRenderer, mBox.x - camera.x, mBox.y - camera.y, new SDL_Rect{ 0, 0, PLAYER_WIDTH, PLAYER_HEIGHT }, rotation);
}