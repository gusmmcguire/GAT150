#include "Actor.h"
#include "Math/MathUtils.h"
#include "Graphics/Renderer.h"
#include <algorithm>

namespace gme {
	void Actor::Update(float dt){
		transform.rotation += .2;
		transform.rotation = gme::Wrap(transform.rotation, 0.0f, 360.0f);

		transform.Update();
		std::for_each(children.begin(), children.end(), [](auto& child) {child->transform.Update(child->parent->transform.matrix); });
	}

	void Actor::Draw(Renderer* renderer){
		renderer->Draw(texture, transform);
	}

	void Actor::addChild(std::unique_ptr<Actor> actor){
		actor->parent = this;
		children.push_back(std::move(actor));
	}

	float Actor::GetRadius(){
		return std::max(texture->GetSize().x, texture->GetSize().y) * 0.5f;
	}
}