#pragma once
#include "Resource/Resource.h"
#include "Renderer.h"
#include "Math/Color.h"
#include <SDL_ttf.h>

namespace gme {
	class Font : public Resource {
	public:
		~Font();

		bool Load(const std::string& name, void* data) override;
		SDL_Surface* CreateSurface(const std::string& text, const gme::Color & color);

	private:
		TTF_Font* font{ nullptr };
		Renderer* renderer{ nullptr };
	};
}