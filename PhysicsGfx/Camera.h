#pragma once

#include <glm/glm.hpp>

namespace gdp
{
	struct Camera {
		glm::vec3 position;
		glm::vec3 target;
		glm::vec3 direction;
		bool targetting;
	};
}