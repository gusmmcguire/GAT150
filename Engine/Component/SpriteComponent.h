#pragma once
#include "Component/GraphicsComponent.h"
#include <memory>

namespace gme {
	class SpriteComponent : public GraphicsComponent {
	public:
		void Update() override;
		void Draw(class Renderer* renderer) override;

	public:
		std::shared_ptr<class Texture> texture;
	};
}