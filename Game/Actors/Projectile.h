#pragma once
#include "Object/Actor.h"

class Projectile : public gme::Actor {
public:
	Projectile(const gme::Transform& transform, std::shared_ptr<gme::Texture> texture, float speed) : gme::Actor{ transform,texture }, speed{ speed }{}

	void Initialize() override;

	void Update(float dt) override;

private:
	float lifetime{ 1 };
	float speed{ 250 };
};