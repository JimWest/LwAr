#pragma once
#include <vector>
#include "Transform.h"

class Object3d
{



public:
	enum Primitves
	{
		Triangle, Quad, Cube, Sphere, Cylinder
	};

	Object3d();
	Object3d(Primitves primitve);
	~Object3d();

	std::vector<float> vertices;
	Transform transform;
	uint32_t vao, vbo;
};
