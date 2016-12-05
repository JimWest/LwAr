#pragma once
#include "Renderer.h"
#include <iostream>
#include <fstream>
#include <string>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#include <GL/glew.h>        
#include <GLFW\glfw3.h>


#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/quaternion.hpp>
#include <gtx/quaternion.hpp>
#include <gtx/euler_angles.hpp>
#include <gtx/norm.hpp>

#include "Object3d.h"

class OpenGLRenderer :
	public Renderer
{
public:
	OpenGLRenderer();
	OpenGLRenderer(int windowWidth, int windowHeight, std::string windowTitle);
	~OpenGLRenderer();

	void initObject(const Object3d* object) override;
	void preDraw() override;
	void drawObject(Object3d* object, cv::Mat &camFrame) override;
	void postDraw() override;

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
	GLuint standardShaderID;
	GLuint unlitShaderID;

	void initGL();
	GLuint loadShaders(const char * vertex_file_path, const char * fragment_file_path);
	static GLuint matToTexture(cv::Mat &mat, GLenum minFilter, GLenum magFilter, GLenum wrapFilter);
	static void keyCallback(GLFWwindow * window, int key, int scancode, int action, int mods);
	static void windowSizeCallback(GLFWwindow * window, int width, int height);
	static void framebufferSizeCallback(GLFWwindow * window, int width, int height);

};

