#pragma once
#include "Component/Component.h"

namespace gme {
	class GraphicsComponent : public Component {
	public:
		virtual void Draw(class Renderer* renderer) = 0;
	};
}