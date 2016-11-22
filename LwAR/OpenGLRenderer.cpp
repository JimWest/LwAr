#include "OpenGLRenderer.h"

using namespace std;

OpenGLRenderer::OpenGLRenderer()
{
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

void OpenGLRenderer::InitGL()
{
	// Define our buffer settings
	int redBits = 8, greenBits = 8, blueBits = 8;
	int alphaBits = 8, depthBits = 24, stencilBits = 8;

	// Initialise glfw
	glfwInit();

	window = glfwCreateWindow(windowWidth, windowHeight, "OpenCV / OpenNI Sensor Data to Texture | r3dux", NULL, NULL);

	glfwMakeContextCurrent(window);

	// Create a window
	if (!window)
	{
		cout << "Failed to open window!" << endl;
		glfwTerminate();
		exit(-1);
	}

	// Specify the callback function for key presses/releases
	glfwSetKeyCallback(window, key_callback);

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

GLuint OpenGLRenderer::matToTexture(cv::Mat & mat, GLenum minFilter, GLenum magFilter, GLenum wrapFilter)
{
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
		mat.ptr());        // The actual image data itself

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


