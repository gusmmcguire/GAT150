#include "Actor.h"
#include "Math/MathUtils.h"
#include "Math/Random.h"
#include "Graphics/Renderer.h"
#include <algorithm>

namespace gme {
	void Actor::Update(float dt){
		transform.rotation += 90 * dt;
		transform.rotation = Wrap(transform.rotation, 0.0f, 360.0f);

		transform.position.x += 200.0f * dt;
		transform.position.x = Wrap(transform.position.x, 0.0f, 800.0f);


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
		return (texture) ? texture->GetSize().Length() * .05f : 0;
	}
}