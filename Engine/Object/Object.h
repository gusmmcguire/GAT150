#pragma once

namespace gme {
	class Object {
	public:
		virtual ~Object(){}

		virtual void Create() {}
	};
}