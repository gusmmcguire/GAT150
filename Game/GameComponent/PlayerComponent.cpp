#include "PlayerComponent.h"
#include "Engine.h"

using namespace gme;

void PlayerComponent::Create(){
	owner->scene->engine->Get<EventSystem>()->Subscribe("collision_enter", std::bind(&PlayerComponent::OnCollisionEnter, this, std::placeholders::_1), owner);
	owner->scene->engine->Get<EventSystem>()->Subscribe("collision_exit", std::bind(&PlayerComponent::OnCollisionExit, this, std::placeholders::_1), owner);
	owner->scene->engine->Get<AudioSystem>()->AddAudio("hurt", "audio/hurt.wav");

}

void PlayerComponent::Update() {
	Vector2 force = Vector2::zero;
	if (IsButtonHeld(SDL_SCANCODE_D, owner->scene->engine)) {
		force.x += speed;
	}
	if (IsButtonHeld(SDL_SCANCODE_A,owner->scene->engine)) {
		force.x -= speed;
	}
	if (contacts.size() > 0 && IsButtonPressed(SDL_SCANCODE_SPACE, owner->scene->engine)) {
		force.y -= jumpSpeed;
	}
	PhysicsComponent* physicsComponent = owner->GetComponent<PhysicsComponent>();
	assert(physicsComponent);

	physicsComponent->ApplyForce(force);
	
	SpriteAnimationComponent* spriteAnimationComponent = owner->GetComponent<SpriteAnimationComponent>();
	assert(spriteAnimationComponent);
	
	if (physicsComponent->velocity.x > 0.1) spriteAnimationComponent->StartSequence("walk_right");
	else if (physicsComponent->velocity.x < -0.1) spriteAnimationComponent->StartSequence("walk_left");
	else spriteAnimationComponent->StartSequence("idle");
}

void PlayerComponent::OnCollisionEnter(const Event& event){
	void* p = std::get<void*>(event.data);
	Actor* actor = reinterpret_cast<Actor*>(p);

	if (istring_compare(actor->tag, "ground")) {
		contacts.push_back(actor);
	}

	if (istring_compare(actor->tag, "enemy")) {
		owner->scene->engine->Get<AudioSystem>()->PlayAudio("hurt");
	}
}

void PlayerComponent::OnCollisionExit(const Event& event){
	void* p = std::get<void*>(event.data);
	Actor* actor = reinterpret_cast<Actor*>(p);

	if (istring_compare(actor->tag, "ground")) {
		contacts.remove(actor);
	}

	
}

bool PlayerComponent::Write(const rapidjson::Value& value) const {
	return false;
}

bool PlayerComponent::Read(const rapidjson::Value& value) {
	JSON_READ(value, speed);
	JSON_READ(value, jumpSpeed);
	return true;
}

