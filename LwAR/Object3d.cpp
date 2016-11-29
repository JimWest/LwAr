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
		vertices = std::vector<float>(9);
		vertices = {
			//  X     Y     Z    
			-1.0f, -1.0f, 0.0f, 
			1.0f, -1.0f, 0.0f,  
			0.0f, 1.0, 0.0f, 
		};

		uvs = std::vector<float>(6);
		uvs = {
			//U     V
			0.0f, 0.0f,
			1.0f, 0.0f,
			0.5f, 1.0f,
		};

		break;
	case Quad:
		vertices = std::vector<float>(12);
		vertices = {
			//  X     Y     Z      
			-1.0f, -1.0f, 0.0f, 
			1.0f, -1.0f, 0.0f,  
			1.0f, 1.0, 0.0f,   
			-1.0f, 1.0f, 0.0f,  
		};

		uvs = std::vector<float>(8);
		uvs = {
			//U     V
			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,
			0.0f, 1.0f,
		};
		break;
	case Cube:
		vertices = std::vector<float>(60);
		vertices = {
			//  X     Y     Z       U			 V
			-1.0f,-1.0f,-1.0f,		0.000059f, 1.0f - 0.000004f,
			-1.0f,-1.0f, 1.0f,	    0.000103f, 1.0f - 0.336048f,
			-1.0f, 1.0f, 1.0f,	    0.335973f, 1.0f - 0.335903f,
			1.0f, 1.0f,-1.0f,	    1.000023f, 1.0f - 0.000013f,
			-1.0f,-1.0f,-1.0f,	    0.667979f, 1.0f - 0.335851f,
			-1.0f, 1.0f,-1.0f,	    0.999958f, 1.0f - 0.336064f,
			1.0f,-1.0f, 1.0f,	    0.667979f, 1.0f - 0.335851f,
			-1.0f,-1.0f,-1.0f,	    0.336024f, 1.0f - 0.671877f,
			1.0f,-1.0f,-1.0f,	    0.667969f, 1.0f - 0.671889f,
			1.0f, 1.0f,-1.0f,	    1.000023f, 1.0f - 0.000013f,
			1.0f,-1.0f,-1.0f,	    0.668104f, 1.0f - 0.000013f,
			-1.0f,-1.0f,-1.0f,	    0.667979f, 1.0f - 0.335851f,
			-1.0f,-1.0f,-1.0f,	    0.000059f, 1.0f - 0.000004f,
			-1.0f, 1.0f, 1.0f,	    0.335973f, 1.0f - 0.335903f,
			-1.0f, 1.0f,-1.0f,	    0.336098f, 1.0f - 0.000071f,
			1.0f,-1.0f, 1.0f,	    0.667979f, 1.0f - 0.335851f,
			-1.0f,-1.0f, 1.0f,	    0.335973f, 1.0f - 0.335903f,
			-1.0f,-1.0f,-1.0f,	    0.336024f, 1.0f - 0.671877f,
			-1.0f, 1.0f, 1.0f,	    1.000004f, 1.0f - 0.671847f,
			-1.0f,-1.0f, 1.0f,	    0.999958f, 1.0f - 0.336064f,
			1.0f,-1.0f, 1.0f,	    0.667979f, 1.0f - 0.335851f,
			1.0f, 1.0f, 1.0f,	    0.668104f, 1.0f - 0.000013f,
			1.0f,-1.0f,-1.0f,	    0.335973f, 1.0f - 0.335903f,
			1.0f, 1.0f,-1.0f,	    0.667979f, 1.0f - 0.335851f,
			1.0f,-1.0f,-1.0f,	    0.335973f, 1.0f - 0.335903f,
			1.0f, 1.0f, 1.0f,	    0.668104f, 1.0f - 0.000013f,
			1.0f,-1.0f, 1.0f,	    0.336098f, 1.0f - 0.000071f,
			1.0f, 1.0f, 1.0f,	    0.000103f, 1.0f - 0.336048f,
			1.0f, 1.0f,-1.0f,	    0.000004f, 1.0f - 0.671870f,
			-1.0f, 1.0f,-1.0f,	    0.336024f, 1.0f - 0.671877f,
			1.0f, 1.0f, 1.0f,	    0.000103f, 1.0f - 0.336048f,
			-1.0f, 1.0f,-1.0f,	    0.336024f, 1.0f - 0.671877f,
			-1.0f, 1.0f, 1.0f,	    0.335973f, 1.0f - 0.335903f,
			1.0f, 1.0f, 1.0f,	    0.667969f, 1.0f - 0.671889f,
			-1.0f, 1.0f, 1.0f,	    1.000004f, 1.0f - 0.671847f,
			1.0f,-1.0f, 1.0f,	    0.667979f, 1.0f - 0.335851f,
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
