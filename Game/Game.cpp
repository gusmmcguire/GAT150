#include "Game.h"
#include "Actors/Player.h"
#include "Actors/Enemy.h"
#include "Actors/RoomBlock.h"

void Game::Initialize() {
	state = eState::Title;
	//create engine
	engine = std::make_unique<gme::Engine>();
	engine->Startup();
	engine->Get<gme::Renderer>()->Create("GAT150", 800, 600);
	//create scene
	scene = std::make_unique<gme::Scene>();
	scene->engine = engine.get();
	//change file path
	gme::SetFilePath("../Resources");

	//get audio
	engine->Get<gme::AudioSystem>()->AddAudio("explosion", "Audio/explosion.wav");
	engine->Get<gme::AudioSystem>()->AddAudio("backingMusic", "Audio/backing.wav");
	//start backing music
	musicChannel = engine->Get<gme::AudioSystem>()->PlayAudio("backingMusic", .2, 1, true);

	
	//make font
	int size = 16;
	font = engine->Get<gme::ResourceSystem>()->Get<gme::Font>("Fonts/AlexandriaFLF.ttf", &size);
	//make font texture and display to screen
	textTexture = std::make_shared<gme::Texture>(engine->Get<gme::Renderer>());
	textTexture->Create(font->CreateSurface("Game Title", gme::Color{ 1,1,1 }));
	engine->Get<gme::ResourceSystem>()->Add("textTexture", textTexture);

	//subscribe to events
	engine->Get<gme::EventSystem>()->Subscribe("PlayerDead", std::bind(&Game::OnPlayerDead,this,std::placeholders::_1));
	engine->Get<gme::EventSystem>()->Subscribe("AddPoints", std::bind(&Game::OnAddPoints,this,std::placeholders::_1));
}

void Game::Shutdown(){
	scene->RemoveAllActors();
	engine->Shutdown();
}

void Game::Update(){
	
	engine->Update();
	stateTimer += engine->time.deltaTime;

	switch (state)
	{
	case Game::eState::Title:
		scene->RemoveAllActors();
		if (engine->Get<gme::InputSystem>()->GetKeyState(SDL_SCANCODE_SPACE) == gme::InputSystem::eKeyState::Pressed){
			state = eState::StartGame;
		}
		break;
	case Game::eState::StartGame:
		score = 0;
		lives = 3;
		state = eState::StartLevel1;
		break;
	case Game::eState::StartLevel1:
	{
		//create position for player based on whether or not a player currently exisists
		gme::Vector2 playerPosition = { 100.0f, 100.0f };
		float rotation = gme::HalfPi;
		if (scene->GetActor<Player>()) {
			playerPosition = { scene->GetActor<Player>()->transform.position.x, 599.0f };
			rotation = scene->GetActor<Player>()->transform.rotation;
		}
		//remove all actors for a fresh scene
		scene->RemoveAllActors();
		//add in the player at the calculated position
		scene->AddActor(std::make_unique<Player>(gme::Transform{ playerPosition, rotation, 2.5f }, engine->Get<gme::ResourceSystem>()->Get<gme::Texture>("ship.png", engine->Get<gme::Renderer>()), 750.0f));

		//add in the enemy
		scene->AddActor(std::make_unique<Enemy>(gme::Transform{ gme::Vector2{700.0f,101.0f}, gme::HalfPi, 3.0f }, engine->Get<gme::ResourceSystem>()->Get<gme::Texture>("enemy.png", engine->Get<gme::Renderer>()), 10.0f));
		scene->AddActor(std::make_unique<Enemy>(gme::Transform{ gme::Vector2{201.0f,300.0f}, 0.0f, 3.0f }, engine->Get<gme::ResourceSystem>()->Get<gme::Texture>("enemy.png", engine->Get<gme::Renderer>()), 10.0f));
		
		//make the first room
		MakeRoomOne();
		//change state to start game
		state = eState::Game1;
	}
		break;
	case Game::eState::Game1:
		if (scene->GetActor<Player>() && scene->GetActor<Player>()->transform.position.y >= 600.0f) state = eState::StartLevel2;
		break;
	case Game::eState::StartLevel2:
	{
		//create position for player based on whether or not a player currently exisists
 		gme::Vector2 playerPosition = { 100.0f, 100.0f }; 
		float rotation = gme::HalfPi;
		if (scene->GetActor<Player>()) {
			playerPosition = { scene->GetActor<Player>()->transform.position.x, 1.0f };
			rotation = scene->GetActor<Player>()->transform.rotation;
		}
		//remove all actors for a fresh scene
		scene->RemoveAllActors();
		//add in the player at the calculated position
		scene->AddActor(std::make_unique<Player>(gme::Transform{ playerPosition, rotation, 2.5f }, engine->Get<gme::ResourceSystem>()->Get<gme::Texture>("ship.png", engine->Get<gme::Renderer>()), 750.0f));
		//add in the enemy
		scene->AddActor(std::make_unique<Enemy>(gme::Transform{ gme::Vector2{700.0f,101.0f}, gme::HalfPi, 3.0f }, engine->Get<gme::ResourceSystem>()->Get<gme::Texture>("enemy.png", engine->Get<gme::Renderer>()), 10.0f));
		scene->AddActor(std::make_unique<Enemy>(gme::Transform{ gme::Vector2{201.0f,300.0f}, 0.0f, 3.0f }, engine->Get<gme::ResourceSystem>()->Get<gme::Texture>("enemy.png", engine->Get<gme::Renderer>()), 10.0f));

		//make the first room
		MakeRoomTwo();
		//change state to start game
		state = eState::Game2;
	}
	break;
	case Game::eState::Game2:
		if (scene->GetActor<Player>() && scene->GetActor<Player>()->transform.position.y <= 0.0f) state = eState::StartLevel1;
		break;
	case Game::eState::GameOver:
		scene->RemoveAllActors();
		if (gme::IsButtonPressed(SDL_SCANCODE_SPACE, scene->engine)) {
			state = eState::StartGame;
		}
		break;
	case Game::eState::GameWin:
		scene->RemoveAllActors();
		if (gme::IsButtonPressed(SDL_SCANCODE_SPACE, scene->engine)) {
			state = eState::StartGame;
		}
		break;
	default:
		break;
	}

	//if (!quit) quit = gme::IsButtonPressed(SDL_SCANCODE_ESCAPE, engine.get());
	
	/*if (engine->Get<gme::InputSystem>()->GetButtonState((int)gme::InputSystem::eMouseButton::Left) == gme::InputSystem::eKeyState::Pressed) {
		engine->Get<gme::ParticleSystem>()->Create(engine->Get<gme::InputSystem>()->GetMousePosition(), 50, .5, engine->Get<gme::ResourceSystem>()->Get<gme::Texture>("particle01.png", engine->Get<gme::Renderer>()), 300);
		engine->Get<gme::AudioSystem>()->PlayAudio("explosion", 1, gme::RandomRange(0.2, 2));
		musicChannel.SetPitch(gme::RandomRange(-2, 2));
	}*/
	scene->Update(engine->time.deltaTime);
}

void Game::Draw(){
	engine->Get<gme::Renderer>()->BeginFrame();
	engine->Draw(engine->Get<gme::Renderer>());
	scene->Draw(engine->Get<gme::Renderer>());
	switch (state)
	{
	case Game::eState::Title:
		textTexture->Create(font->CreateSurface("Game Title", gme::Color{ 1,1,1 }));
		engine->Get<gme::Renderer>()->Draw(textTexture, gme::Transform{ {400, 290 + static_cast<int>(std::sin(stateTimer * 4) * 10)}});
		textTexture->Create(font->CreateSurface("Press Space To Start Game", gme::Color{ 1,1,1 }));
		engine->Get<gme::Renderer>()->Draw(textTexture, gme::Transform{ {400, 305 + static_cast<int>(std::sin(stateTimer * 4) * 10)}});
		break;
	case Game::eState::Game1:
		textTexture->Create(font->CreateSurface("Score: " + std::to_string(score), gme::Color{ 1,1,1 }));
		engine->Get<gme::Renderer>()->Draw(textTexture, gme::Transform{ {100, 17 + static_cast<int>(std::sin(stateTimer * 4) * 5)} });
		textTexture->Create(font->CreateSurface("Lives: " + std::to_string(lives), gme::Color{ 1,1,1 }));
		engine->Get<gme::Renderer>()->Draw(textTexture, gme::Transform{ {700, 17 + static_cast<int>(std::sin(stateTimer * 4) * 5)} });
		break;
	case Game::eState::Game2:
		textTexture->Create(font->CreateSurface("Score: " + std::to_string(score), gme::Color{ 1,1,1 }));
		engine->Get<gme::Renderer>()->Draw(textTexture, gme::Transform{ {100, 17 + static_cast<int>(std::sin(stateTimer * 4) * 5)} });
		textTexture->Create(font->CreateSurface("Lives: " + std::to_string(lives), gme::Color{ 1,1,1 }));
		engine->Get<gme::Renderer>()->Draw(textTexture, gme::Transform{ {700, 17 + static_cast<int>(std::sin(stateTimer * 4) * 5)} });
		break;
	case Game::eState::GameOver:
		textTexture->Create(font->CreateSurface("Game Over", gme::Color{ 1,0,0 }));
		engine->Get<gme::Renderer>()->Draw(textTexture, gme::Transform{ {400, 290 + static_cast<int>(std::sin(stateTimer * 4) * 10)} });
		textTexture->Create(font->CreateSurface("Press Space To Restart Game", gme::Color{ 1,1,1 }));
		engine->Get<gme::Renderer>()->Draw(textTexture, gme::Transform{ {400, 305 + static_cast<int>(std::sin(stateTimer * 4) * 10)} });
		break;
	case Game::eState::GameWin:
		textTexture->Create(font->CreateSurface("You Win", gme::Color{ 1,0,0 }));
		engine->Get<gme::Renderer>()->Draw(textTexture, gme::Transform{ {400, 290 + static_cast<int>(std::sin(stateTimer * 4) * 10)} });
		textTexture->Create(font->CreateSurface("Press Space To Restart Game", gme::Color{ 1,1,1 }));
		engine->Get<gme::Renderer>()->Draw(textTexture, gme::Transform{ {400, 305 + static_cast<int>(std::sin(stateTimer * 4) * 10)} });
		break;
	default:
		break;
	}

	

	engine->Get<gme::Renderer>()->EndFrame();
}

void Game::OnPlayerDead(const gme::Event& event) {
	//std::cout << std::get<std::string>(event.data) << std::endl;
	if (lives <= 0) state = eState::GameOver;
	else {
		lives--;
		if (state == eState::Game1) state = eState::StartLevel1;
		else state = eState::StartLevel2;
	}
}

void Game::OnAddPoints(const gme::Event& event) {
	score += std::get<int>(event.data);
	if (score >= 400) state = eState::GameWin;
}

void Game::MakeRoomOne() {
	scene->AddActor(std::make_unique<RoomBlock>(gme::Transform{ gme::Vector2{400.0f,175.0f}, gme::HalfPi, gme::Vector2{5.0f,15.0f} }, engine->Get<gme::ResourceSystem>()->Get<gme::Texture>("wall.png", engine->Get<gme::Renderer>())));
	scene->AddActor(std::make_unique<RoomBlock>(gme::Transform{ gme::Vector2{400.0f,425.0f}, gme::HalfPi, gme::Vector2{5.0f,15.0f} }, engine->Get<gme::ResourceSystem>()->Get<gme::Texture>("wall.png", engine->Get<gme::Renderer>())));

	scene->AddActor(std::make_unique<RoomBlock>(gme::Transform{ gme::Vector2{15.0f,300.0f}, 0, gme::Vector2{2.0f,20.0f} }, engine->Get<gme::ResourceSystem>()->Get<gme::Texture>("wall.png", engine->Get<gme::Renderer>())));
	scene->AddActor(std::make_unique<RoomBlock>(gme::Transform{ gme::Vector2{785.0f,300.0f}, 0, gme::Vector2{2.0f,20.0f} }, engine->Get<gme::ResourceSystem>()->Get<gme::Texture>("wall.png", engine->Get<gme::Renderer>())));

	scene->AddActor(std::make_unique<RoomBlock>(gme::Transform{ gme::Vector2{400.0f,0.0f}, gme::HalfPi, gme::Vector2{5.0f,24.0f} }, engine->Get<gme::ResourceSystem>()->Get<gme::Texture>("wall.png", engine->Get<gme::Renderer>())));
}

void Game::MakeRoomTwo() {
	scene->AddActor(std::make_unique<RoomBlock>(gme::Transform{ gme::Vector2{400.0f,175.0f}, gme::HalfPi, gme::Vector2{5.0f,15.0f} }, engine->Get<gme::ResourceSystem>()->Get<gme::Texture>("wall.png", engine->Get<gme::Renderer>())));
	scene->AddActor(std::make_unique<RoomBlock>(gme::Transform{ gme::Vector2{400.0f,425.0f}, gme::HalfPi, gme::Vector2{5.0f,15.0f} }, engine->Get<gme::ResourceSystem>()->Get<gme::Texture>("wall.png", engine->Get<gme::Renderer>())));

	scene->AddActor(std::make_unique<RoomBlock>(gme::Transform{ gme::Vector2{15.0f,300.0f}, 0, gme::Vector2{2.0f,20.0f} }, engine->Get<gme::ResourceSystem>()->Get<gme::Texture>("wall.png", engine->Get<gme::Renderer>())));
	scene->AddActor(std::make_unique<RoomBlock>(gme::Transform{ gme::Vector2{785.0f,300.0f}, 0, gme::Vector2{2.0f,20.0f} }, engine->Get<gme::ResourceSystem>()->Get<gme::Texture>("wall.png", engine->Get<gme::Renderer>())));

	scene->AddActor(std::make_unique<RoomBlock>(gme::Transform{ gme::Vector2{400.0f,600.0f}, gme::HalfPi, gme::Vector2{5.0f,24.0f} }, engine->Get<gme::ResourceSystem>()->Get<gme::Texture>("wall.png", engine->Get<gme::Renderer>())));
}