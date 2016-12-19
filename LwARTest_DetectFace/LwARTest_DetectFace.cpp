// LwARTest_DetectFace.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../LwAR/Window.h"
#include "glm.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <direct.h>


int width = 640;
int height = 480;
std::string windowName = "Augmented Reality Test";

float faceScale = 1.6f;
int faceMinNeightbors = 2;
cv::Size size = cv::Size(50, 50);

cv::CascadeClassifier faceCascade;
//dlib::frontal_face_detector detector;

std::vector<cv::Rect> getFace(lwar::Window& window, lwar::Scene& scene, cv::Mat& camFrame)
{
	cv::Mat gray;
	cv::cvtColor(camFrame, gray, cv::COLOR_RGB2GRAY);
	cv::equalizeHist(gray, gray);

	std::vector<cv::Rect> objects;

	// Detect faces
	faceCascade.detectMultiScale(
		gray,
		objects,
		faceScale,
		faceMinNeightbors,
		0 | cv::CASCADE_SCALE_IMAGE,
		size
	);

	return objects;
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

	std::vector<cv::Rect> objects = getFace(window, scene, camFrame);

	for (int i = 0; i < objects.size(); ++i)
	{
		cv::Point center(objects[i].x + objects[i].width*0.5, objects[i].y + objects[i].height*0.5);
		int radius = cvRound((objects[i].width + objects[i].height)*0.25);

		cv::ellipse(camFrame, center, cv::Size(objects[i].width*0.5, objects[i].height*0.5), 0, 0, 360, cv::Scalar(255, 0, 255), 4, 8, 0);
		

		glm::vec3 point = window.screenToWorldPoint(glm::vec2(center.x, center.y));


		lwar::Object3d& monkey = scene.objects[0];

		// move the object to the position of the circle
		monkey.transform.translation = point;

		// set the scale to the radius so it fits the whole circle
		monkey.transform.scale = glm::vec3(1.5f) *  window.screenToWorldDistance(radius);

	}

	// set the background of the window to the current camera image
	window.setBackground(camFrame);

	// check for keys
	int key = window.getLastKey();
	// escape == exit
	if (key == 256)
		window.stop();
}

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

	bool loaded = faceCascade.load("..\\ExternalLibs\\opencv2\\build\\share\\OpenCV\\haarcascades\\haarcascade_frontalface_alt.xml");

	if (!loaded)
		std::cout << "Can't load cascade file" << std::endl;

	window.getScene().camera = camera;

	lwar::Object3d monkey = lwar::Object3d("monkey.obj");
	//lwar::Object3d cube = lwar::Object3d(lwar::Primitves::Cube);
	monkey.transform.scale = glm::vec3(0.7f, 0.7f, 0.7f);
	monkey.transform.rotation = glm::quat(glm::vec3(0, 0, 0));
	monkey.material.texture = cv::Scalar(112, 25, 25);

	window.addObject(monkey);


	// starts the main loop
	window.start();

	return 0;
}
