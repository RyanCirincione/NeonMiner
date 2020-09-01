#include "Tile.h"

Tile::Tile(int x, int y, int tileType)
{
	//Get the offsets
	mBox.x = x;
	mBox.y = y;

	//Set the collision box
	mBox.w = TILE_WIDTH;
	mBox.h = TILE_HEIGHT;

	//Get the tile type
	mType = tileType;
}

void Tile::render(SDL_Renderer* gRenderer, SDL_Rect& camera, LTexture* gTileTexture, SDL_Rect gTileClips[])
{
	//If the tile is on screen
	if (checkCollision(camera, mBox))
	{
		//Show the tile
		gTileTexture->render(gRenderer, mBox.x - camera.x, mBox.y - camera.y, &gTileClips[mType]);
	}
}

int Tile::getType()
{
	return mType;
}

void Tile::setType(int type) {
	mType = type;
}

SDL_Rect Tile::getBox()
{
	return mBox;
}

bool Tile::checkCollision(SDL_Rect a, SDL_Rect b)
{
	//The sides of the rectangles
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	//Calculate the sides of rect A
	leftA = a.x;
	rightA = a.x + a.w;
	topA = a.y;
	bottomA = a.y + a.h;

	//Calculate the sides of rect B
	leftB = b.x;
	rightB = b.x + b.w;
	topB = b.y;
	bottomB = b.y + b.h;

	//If any of the sides from A are outside of B
	if (bottomA <= topB)
	{
		return false;
	}

	if (topA >= bottomB)
	{
		return false;
	}

	if (rightA <= leftB)
	{
		return false;
	}

	if (leftA >= rightB)
	{
		return false;
	}

	//If none of the sides from A are outside B
	return true;
}

bool Tile::touchesWall(SDL_Rect box, Tile* tiles[LEVEL_WIDTH / TILE_WIDTH][LEVEL_HEIGHT / TILE_HEIGHT])
{
	//Go through the tiles
	for (int i = 0; i < LEVEL_WIDTH / TILE_WIDTH; ++i)
	{
		for (int j = 0; j < LEVEL_HEIGHT / TILE_HEIGHT; j++) {
			//If the tile is a wall type tile
			if (tiles[i][j]->getType() == 1)
			{
				//If the collision box touches the wall tile
				if (checkCollision(box, tiles[i][j]->getBox()))
				{
					return true;
				}
			}
		}
	}

	//If no wall tiles were touched
	return false;
}
