#include "EnemyComponent.h"
#include "Engine.h"

using namespace gme;
EnemyComponent::~EnemyComponent()
{
	owner->scene->engine->Get<EventSystem>()->Unsubscribe("collision_enter", owner);
	owner->scene->engine->Get<EventSystem>()->Unsubscribe("collision_exit", owner);
}

void EnemyComponent::Update() {
	Actor* player = owner->scene->FindActor("Player");
	if (player) {
		Vector2 direction = player->transform.position - owner->transform.position;
 		Vector2 force = direction.Normalized() * speed;


		PhysicsComponent* physicsComponent = owner->GetComponent<PhysicsComponent>();
 		assert(physicsComponent);
		owner->transform.position.x = Wrap(owner->transform.position.x, 0.0f, 800.0f);
		owner->transform.position.y = Wrap(owner->transform.position.y, 0.0f, 600.0f);
		physicsComponent->ApplyForce(force);
	}


}

bool EnemyComponent::Write(const rapidjson::Value& value) const {
	return false;
}

bool EnemyComponent::Read(const rapidjson::Value& value) {
	JSON_READ(value, speed);

	return true;
}

