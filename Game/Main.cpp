#include "Engine.h"
#include <SDL.h>
#include <SDL_Image.h>
#include <iostream>

int main(int, char**)
{
	gme::Engine engine;

	engine.Startup();
	engine.Get<gme::Renderer>()->Create("GAT150", 800, 600);

	std::cout << gme::GetFilePath() << std::endl;
	gme::SetFilePath("../Resources");
	std::cout << gme::GetFilePath() << std::endl;

	std::shared_ptr<gme::Texture> texture = engine.Get<gme::ResourceSystem>()->Get<gme::Texture>("sf2.png",engine.Get<gme::Renderer>());
	

	bool quit = false;
	SDL_Event event;

	while (!quit) {
		SDL_PollEvent(&event);
		switch (event.type)
		{
		case SDL_QUIT:
			quit = true;
			break;
		default:
			break;
		}
		
		engine.Get<gme::Renderer>()->BeginFrame();
		gme::Vector2 position{ gme::RandomRangeInt(0,800),gme::RandomRangeInt(0,600) };
		engine.Get<gme::Renderer>()->Draw(texture, position);
		engine.Get<gme::Renderer>()->EndFrame();

		/*for (size_t i = 0; i < 50; i++) {
			SDL_Rect src{ 32,64,32,64 };
			SDL_Rect dest{gme::RandomRangeInt(0,screen.x),gme::RandomRangeInt(0,screen.y), 16, 24};
			SDL_RenderCopy(renderer, texture, &src, &dest);
		}*/

		
	}


	engine.Shutdown();
	SDL_Quit();

	return 0;
}