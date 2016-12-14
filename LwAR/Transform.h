#pragma once

#include <vec3.hpp> // glm::vec3
#include <gtc/quaternion.hpp>

namespace lwar
{
	struct Transform
	{
		glm::vec3 translation = glm::vec3(0.0f);
		glm::quat rotation;
		glm::vec3 scale = glm::vec3(1.0f);

		void rotate(float angle, glm::vec3 axis)
		{
			rotation = glm::rotate(rotation, angle, axis);
		}
	};
}
