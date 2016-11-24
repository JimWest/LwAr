#pragma once
#include "Renderer.h"
#include <iostream>
#include <fstream>
#include <string>
#include "opencv2/opencv.hpp"
#include <GL/glew.h>        
#include <GLFW\glfw3.h>
#include <vec3.hpp> // glm::vec3
#include <vec4.hpp> // glm::vec4
#include <mat4x4.hpp> // glm::mat4
#include <gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective


class OpenGLRenderer :
	public Renderer
{
public:
	OpenGLRenderer();
	OpenGLRenderer(int windowWidth, int windowHeight, std::string windowTitle);
	~OpenGLRenderer();

	void PrepareTriangle();
	void DrawTriangle(cv::Mat &camFrame);
	void Draw(cv::Mat & camFrame);
	GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path);
	static GLuint matToTexture(cv::Mat &mat, GLenum minFilter, GLenum magFilter, GLenum wrapFilter);
	bool quit = false;

private:

	typedef unsigned int uint;
	std::string windowTitle;
	GLint   windowWidth = 640;     // Define our window width
	GLint   windowHeight = 480;     // Define our window height
	GLfloat fieldOfView = 45.0f;   // FoV
	GLfloat zNear = 0.1f;    // Near clip plane
	GLfloat zFar = 200.0f;  // Far clip plane
	int    frameCount = 0;
	double frameStartTime, frameEndTime, frameDrawTime;
	GLFWwindow  *window;
	GLuint programID;

	// This will identify our vertex buffer
	GLuint vertexbuffer;

	void InitGL();
	static void key_callback(GLFWwindow * window, int key, int scancode, int action, int mods);
};

