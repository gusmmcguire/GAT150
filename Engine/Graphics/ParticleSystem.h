#pragma once
#include "Framework/System.h"
#include "Math/Vector2.h"
#include "Math/Color.h"
#include <vector>

namespace gme {
	class Renderer;
	class Texture;

	class ParticleSystem : public GraphicsSystem {
	private:
		struct Particle {
			Vector2 position;
			Vector2 prevPosition;
			Vector2 velocity;
			Vector2 scale;
			std::shared_ptr<Texture> texture;

			float lifetime;
			bool isActive{ false };

			static bool IsNotActive(Particle particle) { return !particle.isActive; }
		};

	public:
		void Startup() override;
		void Shutdown() override;
		
		void Update(float dt) override;
		void Draw(Renderer* renderer) override;

		void Create(const Vector2& position, size_t count, float lifetime, std::shared_ptr<Texture> texture, float speed, const Vector2& scale = Vector2::one);
		void Create(const Vector2& position, size_t count, float lifetime, std::shared_ptr<Texture> texture, float speed, float angle, float angleRange, const Vector2& scale = Vector2::one);

	private:
		std::vector<Particle> particles;
	};
}