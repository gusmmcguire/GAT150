#pragma once
#include "Component/GraphicsComponent.h"
#include <memory>

namespace gme {
	class SpriteComponent : public GraphicsComponent {
	public:
		void Update() override;
		void Draw(class Renderer* renderer) override;

		virtual bool Write(const rapidjson::Value& value) const override;
		virtual bool Read(const rapidjson::Value& value) override;
	public:
		std::shared_ptr<class Texture> texture;

	};
}