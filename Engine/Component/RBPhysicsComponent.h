#pragma once
#include "Component/PhysicsComponent.h"
#include "Physics/PhysicsSystem.h"

namespace gme {
	class RBPhysicsComponent : public PhysicsComponent {
	public:
		void Update() override;
		void ApplyForce(const Vector2& force) override;

		virtual bool Write(const rapidjson::Value& value) const override;
		virtual bool Read(const rapidjson::Value& value) override;
	public:
		PhysicsSystem::RigidBodyData data;
		b2Body* body{ nullptr };
	};
}