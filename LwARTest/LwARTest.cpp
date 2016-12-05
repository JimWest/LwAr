// LwARTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../LwAR/LwAR.h"
#include "../LwAR/glm/glm.hpp"
#include "../LwAR/glm/gtc/matrix_transform.hpp"
#include "../LwAR/glm/gtc/quaternion.hpp"
#include "../LwAR/glm/gtx/quaternion.hpp"
#include "../LwAR/glm/gtx/euler_angles.hpp"


int width = 640;
int height = 480;
std::string windowName = "Augmented Reality Test";


Object3d background;
Object3d cube;

int i = 0;

void onUpdate()
{
	cube.transform.rotation = glm::quat(glm::vec3(10, 1 * i / 100.0f, 1 * i / 100.0f));
	i++;
}

int main()
{
	OpenGLRenderer renderer = OpenGLRenderer(width, height, windowName);

	LwAR lwar = LwAR(&renderer);
	lwar.onUpdate = onUpdate;

	// Create a our video capture using the Kinect and OpenNI
	// Note: To use the cv::VideoCapture class you MUST link in the highgui lib (libopencv_highgui.so)
	std::cout << "Opening Webcam device ..." << std::endl;

	Camera _cam = Camera(0, width, height, 60);
	if (!_cam.isOpened)
	{
		std::cout << "Error openeing camera" << std::endl;
		//return -1;
	}

	background = Object3d(Object3d::Quad);
	cube = Object3d(Object3d::Cube);

	renderer.initObject(&background);
	renderer.initObject(&cube);

	cube.transform.translation = glm::vec3(-0.2f, 0.1f, 0.1f);
	cube.transform.scale = glm::vec3(0.5f, 0.5f, 0.5f);
	cube.transform.rotation = glm::quat(glm::vec3(10, 10, 10));

	lwar.AddObject(&background);
	lwar.AddObject(&cube);

	lwar.Start();


    return 0;
}

