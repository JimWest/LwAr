// LwARTest_DetectRedCircles.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../LwAR/Window.h"
#include "glm.hpp"

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <direct.h>
#include <gtx/euler_angles.hpp>


std::vector<cv::Vec3f> detectRedCircles(cv::Mat& image)
{
	std::vector<cv::Vec3f> circles;
	if (image.empty())
		return circles;

	cv::medianBlur(image, image, 3);

	// Convert input image to HSV
	cv::Mat hsv_image;
	cv::cvtColor(image, hsv_image, cv::COLOR_BGR2HSV);

	// Threshold the HSV image, keep only the red pixels
	cv::Mat lower_red_hue_range;
	cv::Mat upper_red_hue_range;
	cv::inRange(hsv_image, cv::Scalar(0, 100, 100), cv::Scalar(10, 255, 255), lower_red_hue_range);

	cv::inRange(hsv_image, cv::Scalar(160, 100, 100), cv::Scalar(179, 255, 255), upper_red_hue_range);

	// Combine the above two images
	cv::Mat red_hue_image;
	cv::addWeighted(lower_red_hue_range, 1.0, upper_red_hue_range, 1.0, 0.0, red_hue_image);

	cv::GaussianBlur(red_hue_image, red_hue_image, cv::Size(9, 9), 2, 2);

	// Use the Hough transform to detect circles in the combined threshold image
	cv::HoughCircles(red_hue_image, circles, CV_HOUGH_GRADIENT, 2, red_hue_image.rows / 8, 30, 100, 0, 100);

	return circles;
}

void cubesOnCircles(lwar::Window& window, lwar::Scene& scene, cv::Mat& camFrame)
{
	// find all red circles in the scene
	std::vector<cv::Vec3f> circles = detectRedCircles(camFrame);

	// Loop over all detected circles and outline them on the original image
	if (circles.size() != 0)
	{
		for (int i = 0; i < circles.size(); ++i)
		{
			cv::Point center(std::round(circles[i][0]), std::round(circles[i][1]));
			int radius = std::round(circles[i][2]);

			// draw circles into the image vor visualisation
			cv::circle(camFrame, center, radius, cv::Scalar(0, 255, 0), 5);

			glm::vec3 point = window.screenToWorldPoint(glm::vec2(center.x, center.y));

			if (scene.objects.size() < circles.size())
			{
				lwar::Object3d cube = lwar::Object3d(lwar::Primitves::Cube);
				cube.material.texture = lwar::Material::ColorGradient();

				window.addObject(cube);
			}

			lwar::Object3d& cube = scene.objects[i];

			// move the object to the position of the circle
			cube.transform.translation = point;

			// set the scale to the radius so it fits the whole circle
			cube.transform.scale = glm::vec3(1.0f) *  window.screenToWorldDistance(radius);

			//// set everything to the same rotation
			cube.transform.rotation = scene.objects[0].transform.rotation;

			//// rotate it a bit
			cube.transform.rotate(glm::radians(2.0f), glm::vec3(0, 1, 1));

			cube.visible = true;
		}
	}
	else
	{
		scene.hideAllObjects();
	}
}


void onUpdate(lwar::Window& window, float deltaTime)
{
	lwar::Scene& scene = window.getScene();
	cv::Mat camFrame;

	if (scene.camera.isOpened)
	{
		camFrame = scene.camera.retrieve();
		// mirror on y axis so its like a mirror
		cv::flip(camFrame, camFrame, 1);
	}

	cubesOnCircles(window, scene, camFrame);

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
	int width = 640;
	int height = 480;
	std::string windowName = "Augmented Reality Test";

	lwar::Window window = lwar::Window(width, height, windowName);
	window.onUpdate = onUpdate;
	
	lwar::Camera camera = lwar::Camera(0, width, height);
	camera.init();

	if (!camera.isOpened)
	{
		getchar();
		return -1;
	}

	window.getScene().camera = camera;

	// starts the main loop
	window.start();

	return 0;
}


