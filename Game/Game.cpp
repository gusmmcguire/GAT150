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

	engine->Get<gme::EventSystem>()->Subscribe("change_level", std::bind(&Game::ChangeLevel, this, std::placeholders::_1));
	engine->Get<gme::AudioSystem>()->AddAudio("backing", "Audio/backing.wav");
	engine->Get<gme::AudioSystem>()->PlayAudio("backing", 1, 1, true);
}

void Game::Shutdown(){
	scene->RemoveAllActors();
	engine->Shutdown();
}

void Game::Update(){
	
	engine->Update();
	if (!quit) quit = gme::IsButtonPressed(SDL_SCANCODE_ESCAPE, engine.get());
	switch (state)
	{
	case Game::eState::Reset:
		Reset();
		break;
	case Game::eState::Title:
		Title();
		break;
	case Game::eState::StartGame:
		StartGame();
		break;
	case Game::eState::StartLevel:
		StartLevel();
		break;
	case Game::eState::Level:
		Level();
		break;
	case Game::eState::StartLevel2:
		StartLevel2();
		break;
	case Game::eState::Level2:
		Level2();
		break;
	case Game::eState::PlayerDead:
		PlayerDead();
		break;
	case Game::eState::GameOver:
		GameOver();
		break;
	default:
		break;
	}

	if(state != eState::GameOver )surviveTimer += engine->time.deltaTime;

	auto timerText = scene->FindActor("Timer");
	if (timerText) timerText->GetComponent<gme::TextComponent>()->SetText("Time Survived: " + std::to_string(static_cast<int>(surviveTimer)));

	scene->Update(engine->time.deltaTime);
}

void Game::Draw(){
	engine->Get<gme::Renderer>()->BeginFrame();
	engine->Draw(engine->Get<gme::Renderer>());
	scene->Draw(engine->Get<gme::Renderer>());
	engine->Get<gme::Renderer>()->EndFrame();
}

void Game::Reset(){
	scene->RemoveAllActors();


	rapidjson::Document document;
	bool success = gme::json::Load("title.json", document);
	assert(success);
	scene->Read(document);

	state = eState::Title;
}

void Game::Title(){
	if (gme::IsButtonPressed(SDL_SCANCODE_SPACE, engine.get())) {
		auto title = scene->FindActor("Title");
		if (title) title->destroy = true;

		auto instruction = scene->FindActor("Instruction");
		if (instruction) instruction->destroy = true;
		state = eState::StartGame;
	}
}

void Game::StartGame(){
	rapidjson::Document document;
	bool success = gme::json::Load("level1.json", document);
	assert(success);
	scene->Read(document);

	gme::Tilemap tilemap;
	tilemap.scene = scene.get();
	success = gme::json::Load("map1.json", document);
	assert(success);
	tilemap.Read(document);
	tilemap.Create();

	success = gme::json::Load("timer.json", document);
	assert(success);
	scene->Read(document);

	surviveTimer = 0;

	state = eState::StartLevel;
	stateTimer = 0;
}

void Game::StartLevel(){
	stateTimer += engine->time.deltaTime;

	if (!scene->FindActor("Enemy") && stateTimer >= 0.5f) {
		for (int i = 0; i < 5; i++) {
			auto enemy = gme::ObjectFactory::Instance().Create<gme::Actor>("Enemy");
			enemy->transform.position = gme::Vector2{ gme::RandomRange(100,700), gme::RandomRange(100,500) };
			scene->AddActor(std::move(enemy));
		}
	}

	if (stateTimer >= 1) {
		auto player = gme::ObjectFactory::Instance().Create<gme::Actor>("Player");
		if (playerYPos != -100) player->transform.position = { playerXPos, playerYPos };
		scene->AddActor(std::move(player));

		spawnTimer = 2;
		stateTimer = 0;
		state = eState::Level;
	}
}

void Game::Level(){
	auto player = scene->FindActor("Player");
	if (!player) {
		stateTimer += engine->time.deltaTime;
		if(stateTimer >= 0.5f) state = eState::PlayerDead;
	}
}

void Game::StartLevel2(){
 	stateTimer += engine->time.deltaTime;

	if (!scene->FindActor("Enemy") && stateTimer >= 0.5f) {
		for (int i = 0; i < 5; i++) {
			auto enemy = gme::ObjectFactory::Instance().Create<gme::Actor>("Enemy");
			enemy->transform.position = gme::Vector2{ gme::RandomRange(100,700), gme::RandomRange(100,500) };
			scene->AddActor(std::move(enemy));
		}
	}

	if (stateTimer >= 1) {
		auto player = gme::ObjectFactory::Instance().Create<gme::Actor>("Player");
		if (playerYPos != -100) player->transform.position = { playerXPos, playerYPos };
		scene->AddActor(std::move(player));

		spawnTimer = 2;
		stateTimer = 0;
		state = eState::Level;
	}
}

void Game::Level2()
{
	auto player = scene->FindActor("Player");
	if (!player) {
		stateTimer += engine->time.deltaTime;
		if (stateTimer >= 0.5f) state = eState::PlayerDead;
	}
}

void Game::PlayerDead(){
	scene->RemoveAllActors();
	rapidjson::Document document;
	bool success = gme::json::Load("gameover.json", document);
	assert(success);
	scene->Read(document);
	state = eState::GameOver;
}

void Game::GameOver(){
	if (gme::IsButtonPressed(SDL_SCANCODE_SPACE, engine.get())) {
		state = eState::Reset;
	}
}

void Game::ChangeLevel(const gme::Event& event){
	int levelGoingTo = std::get<int>(event.data);
	playerXPos = scene->FindActor("Player")->transform.position.x;

	scene->RemoveAllActors();

	if (levelGoingTo == 1) {
		rapidjson::Document document;
		bool success = gme::json::Load("level1.json", document);
		assert(success);
		scene->Read(document);

		gme::Tilemap tilemap;
		tilemap.scene = scene.get();
		success = gme::json::Load("map1.json", document);
		assert(success);
		tilemap.Read(document);
		tilemap.Create();

		success = gme::json::Load("timer.json", document);
		assert(success);
		scene->Read(document);

		playerYPos = 590;

		state = eState::StartLevel;
		stateTimer = 0;
	}
	else if (levelGoingTo == 2) {
		rapidjson::Document document;
		bool success = gme::json::Load("level2.json", document);
 		assert(success);
		scene->Read(document);

		gme::Tilemap tilemap;
		tilemap.scene = scene.get();
		success = gme::json::Load("map2.json", document);
		assert(success);
		tilemap.Read(document);
		tilemap.Create();
		
		success = gme::json::Load("timer.json", document);
		assert(success);
		scene->Read(document);

		playerYPos = 10;

		state = eState::StartLevel2;
		stateTimer = 0;
	}
}