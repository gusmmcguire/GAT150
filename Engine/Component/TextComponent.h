#pragma once
#include "Component/GraphicsComponent.h"
#include "Graphics/Renderer.h"
#include "Math/Color.h"

namespace gme {
	class Texture;
	class Font;

	class TextComponent : public GraphicsComponent {
	public:
		void Update() override;
		void Draw(Renderer* renderer) override;

		void SetText(const std::string& text);

		bool Write(const rapidjson::Value& value) const override;
		bool Read(const rapidjson::Value& value) override;

	public:
		std::string fontName;
		int fontSize;
		Color color;
		std::string text;

		std::shared_ptr<Font> font;
		std::shared_ptr<Texture> texture;
	};
}