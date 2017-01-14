// LwARTest_ArucoMarker.cpp : Defines the entry point for the console application.
//

//#pragma comment(lib, "aruco201.lib")

#include "stdafx.h"
#include "../LwAR/Window.h"
#include "glm.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/nonfree.hpp>
//#include <aruco/aruco.h>
//#include <aruco/cvdrawingutils.h>

void onUpdate(lwar::Window& window, float deltaTime);

int width = 640;
int height = 480;
std::string windowName = "Augmented Reality Test";

//aruco::MarkerDetector MDetector;
//vector<aruco::Marker> Markers;
cv::RNG rng(12345);

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

	cv::initModule_nonfree();

	// starts the main loop
	window.start();

	return 0;
}


void test(cv::OutputArray _contours)
{
	_contours.create(88, 1, 0, -1, true);
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

	// detector crashs if the picture is entirely black
	//if (cv::sum(camFrame).val[0] > 0.0)
	//{
	//	MDetector.detect(camFrame, Markers);
	//	//for each marker, draw info and its boundaries in the image
	//	for (unsigned int i = 0; i < Markers.size(); i++) {
	//		cout << Markers[i] << endl;
	//		Markers[i].draw(camFrame, cv::Scalar(0, 0, 255), 2);
	//	}
	//}

	std::vector< cv::Vec4i > hierarchy;
	//std::vector< std::vector< cv::Point > > contours;

	std::vector<cv::Mat> contours;

	cv::Mat canny_output;
	cv::Mat gray;
	cv::cvtColor(camFrame, gray, cv::COLOR_RGB2GRAY);
	cv::equalizeHist(gray, gray);

	// Detect edges using canny
	cv::Canny(gray, canny_output, 100, 100 * 2, 3);

	cv::findContours(canny_output, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);

	for (int i = 0; i < contours.size(); i++)
	{
		cv::Scalar color = cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		drawContours(canny_output, contours, i, color, 2, 8, hierarchy, 0, cv::Point());
	}

	// set the background of the window to the current camera image

	window.setBackground(canny_output);

	// check for keys
	int key = window.getLastKey();
	// escape == exit
	if (key == 256)
		window.stop();
}
