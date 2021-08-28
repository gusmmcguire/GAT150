#include "Game.h"
#include "GameComponent/PlayerComponent.h"
#include "GameComponent/EnemyComponent.h"
#include "GameComponent/PickupComponent.h"

void Game::Initialize() {
	//create engine
	engine = std::make_unique<gme::Engine>();
	engine->Startup();
	engine->Get<gme::Renderer>()->Create("GAT150", 800, 600);

	//register classes
	REGISTER_CLASS(PlayerComponent);
	REGISTER_CLASS(EnemyComponent);
	REGISTER_CLASS(PickupComponent);

	//create scene
	scene = std::make_unique<gme::Scene>();
	scene->engine = engine.get();
	//seed random number
	gme::SeedRandom(static_cast<unsigned int>(time(nullptr)));
	//change file path
	gme::SetFilePath("../Resources");

	rapidjson::Document document;
 	bool success = gme::json::Load("scene.json", document);
	assert(success);
	scene->Read(document);
	
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