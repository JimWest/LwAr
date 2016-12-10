#pragma once

#include <vector>
#include "Camera.h"
#include "Object3d.h"
#include "glm.hpp"

namespace lwar
{
	struct Scene
	{
		Camera camera;
		std::vector<Object3d> objects;
	};
}