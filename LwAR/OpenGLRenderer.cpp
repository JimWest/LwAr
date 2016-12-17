#include "OpenGLRenderer.h"

using namespace std;

namespace lwar
{
	OpenGLRenderer::OpenGLRenderer()
	{
		initGL();
	}

	OpenGLRenderer::OpenGLRenderer(int windowWidth, int windowHeight, std::string windowTitle, float fov, float zNear , float zFar, float worldCameraDist)
	{
		this->windowWidth = windowWidth;
		this->windowHeight = windowHeight;
		this->windowTitle = windowTitle;
		this->fov = fov;
		this->zNear = zNear;
		this->zFar = zFar;
		this->worldCameraDist = worldCameraDist;
		initGL();
	}


	OpenGLRenderer::~OpenGLRenderer()
	{
		// Delete buffers
		glDeleteBuffers(1, &Text2DVertexBufferID);
		glDeleteBuffers(1, &Text2DUVBufferID);

		// Delete texture
		glDeleteTextures(1, &Text2DTextureID);

		// Delete shader
		glDeleteProgram(Text2DShaderID);

		glfwTerminate();
	}


	void OpenGLRenderer::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		// callback for pressed keys, glfwGetWindowUserPointer needed cause glfw needs static functions for callbacks
		OpenGLRenderer *renderer = static_cast<OpenGLRenderer*>(glfwGetWindowUserPointer(window));
		if (action == GLFW_PRESS && renderer->onKeyboardInput)
			renderer->onKeyboardInput(key);
		else
			renderer->onKeyboardInput(-1);
	}

	void OpenGLRenderer::windowSizeCallback(GLFWwindow* window, int width, int height)
	{
	}

	void OpenGLRenderer::framebufferSizeCallback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	void OpenGLRenderer::initGL()
	{
		// Initialise glfw
		glfwInit();

		glfwWindowHint(GLFW_SAMPLES, 4);

		// Create a window
		glfwWindow = glfwCreateWindow(windowWidth, windowHeight, windowTitle.c_str(), NULL, NULL);
		glfwMakeContextCurrent(glfwWindow);

		if (!glfwWindow)
		{
			cout << "Failed to open window!" << endl;
			glfwTerminate();
			exit(-1);
		}

		// Specify the callback function for key presses/releases
		glfwSetWindowUserPointer(glfwWindow, this);
		glfwSetKeyCallback(glfwWindow, keyCallback);

		glfwSetWindowSizeCallback(glfwWindow, windowSizeCallback);
		glfwSetFramebufferSizeCallback(glfwWindow, framebufferSizeCallback);

		//  Initialise glew (must occur after window creation or glew will error)
		GLenum err = glewInit();
		if (GLEW_OK != err)
		{
			cout << "GLEW initialisation error: " << glewGetErrorString(err) << endl;
			exit(-1);
		}
		cout << "GLEW okay - using version: " << glewGetString(GLEW_VERSION) << endl;

		// Setup our viewport to be the entire size of the window
		glViewport(0, 0, (GLsizei)windowWidth, (GLsizei)windowHeight);

		// Change to the projection matrix and set our viewing volume
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		// The following code is a fancy bit of math that is equivilant to calling:
		// gluPerspective(fieldOfView/2.0f, width/height , near, far);
		// We do it this way simply to avoid requiring glu.h
		aspectRatio = (windowWidth > windowHeight) ? float(windowWidth) / float(windowHeight) : float(windowHeight) / float(windowWidth);
		GLfloat fH = tan(float(fov / 360.0f * 3.14159f)) * zNear;
		GLfloat fW = fH * aspectRatio;
		glFrustum(-fW, fW, -fH, fH, zNear, zFar);

		// ----- OpenGL settings -----
		glDepthFunc(GL_LEQUAL);
		glEnable(GL_DEPTH_TEST);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glEnable(GL_CULL_FACE);
		glfwSwapInterval(1);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set our clear colour to black

		standardShaderID = loadShaders("shaders/StandardShader.vert", "shaders/StandardShader.frag");
		unlitShaderID = loadShaders("shaders/UnlitShader.vert", "shaders/UnlitShader.frag");

		if ((err = glGetError()) != GL_NO_ERROR) {
			cerr << "OpenGL error: " << err << ", " << gluErrorString(err) << endl;
		}

		//initText2D("Holstein.png");
		initText2D("calibri.bmp");
		
	}

	void OpenGLRenderer::initText2D(const char * texturePath) {

		// Initialize texture
		cv::Mat font = cv::imread(texturePath);	
		// don't know why but the font don't needs to be flipped, so flip it here so its the wright way later
		//cv::Mat flippedFont;
		//cv::flip(font, flippedFont, 0);
		Text2DTextureID = matToTexture(font, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_REPEAT);

		// Initialize VBO
		glGenBuffers(1, &Text2DVertexBufferID);
		glGenBuffers(1, &Text2DUVBufferID);

		// Initialize Shader
		Text2DShaderID = loadShaders("shaders/TextVertexShader.vertexshader", "shaders/TextVertexShader.fragmentshader");

		// Initialize uniforms' IDs
		Text2DUniformID = glGetUniformLocation(Text2DShaderID, "myTextureSampler");
		//Text2DUniformID = glGetUniformLocation(unlitShaderID, "myTextureSampler");
	}

	void OpenGLRenderer::initObject(Object3d& object)
	{
		glGenVertexArrays(1, &(GLuint)object.vao);
		glBindVertexArray((GLuint)object.vao);

		glGenBuffers(1, &(GLuint)object.vbo);
		glBindBuffer(GL_ARRAY_BUFFER, object.vbo);
		glBufferData(GL_ARRAY_BUFFER, object.vertices.size() * sizeof(glm::vec3), &object.vertices[0], GL_STATIC_DRAW);

		glGenBuffers(1, &(GLuint)object.uvbo);
		glBindBuffer(GL_ARRAY_BUFFER, object.uvbo);
		glBufferData(GL_ARRAY_BUFFER, object.uvs.size() * sizeof(glm::vec2), &object.uvs[0], GL_STATIC_DRAW);

		if (object.normals.size() > 0)
		{
			glGenBuffers(1, &(GLuint)object.nbo);
			glBindBuffer(GL_ARRAY_BUFFER, object.nbo);
			glBufferData(GL_ARRAY_BUFFER, object.normals.size() * sizeof(glm::vec3), &object.normals[0], GL_STATIC_DRAW);
		}
	}


	void OpenGLRenderer::preDraw()
	{
		quit = glfwWindowShouldClose(glfwWindow);
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	}

	
	void OpenGLRenderer::drawText(const char * text, int x, int y, int size) {

		unsigned int length = strlen(text);

		// Fill buffers
		std::vector<glm::vec2> vertices;
		std::vector<glm::vec2> UVs;
		for (unsigned int i = 0; i < length; i++) {

			glm::vec2 vertex_up_left = glm::vec2(x + i*size, y + size);
			glm::vec2 vertex_up_right = glm::vec2(x + i*size + size, y + size);
			glm::vec2 vertex_down_right = glm::vec2(x + i*size + size, y);
			glm::vec2 vertex_down_left = glm::vec2(x + i*size, y);

			vertices.push_back(vertex_up_left);
			vertices.push_back(vertex_down_left);
			vertices.push_back(vertex_down_right);

			vertices.push_back(vertex_down_right);
			vertices.push_back(vertex_up_right);
			vertices.push_back(vertex_up_left);

			char character = text[i];
			float uv_x = (character % 16) / 16.0f;
			float uv_y = (character / 16) / 16.0f;

			glm::vec2 uv_up_left = glm::vec2(uv_x, uv_y);
			glm::vec2 uv_up_right = glm::vec2(uv_x + 1.0f / 16.0f, uv_y);
			glm::vec2 uv_down_right = glm::vec2(uv_x + 1.0f / 16.0f, (uv_y + 1.0f / 16.0f));
			glm::vec2 uv_down_left = glm::vec2(uv_x, (uv_y + 1.0f / 16.0f));
			UVs.push_back(uv_up_left);
			UVs.push_back(uv_down_left);
			UVs.push_back(uv_down_right);
			UVs.push_back(uv_down_right);
			UVs.push_back(uv_up_right);			
			UVs.push_back(uv_up_left);
		}
		glBindBuffer(GL_ARRAY_BUFFER, Text2DVertexBufferID);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), &vertices[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, Text2DUVBufferID);
		glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(glm::vec2), &UVs[0], GL_STATIC_DRAW);

		// Bind shader
		glUseProgram(Text2DShaderID);

		// Bind texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Text2DTextureID);
		// Set our "myTextureSampler" sampler to user Texture Unit 0
		glUniform1i(Text2DUniformID, 0);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, Text2DVertexBufferID);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

		// 2nd attribute buffer : UVs
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, Text2DUVBufferID);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glEnable(GL_BLEND);

		glBlendFunc(GL_ONE, GL_CONSTANT_ALPHA);
		glBlendEquation(GL_FUNC_ADD);
		glBlendColor(1.0, 0.0, 0.0, 1.0);

		// Draw call
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());

		glDisable(GL_BLEND);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
	}


	void OpenGLRenderer::drawObject(Object3d& object, bool ignoreDepth)
	{
		GLuint shaderId;

		switch (object.material.shaderType)
		{
		case ShaderType::Standard:
			shaderId = standardShaderID;
			break;
		case ShaderType::Unlit:
		default:
			shaderId = unlitShaderID;
			break;
		}

		// Use our shader
		glUseProgram(shaderId);

		// Camera matrix
		glm::mat4 projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, zNear, zFar);
		glm::mat4 viewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, worldCameraDist),
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f));

		// modell matrix
		glm::mat4 identyMatrix = glm::mat4(1.0f);
		glm::mat4 translationMatrix = glm::translate(identyMatrix, object.transform.translation);
		glm::mat4 scaleMatrix = glm::scale(identyMatrix, object.transform.scale);
		glm::mat4 rotationMatrix = glm::toMat4(object.transform.rotation);
		glm::mat4 modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;

		glm::mat4 mvp = projectionMatrix * viewMatrix * modelMatrix;

		// set the matrix paremters on the shader
		GLuint matrixID = glGetUniformLocation(shaderId, "MVP");
		GLuint viewMatrixID = glGetUniformLocation(shaderId, "V");
		GLuint modelMatrixID = glGetUniformLocation(shaderId, "M");
		glUniformMatrix4fv(matrixID, 1, GL_FALSE, &mvp[0][0]);
		glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &modelMatrix[0][0]);
		glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, &viewMatrix[0][0]);

		// check OpenGL error
		GLenum err;
		if ((err = glGetError()) != GL_NO_ERROR) {
			cerr << "OpenGL error: " << err << ", " << gluErrorString(err) << endl;
		}

		// set the light position in the shader
		GLuint lightID = glGetUniformLocation(shaderId, "LightPosition_worldspace");
		glm::vec3 lightPos = glm::vec3(4, 4, 4);
		glUniform3f(lightID, lightPos.x, lightPos.y, lightPos.z);

		if ((err = glGetError()) != GL_NO_ERROR) {
			cerr << "OpenGL error: " << err << ", " << gluErrorString(err) << endl;
		}

		// Convert image and depth data to OpenGL textures
		//GLuint textureID = matToTexture(camFrame, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_CLAMP);
		GLuint textureID = matToTexture(object.material.texture, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_REPEAT);
		// Draw the textures
		// Note: Window co-ordinates origin is top left, texture co-ordinate origin is bottom left.	

		GLint texLocation = glGetUniformLocation(shaderId, "myTextureSampler");
		glUniform1i(texLocation, 0);

		if ((err = glGetError()) != GL_NO_ERROR) {
			cerr << "OpenGL error: " << err << ", " << gluErrorString(err) << endl;
		}

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID);
		//glUniform1i(textureID, 0);

		if ((err = glGetError()) != GL_NO_ERROR) {
			cerr << "OpenGL error: " << err << ", " << gluErrorString(err) << endl;
		}


		if (ignoreDepth)
			glDisable(GL_DEPTH_TEST);

		glBindVertexArray((GLuint)object.vao);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, object.vbo);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		// 2nd attribute buffer : UVs
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, object.uvbo);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

		// 3rd attribute buffer : normals
		if (object.nbo > 0)
		{
			glEnableVertexAttribArray(2);
			glBindBuffer(GL_ARRAY_BUFFER, object.nbo);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		}

		// Draw the object
		glDrawArrays(GL_TRIANGLES, 0, (object.vertices.size()));

		if (ignoreDepth)
			glEnable(GL_DEPTH_TEST);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		// Free the texture memory
		glDeleteTextures(1, &textureID);
		glDisable(GL_TEXTURE_2D);
	}

	void OpenGLRenderer::postDraw()
	{
		glfwSwapBuffers(glfwWindow);
		glfwPollEvents();

		// check OpenGL error
		GLenum err;
		if ((err = glGetError()) != GL_NO_ERROR) {
			cerr << "OpenGL error: " << err << ", " << gluErrorString(err) << endl;
		}
	}

	int OpenGLRenderer::getWindowWidth()
	{
		return windowWidth;
	}

	int OpenGLRenderer::getWindowHeight()
	{
		return windowHeight;
	}

	GLuint OpenGLRenderer::loadShaders(const char * vertex_file_path, const char * fragment_file_path) {

		// Create the shaders
		GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);

		GLenum err;
		if ((err = glGetError()) != GL_NO_ERROR) {
			cerr << "OpenGL error: " << err << ", " << gluErrorString(err) << endl;
		}

		GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		if ((err = glGetError()) != GL_NO_ERROR) {
			cerr << "OpenGL error: " << err << ", " << gluErrorString(err) << endl;
		}


		// Read the Vertex Shader code from the file
		std::string VertexShaderCode;
		std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
		if (VertexShaderStream.is_open()) {
			std::string Line = "";
			while (getline(VertexShaderStream, Line))
				VertexShaderCode += "\n" + Line;
			VertexShaderStream.close();
		}
		else {
			printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
			getchar();
			return 0;
		}

		// Read the Fragment Shader code from the file
		std::string FragmentShaderCode;
		std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
		if (FragmentShaderStream.is_open()) {
			std::string Line = "";
			while (getline(FragmentShaderStream, Line))
				FragmentShaderCode += "\n" + Line;
			FragmentShaderStream.close();
		}

		GLint Result = GL_FALSE;
		int InfoLogLength;


		// Compile Vertex Shader
		printf("Compiling shader : %s\n", vertex_file_path);
		char const * VertexSourcePointer = VertexShaderCode.c_str();
		glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
		glCompileShader(VertexShaderID);

		// Check Vertex Shader
		glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
		glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if (InfoLogLength > 0) {
			std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
			glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
			printf("%s\n", &VertexShaderErrorMessage[0]);
		}

		// Compile Fragment Shader
		printf("Compiling shader : %s\n", fragment_file_path);
		char const * FragmentSourcePointer = FragmentShaderCode.c_str();
		glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
		glCompileShader(FragmentShaderID);

		// Check Fragment Shader
		glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
		glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if (InfoLogLength > 0) {
			std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
			glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
			printf("%s\n", &FragmentShaderErrorMessage[0]);
		}


		// Link the program
		printf("Linking program\n");
		GLuint ProgramID = glCreateProgram();
		glAttachShader(ProgramID, VertexShaderID);
		glAttachShader(ProgramID, FragmentShaderID);
		glLinkProgram(ProgramID);

		// Check the program
		glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
		glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if (InfoLogLength > 0) {
			std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
			glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
			printf("%s\n", &ProgramErrorMessage[0]);
		}

		glDetachShader(ProgramID, VertexShaderID);
		glDetachShader(ProgramID, FragmentShaderID);

		glDeleteShader(VertexShaderID);
		glDeleteShader(FragmentShaderID);

		return ProgramID;
	}

	// returns the ID of the generated OpenGL Texture
	GLuint OpenGLRenderer::matToTexture(cv::Mat& mat, GLenum minFilter, GLenum magFilter, GLenum wrapFilter)
	{
		// OpenCV stores image from top to buttom, OpenGL the oppsite so flip it
		cv::Mat flippedMat;
		cv::flip(mat, flippedMat, 0);

		// Generate a number for our textureID's unique handle
		GLuint textureID;
		glGenTextures(1, &textureID);

		// Bind to our texture handle
		glBindTexture(GL_TEXTURE_2D, textureID);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		//// Catch silly-mistake texture interpolation method for magnification
		//if (magFilter == GL_LINEAR_MIPMAP_LINEAR ||
		//	magFilter == GL_LINEAR_MIPMAP_NEAREST ||
		//	magFilter == GL_NEAREST_MIPMAP_LINEAR ||
		//	magFilter == GL_NEAREST_MIPMAP_NEAREST)
		//{
		//	std::cout << "You can't use MIPMAPs for magnification - setting filter to GL_LINEAR" << std::endl;
		//	magFilter = GL_LINEAR;
		//}

		//// Set texture interpolation methods for minification and magnification
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

		//// Set texture clamping method
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapFilter);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapFilter);

		// Set incoming texture format to:
		// GL_BGR       for CV_CAP_OPENNI_BGR_IMAGE,
		// GL_LUMINANCE for CV_CAP_OPENNI_DISPARITY_MAP,
		// Work out other mappings as required ( there's a list in comments in main() )
		//GLenum inputColourFormat = GL_BGR;
		//if (mat.channels() == 1)
		//{
		//	inputColourFormat = GL_LUMINANCE;
		//}

		//// Create the texture
		//glTexImage2D(GL_TEXTURE_2D,     // Type of texture
		//	0,                 // Pyramid level (for mip-mapping) - 0 is the top level
		//	GL_RGB,            // Internal colour format to convert to
		//	mat.cols,          // Image width  i.e. 640 for Kinect in standard mode
		//	mat.rows,          // Image height i.e. 480 for Kinect in standard mode
		//	0,                 // Border width in pixels (can either be 1 or 0)
		//	inputColourFormat, // Input image format (i.e. GL_RGB, GL_RGBA, GL_BGR etc.)
		//	GL_UNSIGNED_BYTE,  // Image data type
		//	flippedMat.ptr());        // The actual image data itself

							   // If we're using mipmaps then generate them. Note: This requires OpenGL 3.0 or higher
		//if (minFilter == GL_LINEAR_MIPMAP_LINEAR ||
		//	minFilter == GL_LINEAR_MIPMAP_NEAREST ||
		//	minFilter == GL_NEAREST_MIPMAP_LINEAR ||
		//	minFilter == GL_NEAREST_MIPMAP_NEAREST)
		//{
		//	glGenerateMipmap(GL_TEXTURE_2D);
		//}

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mat.cols, mat.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, flippedMat.ptr());

		// ... nice trilinear filtering.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);

		return textureID;
	}
}

