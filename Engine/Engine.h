#pragma once

#define REGISTER_CLASS(CLASS) ObjectFactory::Instance().Register<CLASS>(#CLASS);

//core
#include "Core/Utilities.h"
#include "Core/FileSystem.h"
#include "Core/Timer.h"
#include "Core/Json.h"

//math
#include "Math/Vector2.h"
#include "Math/Color.h"
#include "Math/Random.h"
#include "Math/MathUtils.h"

//framework
#include "Framework/EventSystem.h"
#include "Framework/Singleton.h"
#include "Framework/Factory.h"

//graphics
#include "Graphics/Renderer.h"
#include "Graphics/Texture.h"
#include "Graphics/ParticleSystem.h"
#include "Graphics/Font.h"

//systems
#include "Audio/AudioSystem.h"
#include "Audio/AudioChannel.h"
#include "Input/InputSystem.h"

//resource
#include "Resource/ResourceSystem.h"

//objects
#include "Object/Actor.h"
#include "Object/Scene.h"

//components
#include "Component/SpriteComponent.h"
#include "Component/SpriteAnimationComponent.h"
#include "Component/PhysicsComponent.h"

#include <vector>
#include <memory>
#include <algorithm>

namespace gme {
	using ObjectFactory = Singleton<Factory<std::string, Object>>;


	class Engine {
	public:
		void Startup();
		void Shutdown();

		void Update();
		void Draw(Renderer* renderer);

		template<typename T>
		T* Get();
	
	public:
		FrameTimer time;

	private:
		std::vector<std::unique_ptr<System>> systems;
	};

	template<typename T>
	inline T* Engine::Get()
	{
		for (auto& system : systems) {
			if(dynamic_cast<T*>(system.get())) return dynamic_cast<T*>(system.get());
		}
		return nullptr;
	}
}