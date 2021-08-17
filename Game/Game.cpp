#include "Game.h"


void Game::Initialize() {
	//create engine
	engine = std::make_unique<gme::Engine>();
	engine->Startup();
	engine->Get<gme::Renderer>()->Create("GAT150", 800, 600);
	//create scene
	scene = std::make_unique<gme::Scene>();
	scene->engine = engine.get();
	//seed random number
	gme::SeedRandom(static_cast<unsigned int>(time(nullptr)));
	//change file path
	gme::SetFilePath("../Resources");

	std::unique_ptr<gme::Actor> actor = std::make_unique<gme::Actor>(gme::Transform{ gme::Vector2{400,300}, 0, 2 });
	{
		std::unique_ptr<gme::SpriteComponent> spriteComponent = std::make_unique<gme::SpriteComponent>();
		spriteComponent->texture = engine->Get<gme::ResourceSystem>()->Get<gme::Texture>("ship.png", engine->Get<gme::Renderer>());
		actor->AddComponent(std::move(spriteComponent));
	}
	{
		std::unique_ptr<gme::PhysicsComponent> physicsComponent = std::make_unique<gme::PhysicsComponent>();
		physicsComponent->ApplyForce(gme::Vector2::right * 100);
		actor->AddComponent(std::move(physicsComponent));
	}
	scene->AddActor(std::move(actor));
	
}

void Game::Shutdown(){
	scene->RemoveAllActors();
	engine->Shutdown();
}

void Game::Update(){
	
	engine->Update();
	if (!quit) quit = gme::IsButtonPressed(SDL_SCANCODE_ESCAPE, engine.get());
	scene->Update(engine->time.deltaTime);
}

void Game::Draw(){
	engine->Get<gme::Renderer>()->BeginFrame();
	engine->Draw(engine->Get<gme::Renderer>());
	scene->Draw(engine->Get<gme::Renderer>());
	engine->Get<gme::Renderer>()->EndFrame();
}