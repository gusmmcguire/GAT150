#include "Player.h"
#include "Enemy.h"
//#include "RoomBlock.h"
#include "Projectile.h"
#include "Core/Utilities.h"
#include "Math/MathUtils.h"
#include "Engine.h"
#include <SDL.h>
#include <memory>
#include <vector>

Player::Player(const gme::Transform& transform, std::shared_ptr<gme::Texture> texture, float speed) : gme::Actor{ transform,texture }, speed{ speed }{}

void Player::Initialize() {
	std::unique_ptr locator = std::make_unique<Actor>();
	locator->transform.localPosition = gme::Vector2{ 5,0 };
	addChild(std::move(locator));

	locator = std::make_unique<Actor>();
	locator->transform.localPosition = gme::Vector2{ -6,0 };
	locator->transform.localRotation = gme::DegToRad(180);
	addChild(std::move(locator));
}

void Player::Update(float dt) {
	Actor::Update(dt);

	//movement
	float thrust = 0;
	if (gme::IsButtonHeld(SDL_SCANCODE_A, scene->engine) || gme::IsButtonHeld(SDL_SCANCODE_LEFT, scene->engine)) { transform.rotation -= gme::DegToRad(180) * dt; }
	if (gme::IsButtonHeld(SDL_SCANCODE_D, scene->engine) || gme::IsButtonHeld(SDL_SCANCODE_RIGHT, scene->engine)) { transform.rotation += gme::DegToRad(180) * dt; }
	if (gme::IsButtonHeld(SDL_SCANCODE_W, scene->engine) || gme::IsButtonHeld(SDL_SCANCODE_UP, scene->engine)) { thrust = speed; }

	gme::Vector2 acceleration;
	acceleration = gme::Vector2::Rotate(gme::Vector2::right, transform.rotation) * thrust;

	velocity += acceleration * dt;

	transform.position += velocity * dt;

	velocity *= .93f;


	transform.position.x = gme::Clamp(transform.position.x, 0.0f, 800.0f);
	transform.position.y = gme::Clamp(transform.position.y, -1.0f, 600.0f);



	//fire
	fireTimer -= dt;
	if (fireTimer <= 0 && gme::IsButtonPressed(SDL_SCANCODE_SPACE, scene->engine)) {
		fireTimer = fireRate;


		gme::Transform t = children[0]->transform;
		t.scale = transform.scale-0.5f;  
		std::unique_ptr<Projectile> projectile = std::make_unique<Projectile>(t, scene->engine->Get<gme::ResourceSystem>()->Get<gme::Texture>("bullet.png", scene->engine->Get<gme::Renderer>()), 600.0f);
		projectile->tag = "Player";
		scene->AddActor(std::move(projectile));

	}

	//generate particle trail
	if (thrust > 0) {
		std::vector<gme::Color> colors = { gme::Color::white, gme::Color::blue, gme::Color::red };
		scene->engine->Get<gme::ParticleSystem>()->Create(children[1]->transform.position, 2, 0.2, scene->engine->Get<gme::ResourceSystem>()->Get<gme::Texture>("thrust.png", scene->engine->Get<gme::Renderer>()), 150, children[1]->transform.localRotation, gme::DegToRad(30), gme::Vector2{ 1 , 1 });
	}
}

void Player::OnCollision(Actor* actor) {
	if (dynamic_cast<Enemy*>(actor) || (dynamic_cast<Projectile*>(actor) && actor->tag == "Enemy")) {
		scene->engine->Get<gme::AudioSystem>()->PlayAudio("explosion");
		scene->engine->Get<gme::ParticleSystem>()->Create(transform.position, 200, 1, scene->engine->Get<gme::ResourceSystem>()->Get<gme::Texture>("particle01.png", scene->engine->Get<gme::Renderer>()), 50);
		destroy = true;
		actor->destroy = true;

		gme::Event event;
		event.name = "PlayerDead";
		event.data = std::string("yes i'm dead!");
		scene->engine->Get<gme::EventSystem>()->Notify(event);
	}


}
