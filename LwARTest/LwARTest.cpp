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

void onUpdate(lwar::Window& window, float deltaTime);

int main()
{
	lwar::Window window = lwar::Window(width, height, windowName);
	window.onUpdate = onUpdate;

	lwar::Camera camera = lwar::Camera(0, width, height);
	camera.init();

	if (!camera.isOpened)
	{
		return -1;
	}

	window.getScene().camera = camera;

	//lwar::Object3d cube = lwar::Object3d("monkey.obj");
	lwar::Object3d cube = lwar::Object3d(lwar::Primitves::Cube);
	cube.transform.scale = glm::vec3(0.7f, 0.7f, 0.7f);
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
	lwar::Renderer* renderer = window.getRenderer();
	lwar::Scene& scene = window.getScene();
	cv::Mat camFrame;

	if (scene.camera.isOpened)
	{
		camFrame = scene.camera.retrieve();
		// mirror on y axis so its like a mirror
		cv::flip(camFrame, camFrame, 1);
	}

	// set the background of the window to the current camera image
	window.setBackground(camFrame);

	// check for keys
	int key = window.getLastKey();
	// escape == exit
	if (key == 256)
		window.stop();
}

