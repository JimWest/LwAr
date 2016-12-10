#pragma once
#include <vector>
#include "Transform.h"
#include "Material.h"

namespace lwar
{
	enum Primitves
	{
		Triangle, Quad, Cube, Sphere, Cylinder
	};


	class Object3d
	{
	public:


		Object3d() : Object3d(Quad) {}
		Object3d(Primitves primitve);
		~Object3d();

		std::vector<float> vertices;
		std::vector<float> uvs;
		bool visible = true;
		Transform transform;
		Material material;
		uint32_t vao, vbo, uvbo;
	};
}
