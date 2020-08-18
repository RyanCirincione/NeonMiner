#ifndef Game_hpp
#define Game_hpp

#include "SDL.h"
#include "SDL_image.h"
#include <iostream>

class Game {
public:
	Game();
	~Game();
	void init();

	void handleEvents();
	void update();
	void render();
	void clean();

	bool running() { return isRunning; };

	static SDL_Renderer *renderer;

private:
	bool isRunning;
	SDL_Window *window;
};

#endif /* Game_hpp */
