#include "OpenGLRenderer.h"

using namespace std;

namespace lwar
{
	OpenGLRenderer::OpenGLRenderer()
	{
		initGL();
	}

	OpenGLRenderer::OpenGLRenderer(int windowWidth, int windowHeight, std::string windowTitle, float fov, float zNear, float zFar, float worldCameraDist)
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

	// GLFW Callback for pressed keys, saves the key 
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

	// GLFW Callback when the windows was resized, we need to resize the Buffer. It will be just scaled so it fits the screen.
	void OpenGLRenderer::framebufferSizeCallback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	void OpenGLRenderer::initGL()
	{
		// Initialise glfw
		glfwInit();

		// Set minimum needed OpenGL Version
		glfwWindowHint(GLFW_SAMPLES, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

		// Creates a GLFW window
		glfwWindow = glfwCreateWindow(windowWidth, windowHeight, windowTitle.c_str(), NULL, NULL);
		glfwMakeContextCurrent(glfwWindow);

		if (!glfwWindow)
		{
			cout << "Failed to open window!" << endl;
			glfwTerminate();
			getchar();
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

		standardShaderID = loadShaders(standardShaderVert, standardShaderFrag);

		if ((err = glGetError()) != GL_NO_ERROR) {
			cerr << "OpenGL error: " << err << ", " << gluErrorString(err) << endl;
		}

		initText2D("calibri.bmp");

	}
		
	void OpenGLRenderer::initText2D(const char * texturePath) {

		// Initialize texture
		cv::Mat font = cv::imread(texturePath);

		Text2DTextureID = matToTexture(font, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_REPEAT);

		// Initialize VBO
		glGenBuffers(1, &Text2DVertexBufferID);
		glGenBuffers(1, &Text2DUVBufferID);

		// Initialize Shader
		Text2DShaderID = loadShaders(textShaderVert, textShaderFrag);

		// Initialize uniforms' IDs
		Text2DUniformID = glGetUniformLocation(Text2DShaderID, "myTextureSampler");
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

	void OpenGLRenderer::drawText(Text& text)
	{
		unsigned int length = strlen(text.text);

		// Fill buffers
		std::vector<glm::vec2> vertices;
		std::vector<glm::vec2> UVs;
		for (unsigned int i = 0; i < length; i++) {

			glm::vec2 vertex_up_left = glm::vec2(text.x + i * text.size, text.y + text.size);
			glm::vec2 vertex_up_right = glm::vec2(text.x + i * text.size + text.size, text.y + text.size);
			glm::vec2 vertex_down_right = glm::vec2(text.x + i * text.size + text.size, text.y);
			glm::vec2 vertex_down_left = glm::vec2(text.x + i * text.size, text.y);

			vertices.push_back(vertex_up_left);
			vertices.push_back(vertex_down_left);
			vertices.push_back(vertex_down_right);

			vertices.push_back(vertex_down_right);
			vertices.push_back(vertex_up_right);
			vertices.push_back(vertex_up_left);

			char character = text.text[i];
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

		glDisable(GL_DEPTH_TEST);

		// Draw call
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());

		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
	}

	void OpenGLRenderer::drawObject(Object3d& object, glm::mat4& projectionMatrix, glm::mat4& viewMatrix, bool ignoreDepth)
	{
		// Use our shader
		glUseProgram(standardShaderID);

		// modell matrix
		glm::mat4 identyMatrix = glm::mat4(1.0f);
		glm::mat4 translationMatrix = glm::translate(identyMatrix, object.transform.translation);
		glm::mat4 scaleMatrix = glm::scale(identyMatrix, object.transform.scale);
		glm::mat4 rotationMatrix = glm::toMat4(object.transform.rotation);
		glm::mat4 modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;

		glm::mat4 mvp = projectionMatrix * viewMatrix * modelMatrix;

		// set the matrix paremters on the shader
		GLuint matrixID = glGetUniformLocation(standardShaderID, "MVP");
		GLuint viewMatrixID = glGetUniformLocation(standardShaderID, "V");
		GLuint modelMatrixID = glGetUniformLocation(standardShaderID, "M");
		glUniformMatrix4fv(matrixID, 1, GL_FALSE, &mvp[0][0]);
		glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &modelMatrix[0][0]);
		glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, &viewMatrix[0][0]);

		// check OpenGL error
		GLenum err;
		if ((err = glGetError()) != GL_NO_ERROR) {
			cerr << "OpenGL error: " << err << ", " << gluErrorString(err) << endl;
		}

		// set if the object is unlit or not
		GLuint unlitId = glGetUniformLocation(standardShaderID, "unlit");
		glUniform1i(unlitId, int(object.material.shaderType == ShaderType::Unlit));

		// set the light position in the shader
		GLuint lightID = glGetUniformLocation(standardShaderID, "LightPosition_worldspace");
		//glm::vec3 lightPos = glm::vec3(4, 4, 4);
		glUniform3f(lightID, light.transform.translation.x, light.transform.translation.y, light.transform.translation.z);

		// set the light position in the shader
		GLuint lightStrength = glGetUniformLocation(standardShaderID, "lightStrength");
		//glm::vec3 lightPos = glm::vec3(4, 4, 4);
		glUniform1f(lightStrength, light.strength);

		// set the light position in the shader
		GLuint lightColor = glGetUniformLocation(standardShaderID, "lightColor");
		//glm::vec3 lightPos = glm::vec3(4, 4, 4);
		glUniform3f(lightColor, light.color.x, light.color.y, light.color.z);

		// Convert image and depth data to OpenGL textures
		//GLuint textureID = matToTexture(camFrame, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_CLAMP);
		GLuint textureID = matToTexture(object.material.texture, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_REPEAT);
		// Draw the textures
		// Note: Window co-ordinates origin is top left, texture co-ordinate origin is bottom left.	

		GLint texLocation = glGetUniformLocation(standardShaderID, "myTextureSampler");
		glUniform1i(texLocation, 0);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID);
		//glUniform1i(textureID, 0);

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

		GLenum drawMode;
		switch (object.material.drawMode)
		{
		case DrawMode::Lines:
			drawMode = GL_LINES;
			break;
		case DrawMode::Points:
			drawMode = GL_POINTS;
			break;
		case DrawMode::Quads:
			drawMode = GL_QUADS;
			break;
		default:
			drawMode = GL_TRIANGLES;
			break;
		}

		// Draw the object
		glDrawArrays(drawMode, 0, (object.vertices.size()));

		if (ignoreDepth)
			glEnable(GL_DEPTH_TEST);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		// Frees the texture memory
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

	bool OpenGLRenderer::getShouldClose()
	{
		return quit;
	}

	GLuint OpenGLRenderer::loadShaders(const char * vertexShader, const char * fragmentShader) {

		// Create the shaders
		GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		
		GLint Result = GL_FALSE;
		int InfoLogLength;

		// Compile Vertex Shader
		printf("Compiling vertex shader\n");
		char const * VertexSourcePointer = vertexShader;
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
		printf("Compiling fragment shader\n");
		char const * FragmentSourcePointer = fragmentShader;
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
		
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mat.cols, mat.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, flippedMat.ptr());

		// texture filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);

		return textureID;
	}
}

