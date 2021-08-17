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