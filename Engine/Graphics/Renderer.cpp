#include "Renderer.h"
#include "Math/MathUtils.h"
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>

namespace gme {
	void Renderer::Startup(){
		if (SDL_Init(SDL_INIT_VIDEO) != 0)
		{
			std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		}

		IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
		TTF_Init();
	}

	void Renderer::Shutdown(){
		IMG_Quit();
		TTF_Quit();
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
	}

	void Renderer::Update(float dt){

	}

	void Renderer::Create(const std::string& windowName, int width, int height){
		window = SDL_CreateWindow(windowName.c_str(), 100, 100, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
		if (window == nullptr)
		{
			std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
			SDL_Quit();
		}

		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
	}

	void Renderer::BeginFrame(){
		SDL_RenderClear(renderer);
	}

	void Renderer::EndFrame(){
		SDL_RenderPresent(renderer);
	}
	
	void Renderer::Draw(std::shared_ptr<Texture> texture, const Vector2& position, const Vector2& scale, float angle){
		Vector2 size = texture->GetSize() * scale;
		Vector2 newPosition = position - (size * 0.5f);

		SDL_Rect dest;
		dest.x = static_cast<int>(newPosition.x);
		dest.y = static_cast<int>(newPosition.y);
		dest.w = static_cast<int>(size.x);
		dest.h = static_cast<int>(size.y);

		
		SDL_RenderCopyEx(renderer, texture->texture, nullptr, &dest, gme::RadToDeg(angle), nullptr, SDL_FLIP_NONE);
	}
	void Renderer::Draw(std::shared_ptr<Texture> texture, const Transform& transform)
	{
		Vector2 size = texture->GetSize() * transform.scale;
		Vector2 newPosition = transform.position - (size * 0.5f);

		SDL_Rect dest;
		dest.x = static_cast<int>(newPosition.x);
		dest.y = static_cast<int>(newPosition.y);
		dest.w = static_cast<int>(size.x);
		dest.h = static_cast<int>(size.y); 
		
		SDL_RenderCopyEx(renderer, texture->texture, nullptr, &dest, gme::RadToDeg(transform.rotation), nullptr, SDL_FLIP_NONE);
	}
}