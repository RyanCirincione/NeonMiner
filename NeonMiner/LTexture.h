#pragma once

#include <SDL.h>
#include <string>

class LTexture
{
public:
	//Initializes variables
	LTexture();
	LTexture(SDL_Renderer* gRenderer, std::string path);

	//Deallocates memory
	~LTexture();

	//Loads image at specified path
	bool loadFromFile(SDL_Renderer* gRenderer, std::string path);

#if defined(SDL_TTF_MAJOR_VERSION)
	//Creates image from font string
	bool loadFromRenderedText(std::string textureText, SDL_Color textColor);
#endif

	//Deallocates texture
	void free();

	//Set color modulation
	void setColor(Uint8 red, Uint8 green, Uint8 blue);

	//Set blending
	void setBlendMode(SDL_BlendMode blending);

	//Set alpha modulation
	void setAlpha(Uint8 alpha);

	//Renders texture at given point
	void render(SDL_Renderer* gRenderer, int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
	void render(SDL_Renderer* gRenderer, SDL_Rect renderTarget, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

	//Gets image dimensions
	int getWidth();
	int getHeight();

	static bool loadTextures(SDL_Renderer* gRenderer);
	static void freeTextures();

private:
	//The actual hardware texture
	SDL_Texture* mTexture;

	//Image dimensions
	int mWidth;
	int mHeight;
};

static LTexture* PLAYER_TXT;
static LTexture* PROJECTILE_TXT;
static LTexture* WALL_SPRITES_TXT;
static LTexture* RED_ORE_TXT;
static LTexture* RED_SPARKLE_TXT;
static LTexture* GREEN_ORE_TXT;
static LTexture* GREEN_SPARKLE_TXT;
static LTexture* BLUE_ORE_TXT;
static LTexture* BLUE_SPARKLE_TXT;