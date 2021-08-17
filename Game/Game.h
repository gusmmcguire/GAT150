#pragma once
#include "Engine.h"
#include <SDL.h>
#include <SDL_Image.h>
#include <iostream>
#include <cassert>

class Game {
public:
	enum class eState {
		Title,
		StartGame,
		StartLevel1,
		Game1,
		StartLevel2,
		Game2,
		GameOver,
		GameWin
	};

public:
	void Initialize();
	void Shutdown();

	void Update();
	void Draw();

	bool IsQuit() { return quit; }

private:
	void OnPlayerDead(const gme::Event& event);
	void OnAddPoints(const gme::Event& event);

	void MakeRoomOne();
	void MakeRoomTwo();

public:
	std::unique_ptr<gme::Engine> engine;
	std::unique_ptr<gme::Scene> scene;

private:
	bool quit = false;
	eState state = eState::Title;
	float stateTimer = 0.0f;

	size_t score = 0;
	size_t lives = 0;

	gme::AudioChannel musicChannel;
	std::shared_ptr<gme::Texture> textTexture;
	std::shared_ptr<gme::Font> font;
};