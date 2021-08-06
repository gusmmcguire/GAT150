#include "Engine.h"
#include <SDL.h>
#include <SDL_Image.h>
#include <iostream>

int main(int, char**)
{
	gme::Engine engine;

	engine.Startup();
	engine.Get<gme::Renderer>()->Create("GAT150", 800, 600);

	gme::Scene scene;
	scene.engine = &engine;

	gme::SetFilePath("../Resources");


	std::shared_ptr<gme::Texture> texture = engine.Get<gme::ResourceSystem>()->Get<gme::Texture>("sf2.png",engine.Get<gme::Renderer>());
	
	for(size_t i = 0; i < 10; i++){
		gme::Transform transform{ gme::Vector2{gme::RandomRange(0,800),gme::RandomRange(0,600)}, gme::RandomRange(0,360), 1.0f };
		std::unique_ptr<gme::Actor> actor = std::make_unique<gme::Actor>(transform, texture);
		scene.AddActor(std::move(actor)); 
	}


	bool quit = false;
	SDL_Event event;
	float angle = 0;

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
		
		engine.Update(0);
		quit = (engine.Get<gme::InputSystem>()->GetKeyState(SDL_SCANCODE_ESCAPE) == gme::InputSystem::eKeyState::Pressed);
		scene.Update(0);

 		
		engine.Get<gme::Renderer>()->BeginFrame();

		scene.Draw(engine.Get<gme::Renderer>());

		engine.Get<gme::Renderer>()->EndFrame();
	
	}


	engine.Shutdown();
	SDL_Quit();

	return 0;
}