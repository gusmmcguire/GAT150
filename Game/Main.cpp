#include "Engine.h"
#include <SDL.h>
#include <SDL_Image.h>
#include <iostream>
#include <cassert>

int main(int, char**){
	gme::Engine engine;

	engine.Startup();
	engine.Get<gme::Renderer>()->Create("GAT150", 800, 600);

	gme::Scene scene;
	scene.engine = &engine;

	gme::SetFilePath("../Resources");

	engine.Get<gme::AudioSystem>()->AddAudio("explosion", "Audio/explosion.wav");
	engine.Get<gme::AudioSystem>()->AddAudio("backingMusic", "Audio/backing.wav");
	gme::AudioChannel channel = engine.Get<gme::AudioSystem>()->PlayAudio("backingMusic", 1, 1, true);

	std::shared_ptr<gme::Texture> movingTexture = engine.Get<gme::ResourceSystem>()->Get<gme::Texture>("shuriken.png",engine.Get<gme::Renderer>());
	
	for(size_t i = 0; i < 10; i++){
		gme::Transform transform{ gme::Vector2{gme::RandomRange(0,800),gme::RandomRange(0,600)}, gme::RandomRange(0,360), 2.0f };
		std::unique_ptr<gme::Actor> actor = std::make_unique<gme::Actor>(transform, movingTexture);
		scene.AddActor(std::move(actor)); 
	}


	int size = 16;
	std::shared_ptr<gme::Font> font = engine.Get<gme::ResourceSystem>()->Get<gme::Font>("Fonts/KarmaFuture.ttf", &size);

	std::shared_ptr<gme::Texture> textTexture = std::make_shared<gme::Texture>(engine.Get<gme::Renderer>());
	textTexture->Create(font->CreateSurface("hello world", gme::Color{ 1,1,1 }));
	engine.Get<gme::ResourceSystem>()->Add("textTexture", textTexture);



	bool quit = false;
	SDL_Event event;
	float quitTime = engine.time.time + 3.0f;

	while (!quit) {
		SDL_PollEvent(&event);
		switch (event.type)
		{
		case SDL_QUIT:
			quit = true;
			break;
		}
		
		
		engine.Update();
		scene.Update(engine.time.deltaTime);
		if(!quit) quit = (engine.Get<gme::InputSystem>()->GetKeyState(SDL_SCANCODE_ESCAPE) == gme::InputSystem::eKeyState::Pressed);
		std::shared_ptr<gme::Texture> particleTexture = engine.Get<gme::ResourceSystem>()->Get<gme::Texture>("particle01.png", engine.Get<gme::Renderer>());

		if (engine.Get<gme::InputSystem>()->GetButtonState((int)gme::InputSystem::eMouseButton::Left) == gme::InputSystem::eKeyState::Pressed) {
			engine.Get<gme::ParticleSystem>()->Create(engine.Get<gme::InputSystem>()->GetMousePosition(), 50, .5, particleTexture, 300);
			engine.Get<gme::AudioSystem>()->PlayAudio("explosion", 1, gme::RandomRange(0.2,2));
			channel.SetPitch(gme::RandomRange(-2, 2));
		}

	

		engine.Get<gme::Renderer>()->BeginFrame();

		gme::Transform t;
		t.position = { 30,30 };
		engine.Get<gme::Renderer>()->Draw(textTexture, t);


		scene.Draw(engine.Get<gme::Renderer>());
		engine.Draw(engine.Get<gme::Renderer>());

		engine.Get<gme::Renderer>()->EndFrame();
	
	}


	engine.Shutdown();
	SDL_Quit();

	return 0;
}