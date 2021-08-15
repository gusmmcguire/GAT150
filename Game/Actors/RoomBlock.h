#pragma once
#include "Object/Actor.h"


class RoomBlock : public gme::Actor {
public:
	RoomBlock(const gme::Transform& transform, std::shared_ptr<gme::Texture> texture) : gme::Actor{ transform,texture } {
		tag = "room";
	}
};