#pragma once
#include "Renderer.h"
#include "opencv2/opencv.hpp"
#include <GL/glew.h>        
#include <GLFW\glfw3.h>
#include <iostream>

class OpenGLRenderer :
	public Renderer
{
public:
	OpenGLRenderer();
	~OpenGLRenderer();

	void Draw(cv::Mat & camFrame);		
	static GLuint matToTexture(cv::Mat &mat, GLenum minFilter, GLenum magFilter, GLenum wrapFilter);

	bool quit = false;

private:

	typedef unsigned int uint;

	GLint   windowWidth = 640;     // Define our window width
	GLint   windowHeight = 480;     // Define our window height
	GLfloat fieldOfView = 45.0f;   // FoV
	GLfloat zNear = 0.1f;    // Near clip plane
	GLfloat zFar = 200.0f;  // Far clip plane

	int    frameCount = 0;
	double frameStartTime, frameEndTime, frameDrawTime;

	GLFWwindow  *window;

	void InitGL();
	static void key_callback(GLFWwindow * window, int key, int scancode, int action, int mods);
};

