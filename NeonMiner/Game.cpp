#include "Game.h"
#include "TextureManager.h"
#include "GameObject.h"
#include "Map.h"
#include "ECS.h"
#include "Components.h"

GameObject *player;
Map* map;

SDL_Renderer *Game::renderer = nullptr;

EntityManager entityManager;
auto& newPlayer(entityManager.addEntity());

Game::Game() {
}
Game::~Game() {
}

void Game::init() {
	const char *title = "Neon Miner";
	int flags = 0;
	if (false) {
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		std::cout << "Subsystems Initialized." << std::endl;

		window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, flags);

		if (window) {
			std::cout << "Window created." << std::endl;
		}

		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer) {
			SDL_SetRenderDrawColor(renderer, 240, 240, 240, 255);

			std::cout << "Renderer created." << std::endl;
		}

		isRunning = true;
	}
	else {
		isRunning = false;
	}

	player = new GameObject("assets/player.png", 0, 0);
	map = new Map();
	newPlayer.addComponent<PositionComponent>();
	newPlayer.getComponent<PositionComponent>().setPos(400, 300);
}

void Game::handleEvents() {
	SDL_Event event;
	SDL_PollEvent(&event);
	switch (event.type) {
	case SDL_QUIT:
		isRunning = false;
		break;
		
	default:
		break;
	}
}

void Game::update() {
	player->Update();
	entityManager.update();
}

void Game::render() {
	SDL_RenderClear(renderer);

	map->DrawMap();

	player->Render();

	SDL_RenderPresent(renderer);
}

void Game::clean() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	std::cout << "Game Cleaned." << std::endl;
}
