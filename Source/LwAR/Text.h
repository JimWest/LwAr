#pragma once

#include <vec3.hpp> // glm::vec3
#include <gtc/quaternion.hpp>

namespace lwar
{
	struct Text
	{
		const char* text;
		int x, y, size;

		Text::Text() {};
		Text::Text(const char * text, int x, int y, int size)
		{
			this->text = text;
			this->x = x;
			this->y = y;
			this->size = size;
		}
	};
}

