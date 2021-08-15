#include "ParticleSystem.h"
#include "Math/Random.h"
#include "Graphics/Renderer.h"
#include "Graphics/Texture.h"

namespace gme {

	void ParticleSystem::Startup(){
		particles.resize(10000);
	}

	void ParticleSystem::Shutdown(){
		particles.clear();
	}

	void ParticleSystem::Update(float dt){
		for (Particle& particle : particles) {
			if (particle.isActive) {
				particle.lifetime -= dt;
				particle.isActive = particle.lifetime > 0;
				particle.prevPosition = particle.position;
				particle.position += particle.velocity * dt;
			}
		}
	}

	void ParticleSystem::Draw(Renderer* renderer){
		for (const Particle& particle : particles) {
			if (particle.isActive) {
				renderer->Draw(particle.texture, particle.position, particle.scale);
			}
		}
	}

	void ParticleSystem::Create(const Vector2& position, size_t count, float lifetime, std::shared_ptr<Texture> texture, float speed, const Vector2& scale){
		for (size_t i = 0; i < count; i++) {
			auto particle = std::find_if(particles.begin(), particles.end(), Particle::IsNotActive);
			if (particle != particles.end()) {
				particle->isActive = true;
				particle->lifetime = lifetime;
				particle->position = position;
				particle->prevPosition = position;
				particle->texture = texture;
				particle->scale = scale;

				particle->velocity = Vector2{ RandomRange(-1,1),RandomRange(-1,1)} * (speed * Random());
			}
		}
	}
	
	void ParticleSystem::Create(const Vector2& position, size_t count, float lifetime, std::shared_ptr<Texture> texture, float speed, float angle, float angleRange, const Vector2& scale){
		for (size_t i = 0; i < count; i++) {
			auto particle = std::find_if(particles.begin(), particles.end(), Particle::IsNotActive);
			if (particle != particles.end()) {
				particle->isActive = true;
				particle->lifetime = lifetime;
				particle->position = position;
				particle->prevPosition = position;
				particle->texture = texture;
				particle->scale = scale;

				particle->velocity = Vector2::Rotate(Vector2::right, angle + RandomRange(-angleRange,angleRange)) * (speed * Random());
			}
		}
	}
}