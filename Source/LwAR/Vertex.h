#pragma once
#include <vec2.hpp>
#include <vec3.hpp>

namespace lwar
{
	struct Vertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
	};
}