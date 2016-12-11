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
		Object3d(const char * path);
		~Object3d();

		std::vector<glm::vec3> vertices;
		std::vector<glm::vec2> uvs;
		std::vector<glm::vec3> normals;
		bool visible = true;
		Transform transform;
		Material material;
		uint32_t vao, vbo, uvbo, nbo;

	private:
		bool loadOBJ(const char * path, std::vector<glm::vec3>& out_vertices, std::vector<glm::vec2>& out_uvs, std::vector<glm::vec3>& out_normals);


	};
}
