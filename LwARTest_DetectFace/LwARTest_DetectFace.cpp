// LwARTest_DetectFace.cpp : Defines the entry point for the console application.
//
#pragma comment(lib, "dlib.lib")

#include "stdafx.h"
#include "../LwAR/Window.h"
#include "glm.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <direct.h>

#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib\all\source.cpp>



int width = 640;
int height = 480;
std::string windowName = "Augmented Reality Test";

float faceScale = 1.6f;
int faceMinNeightbors = 5;
cv::Size size = cv::Size(30, 30);

cv::CascadeClassifier faceCascade;
//dlib::frontal_face_detector detector;

void getFace(lwar::Window& window, lwar::Scene& scene, cv::Mat& camFrame)
{
	cv::Mat gray;
	cv::cvtColor(camFrame, gray, cv::COLOR_RGB2GRAY);
	cv::equalizeHist(gray, gray);

	//std::vector<cv::Rect> objects;

	//// Detect faces
	//faceCascade.detectMultiScale(
	//	gray,
	//	objects,
	//	faceScale,
	//	faceMinNeightbors,
	//	0 | CV_HAAR_SCALE_IMAGE,
	//	size
	//);

	//std::vector<dlib::rectangle> objects = detector(camFrame);

	//for (int i = 0; i < objects.size(); ++i)
	//{
	//	//cv::Point center(objects[i].x + objects[i].width*0.5, objects[i].y + objects[i].height*0.5);
	//	//cv::ellipse(camFrame, center, cv::Size(objects[i].width*0.5, objects[i].height*0.5), 0, 0, 360, cv::Scalar(255, 0, 255), 4, 8, 0);
	//	int a = 1;

	//}



}

void onUpdate(lwar::Window& window)
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
	window.background.material.texture = camFrame;

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

	lwar::Camera camera = lwar::Camera(0, width, height, 60);
	camera.init();

	if (!camera.isOpened)
	{
		return -1;
	}

	bool loaded = faceCascade.load("E:\\HSKA\\Master\\1. Semester\\Projektarbeit\\Code\\LwAr\\ExternalLibs\\opencv2\\build\\share\\OpenCV\\haarcascades\\haarcascade_frontalface_alt.xml");

	if (!loaded)
		std::cout << "Can't load cascade file" << std::endl;

	//detector = dlib::get_frontal_face_detector();


	window.getScene().camera = camera;

	// starts the main loop
	window.start();

	return 0;
}
