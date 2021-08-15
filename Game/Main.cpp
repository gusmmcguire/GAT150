#include "Game.h"

int main(int, char**){
	Game game;
	game.Initialize();

	bool quit = false;
	SDL_Event event;

	while (!game.IsQuit() && !quit) {
		SDL_PollEvent(&event);
		switch (event.type)
		{
		case SDL_QUIT:
			quit = true;
			break;
		}
		game.Update();
		game.Draw();
	}

	game.Shutdown();
	SDL_Quit();

	return 0;
}