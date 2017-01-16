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
#include "Text.h"
#include "OpenGLShader.h"

namespace lwar
{

	class OpenGLRenderer :
		public Renderer
	{
	public:
		OpenGLRenderer();
		OpenGLRenderer(int windowWidth, int windowHeight, std::string windowTitle, float fov = 45.0f, float zNear = 0.1f, float zFar = 200.0f, float worldCameraDist = 3.0f);
		~OpenGLRenderer();
		
		void initObject(Object3d& object) override;
		void preDraw() override;
		void drawText(Text& text);
		void drawObject(Object3d& object, glm::mat4& projectionMatrix, glm::mat4& viewMatrix, bool ignoreDepth) override;
		void postDraw() override;
		int getWindowWidth() override;
		int getWindowHeight() override;
		bool getShouldClose();

	private:

		typedef unsigned int uint;
		std::string windowTitle;
		GLint windowWidth = 640;     // Define our window width
		GLint windowHeight = 480;     // Define our window height
		GLfloat aspectRatio;
		GLfloat fov;   
		GLfloat zNear;  
		GLfloat zFar;
		float worldCameraDist;
		GLFWwindow  *glfwWindow;
		GLuint standardShaderID;
		GLuint unlitShaderID;

		unsigned int Text2DTextureID;
		unsigned int Text2DVertexBufferID;
		unsigned int Text2DUVBufferID;
		unsigned int Text2DShaderID;
		unsigned int Text2DUniformID;
		bool quit = false;

		// Initialises all the OpenGL functions. The needs to be called before any rendering can be done.
		void initGL();
		// inits the given font as Text
		void initText2D(const char * texturePath);
		// Loads the shaders out of the OpenGLShader file as a string into an OpenGL shader which will be later used for rendering.
		GLuint loadShaders(const char * vertex_file_path, const char * fragment_file_path);
		// returns the ID of the generated OpenGL Texture
		static GLuint matToTexture(cv::Mat& mat, GLenum minFilter, GLenum magFilter, GLenum wrapFilter);
		static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void windowSizeCallback(GLFWwindow* window, int width, int height);
		static void framebufferSizeCallback(GLFWwindow* window, int width, int height);

	};
}

