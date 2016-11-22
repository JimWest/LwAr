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

	GLuint matToTexture(cv::Mat &mat, GLenum minFilter, GLenum magFilter, GLenum wrapFilter);
};

