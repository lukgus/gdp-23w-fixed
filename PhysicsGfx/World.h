#pragma once

#include "Engine.h"

namespace gdp
{
	class World
	{
	public:
		~World() { }

		virtual void Initialize() = 0;
		virtual void Destroy() = 0;

		virtual void Update(float dt) = 0;
		virtual void Render() { }

	protected:
		World() { }
	};
}