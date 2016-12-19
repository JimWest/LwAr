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
cv::CascadeClassifier eyeCascade;

std::vector<cv::Rect> getFace(lwar::Window& window, lwar::Scene& scene, cv::Mat& camFrame)
{
	cv::Mat gray;
	cv::cvtColor(camFrame, gray, cv::COLOR_RGB2GRAY);
	cv::equalizeHist(gray, gray);

	std::vector<cv::Rect> faces;

	// Detect faces
	faceCascade.detectMultiScale(gray, faces, faceScale, faceMinNeightbors, 0 | cv::CASCADE_SCALE_IMAGE, size);

	for (int i = 0; i < faces.size(); ++i)
	{
		cv::Mat faceROI = cv::Mat(gray, faces[i]);
		//cv::imshow("Detected Face", faceROI);

		std::vector<cv::Rect> eyes;
		cv::Point eyeCenterDelta;

		eyeCascade.detectMultiScale(faceROI, eyes, 1.2, faceMinNeightbors, 0 | cv::CASCADE_SCALE_IMAGE, cv::Size(20, 20));

		cv::Point center(faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5);
		int radius = cvRound((faces[i].width + faces[i].height)*0.25);

		if (eyes.size() > 0)
		{

			//cv::Mat eyeROI = cv::Mat(gray, eyes[j]);
			//cv::imshow("Detected Eyes", eyeROI);

			cv::Point eyeCenter(faces[i].x + eyes[0].x + eyes[0].width*0.5, faces[i].y + eyes[0].y + eyes[0].height*0.5);
			//int eyeRadius = cvRound((eyes[j].width + eyes[j].height)*0.25);

			eyeCenterDelta = center - eyeCenter;
			cv::ellipse(camFrame, eyeCenter, cv::Size(eyes[0].width * 0.25, eyes[0].height * 0.25), 0, 0, 360, cv::Scalar(255, 0, 255), 4, 8, 0);
		}

		// 0 rotation = looking forward 
		cv::Point eyeForward = cv::Point(40, 30);

		cv::ellipse(camFrame, center, cv::Size(faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, cv::Scalar(255, 0, 255), 4, 8, 0);
		glm::vec3 point = window.screenToWorldPoint(glm::vec2(center.x, center.y));

		lwar::Object3d& monkey = scene.objects[0];

		// rotate it accordlingly to the face rotation 
		monkey.transform.rotation = glm::quat(glm::vec3(glm::radians(-(float)(eyeForward - eyeCenterDelta).y), glm::radians(-(float)(eyeForward - eyeCenterDelta).x), 0));

		// move the object to the position of the circle
		monkey.transform.translation = point;

		// set the scale to the radius so it fits the whole circle
		monkey.transform.scale = glm::vec3(1.5f) *  window.screenToWorldDistance(radius);

	}

	return faces;
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

	getFace(window, scene, camFrame);

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
		std::cout << "Can't load face cascade file" << std::endl;

	loaded = eyeCascade.load("..\\ExternalLibs\\opencv2\\build\\share\\OpenCV\\haarcascades\\haarcascade_righteye_2splits.xml");

	if (!loaded)
		std::cout << "Can't load eye cascade file" << std::endl;


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
