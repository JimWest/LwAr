#include "Object3d.h"



Object3d::Object3d()
{
	transform = Transform();
}

Object3d::Object3d(Primitves primitve)
{

	switch (primitve)
	{
	case Triangle:
		vertices = std::vector<float>(20);
		vertices = {
			//  X     Y     Z       U     V
			-1.0f, -1.0f, 0.0f,   0.0f, 0.0f,
			1.0f, -1.0f, 0.0f,   1.0f, 0.0f,
			0.0f, 1.0, 0.0f,   1.0f, 1.0f,
		};
		break;
	case Quad:
		vertices = std::vector<float>(20);
		vertices = {
			//  X     Y     Z       U     V
			-1.0f, -1.0f, 0.0f,   0.0f, 0.0f,
			1.0f, -1.0f, 0.0f,   1.0f, 0.0f,
			1.0f, 1.0, 0.0f,   1.0f, 1.0f,
			-1.0f, 1.0f, 0.0f,   0.0f, 1.0f,
		};
		break;
	}

	/*case Quad:
		vertices = std::vector<Vertex>(4);
		Vertex vertex;
		vertex.Position = glm::vec3(-1.0f, -1.0f, 0.0f);
		vertex.Normal = glm::vec3(0.0f, 1.0f, 0.0f);
		vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		vertices.push_back(vertex);

		vertex.Position = glm::vec3(1.0f, -1.0f, 0.0f);
		vertex.Normal = glm::vec3(0.0f, 1.0f, 0.0f);
		vertex.TexCoords = glm::vec2(1.0f, 0.0f);
		vertices.push_back(vertex);

		vertex.Position = glm::vec3(1.0f, 1.0f, 0.0f);
		vertex.Normal = glm::vec3(0.0f, 1.0f, 0.0f);
		vertex.TexCoords = glm::vec2(1.0f, 1.0f);
		vertices.push_back(vertex);

		vertex.Position = glm::vec3(-1.0f, 1.0f, 0.0f);
		vertex.Normal = glm::vec3(0.0f, 1.0f, 0.0f);
		vertex.TexCoords = glm::vec2(0.0f, 1.0f);
		vertices.push_back(vertex);*/
}


Object3d::~Object3d()
{
	;
}
