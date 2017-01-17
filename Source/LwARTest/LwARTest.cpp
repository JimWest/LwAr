// LwARTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../LwAR/Window.h"
#include "glm.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>


int width = 640;
int height = 480;
std::string windowName = "Augmented Reality Test";
lwar::Camera camera;

void onUpdate(lwar::Window& window, float deltaTime);

int main()
{
	lwar::Window window = lwar::Window(width, height, windowName);
	window.onUpdate = onUpdate;

	camera = lwar::Camera(0, width, height);
	camera.init();

	if (!camera.isOpened)
	{
		return -1;
	}

	//lwar::Object3d cube = lwar::Object3d("monkey.obj");
	lwar::Object3d cube = lwar::Object3d(lwar::Primitves::Cube);
	cube.transform.translation = glm::vec3(-0.6f, -0.6f, 0);
	cube.transform.scale = glm::vec3(0.4f, 0.4f, 0.4f);
	cube.transform.rotation = glm::quat(glm::vec3(0, 20, 20));
	cube.material.texture = lwar::Material::ColorGradient();
	cube.material.drawMode = lwar::DrawMode::Triangles;

	window.addObject(cube);

	window.addText(lwar::Text("!ABCDEFGHAJKLMOPQRSTUVWXYZ", 10, 500, 30));
	// starts the main loop
	window.start();

	return 0;
}


void onUpdate(lwar::Window& window, float deltaTime)
{
	lwar::Scene& scene = window.getScene();
	cv::Mat camFrame;

	if (camera.isOpened)
	{
		camFrame = camera.retrieve();
		// mirror on y axis so its like a mirror
		cv::flip(camFrame, camFrame, 1);
	}

	// set the background of the window to the current camera image
	window.setBackground(camFrame);

	// check for keys
	int key = window.getLastKey();
	// print pressed key
	//std::cout << std::to_string(key) << std::endl;

	// 263 = left
	// 265 = up
	// 264 = down
	// 262 = right

	glm::vec3 movementVec = glm::vec3(0, 0, 0);
	if (key == 263)
		movementVec.x = -1;
	if (key == 262)
		movementVec.x = 1;
	if (key == 265)
		movementVec.y = 1;
	if (key == 264)
		movementVec.y = -1;

	scene.objects[0].transform.translation += movementVec * deltaTime;

	// escape == exit
	if (key == 256)
		window.stop();
}

