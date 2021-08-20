#include "Game.h"

int main(int, char**){

	Game game;
	game.Initialize();

	rapidjson::Document document;
	bool success = gme::json::Load("json.txt", document);
	assert(success);

	std::string str;
	gme::json::Get(document, "string", str);
	std::cout << str << std::endl;

	bool b;
	gme::json::Get(document, "boolean", b);
	std::cout << b << std::endl;

	int i1;
	gme::json::Get(document, "integer1", i1);
	std::cout << i1 << std::endl;

	int i2;
	gme::json::Get(document, "integer2", i2);
	std::cout << i2 << std::endl;

	float f;
	gme::json::Get(document, "float", f);
	std::cout << f << std::endl;

	gme::Vector2 v2;
	gme::json::Get(document, "vector2", v2);
	std::cout << v2 << std::endl;

	gme::Color color;
	gme::json::Get(document, "color", color);
	std::cout << color << std::endl;

	bool quit = false;
	SDL_Event event;

	while (!game.IsQuit() && !quit) {
		SDL_PollEvent(&event);
		switch (event.type)
		{
		case SDL_QUIT:
			quit = true;
			break;
		}
		game.Update();
		game.Draw();
	}

	game.Shutdown();
	SDL_Quit();

	return 0;
}