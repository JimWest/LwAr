#include "OpenGLRenderer.h"

using namespace std;

OpenGLRenderer::OpenGLRenderer()
{
	initGL();
}

OpenGLRenderer::OpenGLRenderer(int windowWidth, int windowHeight, std::string windowTitle)
{
	this->windowWidth = windowWidth;
	this->windowHeight = windowHeight;
	this->windowTitle = windowTitle;
	initGL();
}


OpenGLRenderer::~OpenGLRenderer()
{
	glfwTerminate();
}


void OpenGLRenderer::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
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

	// Create a window
	window = glfwCreateWindow(windowWidth, windowHeight, windowTitle.c_str(), NULL, NULL);
	glfwMakeContextCurrent(window);

	if (!window)
	{
		cout << "Failed to open window!" << endl;
		glfwTerminate();
		exit(-1);
	}

	// Specify the callback function for key presses/releases
	glfwSetKeyCallback(window, keyCallback);
	glfwSetWindowSizeCallback(window, windowSizeCallback);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

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
	GLfloat aspectRatio = (windowWidth > windowHeight) ? float(windowWidth) / float(windowHeight) : float(windowHeight) / float(windowWidth);
	GLfloat fH = tan(float(fieldOfView / 360.0f * 3.14159f)) * zNear;
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
}

void OpenGLRenderer::initObject(Object3d& object)
{
	glGenVertexArrays(1, &(GLuint)object.vao);
	glBindVertexArray((GLuint)object.vao);

	glGenBuffers(1, &(GLuint)object.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, object.vbo);
	glBufferData(GL_ARRAY_BUFFER, object.vertices.size() * sizeof(float), &object.vertices[0], GL_STATIC_DRAW);

	GLuint uvbuffer;
	glGenBuffers(1, &(GLuint)object.uvbo);
	glBindBuffer(GL_ARRAY_BUFFER, object.uvbo);
	glBufferData(GL_ARRAY_BUFFER, object.uvs.size() * sizeof(float), &object.uvs[0], GL_STATIC_DRAW);
}


void OpenGLRenderer::preDraw()
{
	quit = glfwWindowShouldClose(window);
	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

GLuint raw_texture_load(Material mat)
{
	GLuint textureID;

	// allocate a texture name
	glGenTextures(1, &textureID);

	// select our current texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	// select modulate to mix texture with color for shading
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_DECAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_DECAL);

	// when texture area is small, bilinear filter the closest mipmap
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	// when texture area is large, bilinear filter the first mipmap
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// texture should tile
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// build our texture mipmaps
	glTexImage2D(GL_TEXTURE_2D,     // Type of texture
		0,                 // Pyramid level (for mip-mapping) - 0 is the top level
		GL_RGB,            // Internal colour format to convert to
		512,          // Image width  i.e. 640 for Kinect in standard mode
		512,          // Image height i.e. 480 for Kinect in standard mode
		0,                 // Border width in pixels (can either be 1 or 0)
		GL_BGR, // Input image format (i.e. GL_RGB, GL_RGBA, GL_BGR etc.)
		GL_UNSIGNED_BYTE,  // Image data type
		mat.texture.ptr());

	return textureID;
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
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
	glm::mat4 viewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f),
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
	GLuint textureID = matToTexture(object.material.texture, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_CLAMP);
	//GLuint textureID = raw_texture_load(object->material);
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

	// Draw the object
	glDrawArrays(GL_TRIANGLES, 0, (object.vertices.size()));

	if (ignoreDepth)
		glEnable(GL_DEPTH_TEST);


	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	// Free the texture memory
	glDeleteTextures(1, &textureID);
	glDisable(GL_TEXTURE_2D);
}

void OpenGLRenderer::postDraw()
{
	glfwSwapBuffers(window);
	glfwPollEvents();

	// check OpenGL error
	GLenum err;
	if ((err = glGetError()) != GL_NO_ERROR) {
		cerr << "OpenGL error: " << err << ", " << gluErrorString(err) << endl;
	}
}

GLuint OpenGLRenderer::loadShaders(const char * vertex_file_path, const char * fragment_file_path) {

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

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

	// Catch silly-mistake texture interpolation method for magnification
	if (magFilter == GL_LINEAR_MIPMAP_LINEAR ||
		magFilter == GL_LINEAR_MIPMAP_NEAREST ||
		magFilter == GL_NEAREST_MIPMAP_LINEAR ||
		magFilter == GL_NEAREST_MIPMAP_NEAREST)
	{
		std::cout << "You can't use MIPMAPs for magnification - setting filter to GL_LINEAR" << std::endl;
		magFilter = GL_LINEAR;
	}

	// Set texture interpolation methods for minification and magnification
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

	// Set texture clamping method
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapFilter);

	// Set incoming texture format to:
	// GL_BGR       for CV_CAP_OPENNI_BGR_IMAGE,
	// GL_LUMINANCE for CV_CAP_OPENNI_DISPARITY_MAP,
	// Work out other mappings as required ( there's a list in comments in main() )
	GLenum inputColourFormat = GL_BGR;
	if (mat.channels() == 1)
	{
		inputColourFormat = GL_LUMINANCE;
	}

	// Create the texture
	glTexImage2D(GL_TEXTURE_2D,     // Type of texture
		0,                 // Pyramid level (for mip-mapping) - 0 is the top level
		GL_RGB,            // Internal colour format to convert to
		mat.cols,          // Image width  i.e. 640 for Kinect in standard mode
		mat.rows,          // Image height i.e. 480 for Kinect in standard mode
		0,                 // Border width in pixels (can either be 1 or 0)
		inputColourFormat, // Input image format (i.e. GL_RGB, GL_RGBA, GL_BGR etc.)
		GL_UNSIGNED_BYTE,  // Image data type
		flippedMat.ptr());        // The actual image data itself

						   // If we're using mipmaps then generate them. Note: This requires OpenGL 3.0 or higher
	if (minFilter == GL_LINEAR_MIPMAP_LINEAR ||
		minFilter == GL_LINEAR_MIPMAP_NEAREST ||
		minFilter == GL_NEAREST_MIPMAP_LINEAR ||
		minFilter == GL_NEAREST_MIPMAP_NEAREST)
	{
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	return textureID;
}


