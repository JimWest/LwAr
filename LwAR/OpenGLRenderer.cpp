#include "OpenGLRenderer.h"

using namespace std;

OpenGLRenderer::OpenGLRenderer()
{
	InitGL();
}

OpenGLRenderer::OpenGLRenderer(int windowWidth, int windowHeight, std::string windowTitle)
{
	this->windowWidth = windowWidth;
	this->windowHeight = windowHeight;
	this->windowTitle = windowTitle;
	InitGL();
}


OpenGLRenderer::~OpenGLRenderer()
{
	glfwTerminate();
}


void OpenGLRenderer::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void OpenGLRenderer::window_size_callback(GLFWwindow* window, int width, int height)
{
}

void OpenGLRenderer::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void OpenGLRenderer::InitGL()
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
	glfwSetKeyCallback(window, key_callback);
	glfwSetWindowSizeCallback(window, window_size_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//  Initialise glew (must occur AFTER window creation or glew will error)
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

	glDepthFunc(GL_LEQUAL);		// Specify depth function to use
	glEnable(GL_DEPTH_TEST);    // Enable the depth buffer
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Ask for nicest perspective correction
	glEnable(GL_CULL_FACE);     // Cull back facing polygons
	glfwSwapInterval(1);        // Lock screen updates to vertical refresh
								// Switch to ModelView matrix and reset
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set our clear colour to black

	programID = LoadShaders("shaders/VertexShader.vertexshader", "shaders/FragmentShader.fragmentshader");
}

void OpenGLRenderer::PrepareObject(const Object3d* object)
{
	glGenVertexArrays(1, &(GLuint)object->vao);
	glBindVertexArray((GLuint)object->vao);

	// Generate 1 buffer, put the resulting identifier in vertexbuffer
	glGenBuffers(1, &(GLuint)object->vbo);
	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, (GLuint)object->vao);
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, object->vertices.size() * sizeof(float), &object->vertices.front(), GL_STATIC_DRAW);

	// Retrieve the vertex location in the program.
	GLint vertLoc = glGetAttribLocation(programID, "vert");

	// connect the xyz to the "vert" attribute of the vertex shader
	glEnableVertexAttribArray(vertLoc);
	glVertexAttribPointer(vertLoc, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), NULL);

	// Retrieve the vertex location in the program.
	GLint vertCoordLoc = glGetAttribLocation(programID, "vertTexCoord");

	// connect the uv coords to the "vertTexCoord" attribute of the vertex shader
	glEnableVertexAttribArray(vertCoordLoc);
	glVertexAttribPointer(vertCoordLoc, 2, GL_FLOAT, GL_TRUE, 5 * sizeof(GLfloat), (const GLvoid*)(3 * sizeof(GLfloat)));

	glUseProgram(programID);
}
//
//void OpenGLRenderer::PrepareObject(const Object3d* object)
//{
//	glGenVertexArrays(1, &this->VAO);
//	glGenBuffers(1, &this->VBO);
//	glGenBuffers(1, &this->EBO);
//
//	glBindVertexArray(this->VAO);
//	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
//
//	glBufferData(GL_ARRAY_BUFFER, object->vertices.size() * sizeof(Vertex),
//		&object->vertices[0], GL_STATIC_DRAW);
//
//	GLint vertLoc = glGetAttribLocation(programID, "vert");
//	GLint vertCoordLoc = glGetAttribLocation(programID, "vertTexCoord");
//
//	// Vertex Positions
//	glEnableVertexAttribArray(vertLoc);
//	glVertexAttribPointer(vertLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
//		(GLvoid*)0);
//	// Vertex Normals
//	//glEnableVertexAttribArray(1);
//	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
//	//	(GLvoid*)offsetof(Vertex, Normal));
//
//	// Vertex Texture Coords
//	glEnableVertexAttribArray(vertCoordLoc);
//	glVertexAttribPointer(vertCoordLoc, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
//		(GLvoid*)offsetof(Vertex, TexCoords));
//
//	//// connect the xyz to the "vert" attribute of the vertex shader
//	//glEnableVertexAttribArray(vertLoc);
//	//glVertexAttribPointer(vertLoc, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), NULL);
//
//
//	//// connect the uv coords to the "vertTexCoord" attribute of the vertex shader
//	//glEnableVertexAttribArray(vertCoordLoc);
//	//glVertexAttribPointer(vertCoordLoc, 2, GL_FLOAT, GL_TRUE, 5 * sizeof(GLfloat), (const GLvoid*)(3 * sizeof(GLfloat)));
//
//	glBindVertexArray(0);
//	glUseProgram(programID);
//}

int i = 0;

void OpenGLRenderer::PreDraw()
{
	quit = glfwWindowShouldClose(window);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRenderer::DrawObject(Object3d* object, cv::Mat &camFrame)
{


	// Convert image and depth data to OpenGL textures
	GLuint imageTex = matToTexture(camFrame, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_CLAMP);

	// Draw the textures
	// Note: Window co-ordinates origin is top left, texture co-ordinate origin is bottom left.

	// Front facing texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, imageTex);
	GLint texLocation = glGetUniformLocation(programID, "tex");
	glUniform1i(texLocation, GL_TEXTURE0);

	glTranslatef(0, 0, i);

	i--;

	glBindVertexArray((GLuint)object->vao);
	// Draw the object 
	glDrawArrays(GL_QUADS, 0, 4);

	glBindVertexArray(0);

	// Free the texture memory
	glDeleteTextures(1, &imageTex);
	glDisable(GL_TEXTURE_2D);


}

void OpenGLRenderer::PostDraw()
{
	glfwSwapBuffers(window);
	glfwPollEvents();
}

void OpenGLRenderer::Draw(cv::Mat &camFrame)
{
	quit = glfwWindowShouldClose(window);

	// Clear the screen and depth buffer, and reset the ModelView matrix to identity
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	// Move things back into the screen
	glTranslatef(0.0f, 0.0f, -8.0f);

	// Rotate around the y-axis
	glRotatef(frameCount, 0.0f, 1.0f, 0.0f);

	// Rotate around the x-axis
	static float rateOfChange = 0.01f;
	static float degreesToMoveThrough = 180.0f;
	glRotatef(sin(frameCount * rateOfChange) * degreesToMoveThrough, 1.0f, 0.0f, 0.0f);

	// Rotate around the z-axis
	glRotatef(cos(frameCount * rateOfChange) * degreesToMoveThrough, 0.0f, 1.0f, 0.0f);

	glEnable(GL_TEXTURE_2D);

	// Quad width and height
	float w = 6.4f;
	float h = 4.8f;

	// Convert image and depth data to OpenGL textures
	GLuint imageTex = matToTexture(camFrame, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_CLAMP);

	// Draw the textures
	// Note: Window co-ordinates origin is top left, texture co-ordinate origin is bottom left.

	// Front facing texture
	glBindTexture(GL_TEXTURE_2D, imageTex);
	glBegin(GL_QUADS);
	glTexCoord2f(1, 1);
	glVertex2f(-w / 2, h / 2);
	glTexCoord2f(0, 1);
	glVertex2f(w / 2, h / 2);
	glTexCoord2f(0, 0);
	glVertex2f(w / 2, -h / 2);
	glTexCoord2f(1, 0);
	glVertex2f(-w / 2, -h / 2);
	glEnd();

	// Free the texture memory
	glDeleteTextures(1, &imageTex);

	glDisable(GL_TEXTURE_2D);

	glfwSwapBuffers(window);
	glfwPollEvents();
}

GLuint OpenGLRenderer::LoadShaders(const char * vertex_file_path, const char * fragment_file_path) {

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
GLuint OpenGLRenderer::matToTexture(cv::Mat & mat, GLenum minFilter, GLenum magFilter, GLenum wrapFilter)
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


