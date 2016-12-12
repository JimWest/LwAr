#pragma once
#include "Renderer.h"
#include <iostream>
#include <fstream>
#include <string>
#include <assert.h>
#include <functional>

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

namespace lwar
{

	class OpenGLRenderer :
		public Renderer
	{
	public:
		OpenGLRenderer();
		OpenGLRenderer(int windowWidth, int windowHeight, std::string windowTitle);
		~OpenGLRenderer();

		void initObject(Object3d& object) override;
		void preDraw() override;
		void drawText(const char * text, int x, int y, int size);
		void drawObject(Object3d& object, bool ignoreDepth = false) override;
		void postDraw() override;
		int getWindowWidth() override;
		int getWindowHeight() override;

		//std::function<void(lwar::Renderer* renderer, int key, int scancode, int action, int mods)> onKeyboardInput;
		bool quit = false;

	private:

		typedef unsigned int uint;
		std::string windowTitle;
		GLint windowWidth = 640;     // Define our window width
		GLint windowHeight = 480;     // Define our window height
		GLfloat aspectRatio;
		GLfloat fieldOfView = 45.0f;   // FoV
		GLfloat zNear = 0.1f;    // Near clip plane
		GLfloat zFar = 200.0f;  // Far clip plane
		GLFWwindow  *window;
		GLuint standardShaderID;
		GLuint unlitShaderID;

		unsigned int Text2DTextureID;
		unsigned int Text2DVertexBufferID;
		unsigned int Text2DUVBufferID;
		unsigned int Text2DShaderID;
		unsigned int Text2DUniformID;

		void initGL();
		void initText2D(const char * texturePath);
		GLuint loadShaders(const char * vertex_file_path, const char * fragment_file_path);
		static GLuint matToTexture(cv::Mat& mat, GLenum minFilter, GLenum magFilter, GLenum wrapFilter);
		static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void windowSizeCallback(GLFWwindow* window, int width, int height);
		static void framebufferSizeCallback(GLFWwindow* window, int width, int height);

	};
}

