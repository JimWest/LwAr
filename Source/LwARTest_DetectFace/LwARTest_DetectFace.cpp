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
std::string windowName = "LWAR - Detect Face Application";
lwar::Camera camera;

cv::CascadeClassifier faceCascade;
cv::Point oldCenter;
int oldRadius;
int framesWithoutFace = 0;

void onUpdate(lwar::Window& window, float deltaTime);

int main()
{
	lwar::Window window;
	window.onUpdate = onUpdate;

	window.create(width, height, windowName);

	camera = lwar::Camera(0, width, height);
	camera.init();

	if (!camera.isOpened)
	{
		return -1;
	}

	bool loaded = faceCascade.load("..\\..\\..\\..\\ExternalLibs\\opencv\\etc\\haarcascades\\haarcascade_frontalface_alt.xml");

	if (!loaded)
	{ 		
		std::cout << "Can't load face cascade file" << std::endl;
		getchar();
		return -1;
	}

	lwar::Object3d monkey = lwar::Object3d("monkey.obj");
	monkey.transform.scale = glm::vec3(0.7f, 0.7f, 0.7f);
	monkey.transform.rotation = glm::quat(glm::vec3(0, 0, 0));
	monkey.material.texture = cv::Scalar(112, 25, 25);

	window.addObject(monkey);
	
	// starts the main loop
	window.start();

	return 0;
}


void getFace(lwar::Window& window, lwar::Scene& scene, cv::Mat& camFrame, float deltaTime)
{
	cv::Mat gray;
	cv::cvtColor(camFrame, gray, cv::COLOR_RGB2GRAY);
	cv::equalizeHist(gray, gray);

	std::vector<cv::Rect> faces;
	bool faceFound = false;

	cv::Point center;
	int radius = 0;

	// Detect faces
	faceCascade.detectMultiScale(gray, faces, 1.4f, 1, 0 | CV_HAAR_FIND_BIGGEST_OBJECT, cv::Size(80, 80));

	if (faces.size() > 0)
	{
		// show the face only in a separate window
		//cv::Mat faceROI = cv::Mat(gray, faces[0]);
		//cv::imshow("Detected Face", faceROI);

		center = cv::Point(faces[0].x + static_cast<int>(round(faces[0].width * 0.5)), static_cast<int>(round(faces[0].y + faces[0].height*0.5)));
		radius = cvRound((faces[0].width + faces[0].height)*0.25);

		faceFound = true;
	}
	// only lose the face if it wasnt detected for 10 frames;
	else if (radius > 0 && framesWithoutFace <= 10)
	{
		center = oldCenter;
		radius = oldRadius;
		faceFound = true;
		framesWithoutFace++;
	}
	else
	{
		radius = 0;
		framesWithoutFace = 0;
	}

	if (faceFound)
	{
		// draw an ellipse for debugging
		//cv::ellipse(camFrame, center, cv::Size(faces[0].width*0.5, faces[0].height*0.5), 0, 0, 360, cv::Scalar(255, 0, 255), 4, 8, 0);
		glm::vec3 point = window.screenToWorldPoint(glm::vec2(center.x, center.y));

		// get the reference of the monkey of the scene
		lwar::Object3d& monkey = scene.objects.at(0);

		// move the object to the position of the circle smoothly
		monkey.transform.translation = glm::mix(monkey.transform.translation, point, 10 * deltaTime);

		// set the scale to the radius so it fits the whole circle
		monkey.transform.scale = glm::mix(monkey.transform.scale, glm::vec3(1.5f) * window.screenToWorldDistance(static_cast<float>(radius)), 2.5f * deltaTime);
		oldCenter = center;
		oldRadius = radius;
	}
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

	getFace(window, scene, camFrame, deltaTime);

	// set the background of the window to the current camera image
	window.setBackground(camFrame);

	// check for keys
	int key = window.getLastKey();
	// escape == exit
	if (key == 256)
		window.stop();
}