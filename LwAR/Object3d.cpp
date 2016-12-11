#include "Object3d.h"

namespace lwar
{
	Object3d::Object3d(Primitves primitve)
	{
		transform = Transform();
		material = Material();

		switch (primitve)
		{
		case Triangle:
			//  X     Y     Z    
			vertices.push_back(glm::vec3(-1.0f, -1.0f, 0.0f));
			vertices.push_back(glm::vec3(1.0f, -1.0f, 0.0f));
			vertices.push_back(glm::vec3(0.0f, 1.0, 0.0f));

			//U     V
			uvs.push_back(glm::vec2(0.0f, 0.0f));
			uvs.push_back(glm::vec2(1.0f, 0.0f));
			uvs.push_back(glm::vec2(0.5f, 1.0f));

			break;

		case Quad:
			//  X     Y     Z      			
			vertices.push_back(glm::vec3(-1.0f, 1.0f, 0.0f));
			vertices.push_back(glm::vec3(-1.0f, -1.0f, 0.0f));
			vertices.push_back(glm::vec3(1.0f, -1.0f, 0.0f));
			vertices.push_back(glm::vec3(1.0f, -1.0f, 0.0f));
			vertices.push_back(glm::vec3(1.0f, 1.0f, 0.0f));
			vertices.push_back(glm::vec3(-1.0f, 1.0f, 0.0f));

			//U     V
			uvs.push_back(glm::vec2(0.0f, 1.0f));
			uvs.push_back(glm::vec2(0.0f, 0.0f));
			uvs.push_back(glm::vec2(1.0f, 0.0f));
			uvs.push_back(glm::vec2(1.0f, 0.0f));
			uvs.push_back(glm::vec2(1.0f, 1.0f));
			uvs.push_back(glm::vec2(0.0f, 1.0f));

			break;

		case Cube:

			//  X     Y     Z      
			vertices.push_back(glm::vec3(-1.0f, -1.0f, -1.0f));
			vertices.push_back(glm::vec3(-1.0f, -1.0f, 1.0f));
			vertices.push_back(glm::vec3(-1.0f, 1.0f, 1.0f));
			vertices.push_back(glm::vec3(1.0f, 1.0f, -1.0f));
			vertices.push_back(glm::vec3(-1.0f, -1.0f, -1.0f));
			vertices.push_back(glm::vec3(-1.0f, 1.0f, -1.0f));
			vertices.push_back(glm::vec3(1.0f, -1.0f, 1.0f));
			vertices.push_back(glm::vec3(-1.0f, -1.0f, -1.0f));
			vertices.push_back(glm::vec3(1.0f, -1.0f, -1.0f));
			vertices.push_back(glm::vec3(1.0f, 1.0f, -1.0f));
			vertices.push_back(glm::vec3(1.0f, -1.0f, -1.0f));
			vertices.push_back(glm::vec3(-1.0f, -1.0f, -1.0f));
			vertices.push_back(glm::vec3(-1.0f, -1.0f, -1.0f));
			vertices.push_back(glm::vec3(-1.0f, 1.0f, 1.0f));
			vertices.push_back(glm::vec3(-1.0f, 1.0f, -1.0f));
			vertices.push_back(glm::vec3(1.0f, -1.0f, 1.0f));
			vertices.push_back(glm::vec3(-1.0f, -1.0f, 1.0f));
			vertices.push_back(glm::vec3(-1.0f, -1.0f, -1.0f));
			vertices.push_back(glm::vec3(-1.0f, 1.0f, 1.0f));
			vertices.push_back(glm::vec3(-1.0f, -1.0f, 1.0f));
			vertices.push_back(glm::vec3(1.0f, -1.0f, 1.0f));
			vertices.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
			vertices.push_back(glm::vec3(1.0f, -1.0f, -1.0f));
			vertices.push_back(glm::vec3(1.0f, 1.0f, -1.0f));
			vertices.push_back(glm::vec3(1.0f, -1.0f, -1.0f));
			vertices.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
			vertices.push_back(glm::vec3(1.0f, -1.0f, 1.0f));
			vertices.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
			vertices.push_back(glm::vec3(1.0f, 1.0f, -1.0f));
			vertices.push_back(glm::vec3(-1.0f, 1.0f, -1.0f));
			vertices.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
			vertices.push_back(glm::vec3(-1.0f, 1.0f, -1.0f));
			vertices.push_back(glm::vec3(-1.0f, 1.0f, 1.0f));
			vertices.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
			vertices.push_back(glm::vec3(-1.0f, 1.0f, 1.0f));
			vertices.push_back(glm::vec3(1.0f, -1.0f, 1.0f));

			//U			V
			uvs.push_back(glm::vec2(0.000059f, 1.0f - 0.000004f));
			uvs.push_back(glm::vec2(0.000103f, 1.0f - 0.336048f));
			uvs.push_back(glm::vec2(0.335973f, 1.0f - 0.335903f));
			uvs.push_back(glm::vec2(1.000023f, 1.0f - 0.000013f));
			uvs.push_back(glm::vec2(0.667979f, 1.0f - 0.335851f));
			uvs.push_back(glm::vec2(0.999958f, 1.0f - 0.336064f));
			uvs.push_back(glm::vec2(0.667979f, 1.0f - 0.335851f));
			uvs.push_back(glm::vec2(0.336024f, 1.0f - 0.671877f));
			uvs.push_back(glm::vec2(0.667969f, 1.0f - 0.671889f));
			uvs.push_back(glm::vec2(1.000023f, 1.0f - 0.000013f));
			uvs.push_back(glm::vec2(0.668104f, 1.0f - 0.000013f));
			uvs.push_back(glm::vec2(0.667979f, 1.0f - 0.335851f));
			uvs.push_back(glm::vec2(0.000059f, 1.0f - 0.000004f));
			uvs.push_back(glm::vec2(0.335973f, 1.0f - 0.335903f));
			uvs.push_back(glm::vec2(0.336098f, 1.0f - 0.000071f));
			uvs.push_back(glm::vec2(0.667979f, 1.0f - 0.335851f));
			uvs.push_back(glm::vec2(0.335973f, 1.0f - 0.335903f));
			uvs.push_back(glm::vec2(0.336024f, 1.0f - 0.671877f));
			uvs.push_back(glm::vec2(1.000004f, 1.0f - 0.671847f));
			uvs.push_back(glm::vec2(0.999958f, 1.0f - 0.336064f));
			uvs.push_back(glm::vec2(0.667979f, 1.0f - 0.335851f));
			uvs.push_back(glm::vec2(0.668104f, 1.0f - 0.000013f));
			uvs.push_back(glm::vec2(0.335973f, 1.0f - 0.335903f));
			uvs.push_back(glm::vec2(0.667979f, 1.0f - 0.335851f));
			uvs.push_back(glm::vec2(0.335973f, 1.0f - 0.335903f));
			uvs.push_back(glm::vec2(0.668104f, 1.0f - 0.000013f));
			uvs.push_back(glm::vec2(0.336098f, 1.0f - 0.000071f));
			uvs.push_back(glm::vec2(0.000103f, 1.0f - 0.336048f));
			uvs.push_back(glm::vec2(0.000004f, 1.0f - 0.671870f));
			uvs.push_back(glm::vec2(0.336024f, 1.0f - 0.671877f));
			uvs.push_back(glm::vec2(0.000103f, 1.0f - 0.336048f));
			uvs.push_back(glm::vec2(0.336024f, 1.0f - 0.671877f));
			uvs.push_back(glm::vec2(0.335973f, 1.0f - 0.335903f));
			uvs.push_back(glm::vec2(0.667969f, 1.0f - 0.671889f));
			uvs.push_back(glm::vec2(1.000004f, 1.0f - 0.671847f));
			uvs.push_back(glm::vec2(0.667979f, 1.0f - 0.335851f));

			break;
		}
	}

	Object3d::Object3d(const char * path)
	{
		loadOBJ(path, vertices, uvs, normals);
	}


	Object3d::~Object3d()
	{
	}


	// code taken from opengl-tutorial.org:
	//http://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/

	bool Object3d::loadOBJ(const char * path, std::vector<glm::vec3> & out_vertices, std::vector<glm::vec2> & out_uvs,
		std::vector<glm::vec3> & out_normals)
	{
		printf("Loading OBJ file %s...\n", path);

		std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
		std::vector<glm::vec3> temp_vertices;
		std::vector<glm::vec2> temp_uvs;
		std::vector<glm::vec3> temp_normals;


		FILE * file = fopen(path, "r");
		if (file == NULL) {
			printf("Impossible to open the file ! Are you in the right path ? See Tutorial 1 for details\n");
			getchar();
			return false;
		}

		while (1) {

			char lineHeader[128];
			// read the first word of the line
			int res = fscanf(file, "%s", lineHeader);
			if (res == EOF)
				break; // EOF = End Of File. Quit the loop.

					   // else : parse lineHeader

			if (strcmp(lineHeader, "v") == 0) {
				glm::vec3 vertex;
				fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
				temp_vertices.push_back(vertex);
			}
			else if (strcmp(lineHeader, "vt") == 0) {
				glm::vec2 uv;
				fscanf(file, "%f %f\n", &uv.x, &uv.y);
				uv.y = -uv.y; // Invert V coordinate since we will only use DDS texture, which are inverted. Remove if you want to use TGA or BMP loaders.
				temp_uvs.push_back(uv);
			}
			else if (strcmp(lineHeader, "vn") == 0) {
				glm::vec3 normal;
				fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
				temp_normals.push_back(normal);
			}
			else if (strcmp(lineHeader, "f") == 0) {
				std::string vertex1, vertex2, vertex3;
				unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
				int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
				if (matches != 9) {
					printf("File can't be read by our simple parser :-( Try exporting with other options\n");
					fclose(file);
					return false;
				}
				vertexIndices.push_back(vertexIndex[0]);
				vertexIndices.push_back(vertexIndex[1]);
				vertexIndices.push_back(vertexIndex[2]);
				uvIndices.push_back(uvIndex[0]);
				uvIndices.push_back(uvIndex[1]);
				uvIndices.push_back(uvIndex[2]);
				normalIndices.push_back(normalIndex[0]);
				normalIndices.push_back(normalIndex[1]);
				normalIndices.push_back(normalIndex[2]);
			}
			else {
				// Probably a comment, eat up the rest of the line
				char stupidBuffer[1000];
				fgets(stupidBuffer, 1000, file);
			}

		}

		// For each vertex of each triangle
		for (unsigned int i = 0; i < vertexIndices.size(); i++) {

			// Get the indices of its attributes
			unsigned int vertexIndex = vertexIndices[i];
			unsigned int uvIndex = uvIndices[i];
			unsigned int normalIndex = normalIndices[i];

			// Get the attributes thanks to the index
			glm::vec3 vertex = temp_vertices[vertexIndex - 1];
			glm::vec2 uv = temp_uvs[uvIndex - 1];
			glm::vec3 normal = temp_normals[normalIndex - 1];

			// Put the attributes in buffers
			out_vertices.push_back(vertex);
			out_uvs.push_back(uv);
			out_normals.push_back(normal);

		}
		fclose(file);
		return true;
	}


}