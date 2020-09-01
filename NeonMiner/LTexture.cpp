#include <SDL_image.h>

#include "LTexture.h"

LTexture::LTexture()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::LTexture(SDL_Renderer* gRenderer, std::string path)
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;

	if (!this->loadFromFile(gRenderer, path)) {
		printf("Failed to load texture from %s!\n", path);
	}
}

LTexture::~LTexture()
{
	//Deallocate
	free();
}

bool LTexture::loadFromFile(SDL_Renderer* gRenderer, std::string path)
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		//Color key image
		//SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

bool LTexture::loadTextures(SDL_Renderer* gRenderer) {
	bool success = true;

	PLAYER_TXT = new LTexture();
	PROJECTILE_TXT = new LTexture();
	WALL_SPRITES_TXT = new LTexture();
	RED_ORE_TXT = new LTexture();
	RED_SPARKLE_TXT = new LTexture();
	GREEN_ORE_TXT = new LTexture();
	GREEN_SPARKLE_TXT = new LTexture();
	BLUE_ORE_TXT = new LTexture();
	BLUE_SPARKLE_TXT = new LTexture();

	if (!PLAYER_TXT->loadFromFile(gRenderer, "assets/neon_player.png"))
	{
		printf("Failed to load player texture!\n");
		success = false;
	}

	if (!PROJECTILE_TXT->loadFromFile(gRenderer, "assets/neon_projectile.png"))
	{
		printf("Failed to load projectile texture!\n");
		success = false;
	}

	if (!WALL_SPRITES_TXT->loadFromFile(gRenderer, "assets/neon_wall_sheet.png"))
	{
		printf("Failed to load tile set texture!\n");
		success = false;
	}

	if (!RED_SPARKLE_TXT->loadFromFile(gRenderer, "assets/neon_red_sparkle.png"))
	{
		printf("Failed to load red sparkle texture!\n");
		success = false;
	}

	if (!GREEN_SPARKLE_TXT->loadFromFile(gRenderer, "assets/neon_green_sparkle.png"))
	{
		printf("Failed to load green sparkle texture!\n");
		success = false;
	}

	if (!BLUE_SPARKLE_TXT->loadFromFile(gRenderer, "assets/neon_blue_sparkle.png"))
	{
		printf("Failed to load blue sparkle texture!\n");
		success = false;
	}

	if (!RED_ORE_TXT->loadFromFile(gRenderer, "assets/neon_red_ore.png"))
	{
		printf("Failed to load red ore texture!\n");
		success = false;
	}

	if (!GREEN_ORE_TXT->loadFromFile(gRenderer, "assets/neon_green_ore.png"))
	{
		printf("Failed to load green ore texture!\n");
		success = false;
	}

	if (!BLUE_ORE_TXT->loadFromFile(gRenderer, "assets/neon_blue_ore.png"))
	{
		printf("Failed to load blue ore texture!\n");
		success = false;
	}
	printf("%d\n", WALL_SPRITES_TXT); // -----------------------------------------------------------------------------------------

	return success;
}

void LTexture::freeTextures() {
	PLAYER_TXT->free();
	PROJECTILE_TXT->free();
	WALL_SPRITES_TXT->free();
	RED_ORE_TXT->free();
	RED_SPARKLE_TXT->free();
	GREEN_ORE_TXT->free();
	GREEN_SPARKLE_TXT->free();
	BLUE_ORE_TXT->free();
	BLUE_SPARKLE_TXT->free();
}

#if defined(SDL_TTF_MAJOR_VERSION)
bool LTexture::loadFromRenderedText(std::string textureText, SDL_Color textColor)
{
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, textureText.c_str(), textColor);
	if (textSurface != NULL)
	{
		//Create texture from surface pixels
		mTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
		if (mTexture == NULL)
		{
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		else
		{
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface(textSurface);
	}
	else
	{
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}


	//Return success
	return mTexture != NULL;
}
#endif

void LTexture::free()
{
	//Free texture if it exists
	if (mTexture != NULL)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
	//Modulate texture rgb
	SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void LTexture::setBlendMode(SDL_BlendMode blending)
{
	//Set blending function
	SDL_SetTextureBlendMode(mTexture, blending);
}

void LTexture::setAlpha(Uint8 alpha)
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod(mTexture, alpha);
}

void LTexture::render(SDL_Renderer* gRenderer, int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad, angle, center, flip);
}

void LTexture::render(SDL_Renderer* gRenderer, SDL_Rect renderTarget, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
	//Render to screen
	SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderTarget, angle, center, flip);
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}