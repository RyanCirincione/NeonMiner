#include <cmath>

#include "Constants.h"
#include "Player.h"
#include "Tile.h"
#include "LTexture.h"

Player::Player()
{
	//Initialize the collision box
	posX = CENTER_X;
	posY = CENTER_Y;

	//Initialize the velocity
	mVelX = 0;
	mVelY = 0;
	rotation = 0;

	dashCooldown = MAX_TIME_TO_BONUS_DECAY;
	dashBoost = 1;
}

void Player::handleEvent(SDL_Event& e, std::vector<Projectile*>* projectiles)
{
	if (dashCooldown >= MIN_TIME_TO_DASH && e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_LSHIFT) {
		mVelX += cos(rotation / 180 * M_PI) * DASH_VEL * dashBoost;
		mVelY += sin(rotation / 180 * M_PI) * DASH_VEL * dashBoost;

		if (dashCooldown >= MIN_TIME_TO_DASH_BONUS && dashCooldown <= MAX_TIME_TO_DASH_BONUS) {
			dashBoost += DASH_BONUS_FACTOR;
		}

		dashCooldown = 0;
	}

	if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON(SDL_BUTTON_LEFT)) {
		Projectile* p = new Projectile(posX + PLAYER_WIDTH / 2 - PROJECTILE_WIDTH / 2, posY + PROJECTILE_HEIGHT / 2, rotation);
		projectiles->push_back(p);
	}
}

void Player::move(Tile*** tiles, SDL_Rect& camera, std::vector<Item*> items)
{
	dashCooldown += 0.01;
	if (dashCooldown >= MAX_TIME_TO_BONUS_DECAY) {
		dashBoost = fmax(dashBoost - DASH_BONUS_DECAY, 1.0);
	}

	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);
	int diffX = mouseX - posX + camera.x - PLAYER_WIDTH / 2;
	int diffY = mouseY - posY + camera.y - PLAYER_HEIGHT / 2;

	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_W]) {
		mVelY -= PLAYER_VEL;
	}
	if (state[SDL_SCANCODE_A]) {
		mVelX -= PLAYER_VEL;
	}
	if (state[SDL_SCANCODE_S]) {
		mVelY += PLAYER_VEL;
	}
	if (state[SDL_SCANCODE_D]) {
		mVelX += PLAYER_VEL;
	}
	//mVelX += fmin(FRICTION, fmax(-FRICTION, -mVelX));
	mVelX /= FRICTION;
	//mVelY += fmin(FRICTION, fmax(-FRICTION, -mVelY));
	mVelY /= FRICTION;

	if (mouseY - posY == 0) {
		rotation = diffX > 0 ? 0 : 180;
	}
	else {
		rotation = atan((float) diffY / diffX) / M_PI * 180;
		rotation += diffX > 0 ? 0 : 180;
	}

	//Move the dot left or right
	posX += mVelX;

	//If the dot went too far to the left or right or touched a wall
	if ((posX < 0) || (posX + PLAYER_WIDTH > LEVEL_WIDTH) || Tile::touchesWall(*new SDL_Rect{ (int)posX, (int)posY, PLAYER_WIDTH, PLAYER_HEIGHT }, tiles))
	{
		//move back
		posX -= mVelX;
	}

	//Move the dot up or down
	posY += mVelY;

	//If the dot went too far up or down or touched a wall
	if ((posY < 0) || (posY + PLAYER_HEIGHT > LEVEL_HEIGHT) || Tile::touchesWall(*new SDL_Rect{ (int)posX, (int)posY, PLAYER_WIDTH, PLAYER_HEIGHT }, tiles))
	{
		//move back
		posY -= mVelY;
	}

	//Move items closer to player
	for (auto i : items) {
		float dist = sqrt(pow(i->posX - posX, 2) + pow(i->posY - posY, 2));
		if (dist < 100) {
			i->posX += (posX - i->posX) * 5 / dist;
			i->posY += (posY - i->posY) * 5 / dist;
		}

		//Remove, and later handle, items that are "picked up"
		if (dist < 6) {
			i->remove = true;
			printf("Item: %d\n", i->type);
		}
	}

	dashCooldown += 0.01;
}

void Player::setCamera(SDL_Rect& camera)
{
	//Center the camera over the dot
	camera.x = (posX + PLAYER_WIDTH / 2) - SCREEN_WIDTH / 2;
	camera.y = (posY + PLAYER_HEIGHT / 2) - SCREEN_HEIGHT / 2;

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
	gDotTexture->render(gRenderer, posX - camera.x, posY - camera.y, new SDL_Rect{ 0, 0, PLAYER_WIDTH, PLAYER_HEIGHT }, rotation);

	const int DASH_BAR_WIDTH = 200;
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderDrawLine(gRenderer, 25, SCREEN_HEIGHT - 25, 25, SCREEN_HEIGHT - 35);
	SDL_RenderDrawLine(gRenderer, 25 + DASH_BAR_WIDTH, SCREEN_HEIGHT - 25, 25 + DASH_BAR_WIDTH, SCREEN_HEIGHT - 35);
	SDL_RenderDrawLine(gRenderer, 25 + fmin(DASH_BAR_WIDTH, dashCooldown / MAX_TIME_TO_BONUS_DECAY * DASH_BAR_WIDTH), SCREEN_HEIGHT - 25,
		25 + fmin(DASH_BAR_WIDTH, dashCooldown / MAX_TIME_TO_BONUS_DECAY * DASH_BAR_WIDTH), SCREEN_HEIGHT - 35);
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF);
	SDL_RenderDrawLine(gRenderer, 25 + fmin(DASH_BAR_WIDTH, MIN_TIME_TO_DASH / MAX_TIME_TO_BONUS_DECAY * DASH_BAR_WIDTH), SCREEN_HEIGHT - 25,
		25 + fmin(DASH_BAR_WIDTH, MIN_TIME_TO_DASH / MAX_TIME_TO_BONUS_DECAY * DASH_BAR_WIDTH), SCREEN_HEIGHT - 35);
	SDL_SetRenderDrawColor(gRenderer, 0x00, 0xFF, 0x00, 0xFF);
	SDL_RenderDrawLine(gRenderer, 25 + fmin(DASH_BAR_WIDTH, MIN_TIME_TO_DASH_BONUS / MAX_TIME_TO_BONUS_DECAY * DASH_BAR_WIDTH), SCREEN_HEIGHT - 25,
		25 + fmin(DASH_BAR_WIDTH, MIN_TIME_TO_DASH_BONUS / MAX_TIME_TO_BONUS_DECAY * DASH_BAR_WIDTH), SCREEN_HEIGHT - 35);
	SDL_RenderDrawLine(gRenderer, 25 + fmin(DASH_BAR_WIDTH, MAX_TIME_TO_DASH_BONUS / MAX_TIME_TO_BONUS_DECAY * DASH_BAR_WIDTH), SCREEN_HEIGHT - 25,
		25 + fmin(DASH_BAR_WIDTH, MAX_TIME_TO_DASH_BONUS / MAX_TIME_TO_BONUS_DECAY * DASH_BAR_WIDTH), SCREEN_HEIGHT - 35);
}