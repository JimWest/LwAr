// LwARTest.cpp : Defines the entry point for the console application.
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



// taken from opencv example,
// http://docs.opencv.org/2.4/doc/tutorials/imgproc/shapedescriptors/bounding_rotated_ellipses/bounding_rotated_ellipses.html

void boundingBoxEllipse(lwar::Window& window, lwar::Scene& scene, cv::Mat& camFrame)
{
	int thresh = 100;
	int max_thresh = 255;
	cv::RNG rng(12345);

	cv::Mat greyImage;
	cv::Mat threshold_output;
	std::vector<cv::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;

	cv::cvtColor(camFrame, greyImage, CV_BGR2GRAY);
	cv::blur(greyImage, greyImage, cv::Size(3, 3));

	// Detect edges using Threshold
	cv::threshold(greyImage, threshold_output, thresh, 255, cv::THRESH_BINARY);
	// Find contours
	cv::findContours(threshold_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

	// Find the rotated rectangles and ellipses for each contour
	std::vector<cv::RotatedRect> minRect(contours.size());
	std::vector<cv::RotatedRect> minEllipse(contours.size());

	for (int i = 0; i < contours.size(); i++)
	{
		minRect[i] = cv::minAreaRect(cv::Mat(contours[i]));
		if (contours[i].size() > 5)
		{
			minEllipse[i] = cv::fitEllipse(cv::Mat(contours[i]));
		}
	}

	// Draw contours + rotated rects + ellipses
	cv::Mat drawing = cv::Mat::zeros(threshold_output.size(), CV_8UC3);
	for (int i = 0; i < contours.size(); i++)
	{
		cv::Scalar color = cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		// contour
		cv::drawContours(drawing, contours, i, color, 1, 8, std::vector<cv::Vec4i>(), 0, cv::Point());
		// ellipse
		cv::ellipse(drawing, minEllipse[i], color, 2, 8);
		//rotated rectangle
		cv::Point2f rect_points[4]; minRect[i].points(rect_points);
		for (int j = 0; j < 4; j++)
			line(drawing, rect_points[j], rect_points[(j + 1) % 4], color, 1, 8);
	}

	camFrame = drawing;
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
	//boundingBoxEllipse(window, scene, camFrame);
	//getFace(window, scene, camFrame);


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

	std::cout << "Opening Webcam device ..." << std::endl;

	lwar::Camera camera = lwar::Camera(0, width, height, 60);
	camera.init();

	if (!camera.isOpened)
	{
		std::cout << "Error openeing camera" << std::endl;
		//return -1;
	}

	window.getScene().camera = camera;

	char cCurrentPath[FILENAME_MAX];

	if (!_getcwd(cCurrentPath, sizeof(cCurrentPath)))
	{
		return errno;
	}

	lwar::Object3d cube = lwar::Object3d("cube.obj");
	//lwar::Object3d cube = lwar::Object3d(lwar::Primitves::Cube);
	cube.transform.scale = glm::vec3(0.2f, 0.2f, 0.2f);
	cube.transform.rotation = glm::quat(glm::vec3(0, 45, 45));
	cube.material.texture = lwar::Material::ColorGradient();

	window.addObject(cube);

	//window.background.visible = false;
	// starts the main loop
	window.start();

	return 0;
}

