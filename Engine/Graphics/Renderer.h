#pragma once
#include "Framework/System.h"
#include "Graphics/Texture.h"
#include "Math/Transform.h"
#include <SDL.h>
#include <string>

namespace gme {

	class Renderer : public System {
	public:
		void Startup() override;
		void Shutdown() override;
		void Update(float dt) override;

		void Create(const std::string& windowName, int width, int height);
		void BeginFrame();
		void EndFrame();

		void Draw(std::shared_ptr<Texture> texture, const Vector2& position, const Vector2& scale = Vector2::one, float angle = 0);
		void Draw(std::shared_ptr<Texture> texture, const Transform& transform);

		friend class Texture;

	private:
		SDL_Renderer* renderer{ nullptr };
		SDL_Window* window{ nullptr };
	};
}