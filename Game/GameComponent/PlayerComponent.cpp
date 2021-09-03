#include "PlayerComponent.h"
#include "Engine.h"

using namespace gme;
PlayerComponent::~PlayerComponent()
{
	owner->scene->engine->Get<EventSystem>()->Unsubscribe("collision_enter", owner);
	owner->scene->engine->Get<EventSystem>()->Unsubscribe("collision_exit", owner);
}

void PlayerComponent::Create(){
	owner->scene->engine->Get<EventSystem>()->Subscribe("collision_enter", std::bind(&PlayerComponent::OnCollisionEnter, this, std::placeholders::_1), owner);
	owner->scene->engine->Get<EventSystem>()->Subscribe("collision_exit", std::bind(&PlayerComponent::OnCollisionExit, this, std::placeholders::_1), owner);
	owner->scene->engine->Get<AudioSystem>()->AddAudio("hurt", "audio/hurt.wav");

}

void PlayerComponent::Update() {
	SpriteAnimationComponent* spriteAnimationComponent = owner->GetComponent<SpriteAnimationComponent>();
	assert(spriteAnimationComponent);
	PhysicsComponent* physicsComponent = owner->GetComponent<PhysicsComponent>();
	assert(physicsComponent);
	Vector2 force = Vector2::zero;

	if (IsButtonHeld(SDL_SCANCODE_D, owner->scene->engine)) {
		spriteAnimationComponent->StartSequence("walk_right");
		force.x += speed;
	}
	if (IsButtonHeld(SDL_SCANCODE_A, owner->scene->engine)) {
		spriteAnimationComponent->StartSequence("walk_left");
		force.x -= speed;
	}
	if (IsButtonHeld(SDL_SCANCODE_S, owner->scene->engine)) {
		spriteAnimationComponent->StartSequence("walk_down");
		force.y += speed;
	}
	if (IsButtonHeld(SDL_SCANCODE_W, owner->scene->engine)) {
		spriteAnimationComponent->StartSequence("walk_up");
		force.y -= speed;
	}
	if (force.x == 0 && force.y == 0) spriteAnimationComponent->StartSequence("idle");
	
	physicsComponent->ApplyForce(force);
	
	if (owner->transform.position.y >= 595) {
		Event event;
		event.name = "change_level";
		event.data = 2;
		owner->scene->engine->Get<EventSystem>()->Notify(event);
	}
	else if (owner->transform.position.y <= 5) {
		Event event;
		event.name = "change_level";
		event.data = 1;
		owner->scene->engine->Get<EventSystem>()->Notify(event);
	}
}

void PlayerComponent::OnCollisionEnter(const Event& event){
	void* p = std::get<void*>(event.data);
	Actor* actor = reinterpret_cast<Actor*>(p);

	if (istring_compare(actor->tag, "enemy")) {
		owner->scene->engine->Get<AudioSystem>()->PlayAudio("hurt");
		owner->destroy = true;
	}
}

void PlayerComponent::OnCollisionExit(const Event& event){
	void* p = std::get<void*>(event.data);
	Actor* actor = reinterpret_cast<Actor*>(p);
}

bool PlayerComponent::Write(const rapidjson::Value& value) const {
	return false;
}

bool PlayerComponent::Read(const rapidjson::Value& value) {
	JSON_READ(value, speed);
	return true;
}

