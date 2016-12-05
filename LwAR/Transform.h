#pragma once

#include <vec3.hpp> // glm::vec3
#include <vec4.hpp> // glm::vec4
#include <mat4x4.hpp> // glm::mat4
#include <gtc/quaternion.hpp>
#include <gtx/euler_angles.hpp>

class Transform
{
public:
	Transform();
	~Transform();

	glm::vec3 translation = glm::vec3(0.0f);
	glm::quat rotation;
	glm::vec3 scale = glm::vec3(1.0f);
};
