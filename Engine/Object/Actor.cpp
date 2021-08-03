#include "Actor.h"
#include "Math/MathUtils.h"
#include <algorithm>

namespace gme {
	void Actor::Update(float dt){
		transform.Update();
		std::for_each(children.begin(), children.end(), [](auto& child) {child->transform.Update(child->parent->transform.matrix); });
	}

	void Actor::Draw(){
	}

	void Actor::addChild(std::unique_ptr<Actor> actor){
		actor->parent = this;
		children.push_back(std::move(actor));
	}

	float Actor::GetRadius(){
		return 0;
	}
}