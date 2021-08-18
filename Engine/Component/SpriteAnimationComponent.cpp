#include "SpriteAnimationComponent.h"
#include "Engine.h"

namespace gme {
	void SpriteAnimationComponent::Update(){
		frameTime = 1 / fps;

		frameTimer += owner->scene->engine->time.deltaTime;
		if (frameTimer >= frameTime) {
			frameTimer = 0;
			frame++;

			if (frame >= numFramesX * numFramesY) frame = 0;
		}
		Vector2 size = texture->GetSize();
		Vector2 frameCount{ numFramesX,numFramesY };
		Vector2 frameSize = size / frameCount;
		
		rect.x = static_cast<int>((frame % numFramesX) * frameSize.x);
		rect.y = static_cast<int>((frame / numFramesY) * frameSize.y);
		rect.w = static_cast<int>(frameSize.x);
		rect.h = static_cast<int>(frameSize.y);
	}

	void SpriteAnimationComponent::Draw(Renderer* renderer){
		renderer->Draw(texture, rect, owner->transform);
	}
}