#pragma once

#include <vector>
#include "Camera.h"
#include "Object3d.h"
#include "Text.h"
#include "glm.hpp"

namespace lwar
{
	struct Scene
	{
		std::vector<Object3d> objects;
		std::vector<Text> texts;

		void hideAllObjects()
		{
			for (int i = 0; i < objects.size(); ++i)
			{
				objects[i].visible = false;
			}
		}
	};
}