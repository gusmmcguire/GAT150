#pragma once
#include "Engine.h"
#include <SDL.h>
#include <SDL_Image.h>
#include <iostream>
#include <cassert>

class Game {
public:
	enum class eState {
		Reset,
		Title,
		StartGame,
		StartLevel,
		Level,
		StartLevel2,
		Level2,
		PlayerDead,
		GameOver
	};

public:
	void Initialize();
	void Shutdown();

	void Update();
	void Draw();

	bool IsQuit() { return quit; }

private:
	void Reset();
	void Title();
	void StartGame();
	void StartLevel();
	void Level();
	void StartLevel2();
	void Level2();
	void PlayerDead();
	void GameOver();

	void ChangeLevel(const gme::Event& event);

public:
	std::unique_ptr<gme::Engine> engine;
	std::unique_ptr<gme::Scene> scene;

private:
	bool quit = false;
	eState state = eState::Reset;
	float stateTimer = 0;
	float spawnTimer = 0;
	float surviveTimer = 0;
	int playerXPos = 100;
	int playerYPos = -100;
};