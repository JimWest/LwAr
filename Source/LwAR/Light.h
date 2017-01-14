#pragma once

#include "Transform.h"
#include "vec3.hpp"

namespace lwar
{
	struct Light
	{
		glm::vec3 color = glm::vec3(1,1,1);
		float strength = 50.0f;
		Transform transform;
	};
}

