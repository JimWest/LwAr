// LwARTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../LwAR/Window.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/quaternion.hpp"
#include "gtx/quaternion.hpp"
#include "gtx/euler_angles.hpp"
#include <opencv2/opencv.hpp>

int width = 640;
int height = 480;
std::string windowName = "Augmented Reality Test";
int i = 0;

std::vector<cv::Vec3f> detectRedCircle(cv::Mat& image)
{
	std::vector<cv::Vec3f> circles;
	if (image.empty())
		return circles;

	cv::Mat orig_image = image.clone();

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
	cv::HoughCircles(red_hue_image, circles, CV_HOUGH_GRADIENT, 2, red_hue_image.rows / 4, 5, 50);

	return circles;

	//// Loop over all detected circles and outline them on the original image
	//if (circles.size() != 0)
	//{
	//	for (size_t current_circle = 0; current_circle < circles.size(); ++current_circle) {
	//		cv::Point center(std::round(circles[current_circle][0]), std::round(circles[current_circle][1]));
	//		int radius = std::round(circles[current_circle][2]);

	//		cv::circle(orig_image, center, radius, cv::Scalar(0, 255, 0), 5);
	//	}
	//}

	//return orig_image;
}

glm::vec3 get3dPoint(glm::vec2 point2d, int width,
	int height, glm::mat4 viewMatrix, glm::mat4 projectionMatrix) {

	double x = 2.0 * point2d.x / width - 1;
	double y = -2.0 * point2d.y / height + 1;
	glm::mat4 viewProjectionInverse = glm::inverse(projectionMatrix *
		viewMatrix);

	glm::vec4 point3D = glm::vec4(x, y, 0, 1);
	point3D = viewProjectionInverse * point3D;
	return  glm::vec3(point3D.x, point3D.y, 0);
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

	std::vector<cv::Vec3f> circles = detectRedCircle(camFrame);

	// Loop over all detected circles and outline them on the original image
	if (circles.size() != 0)
	{
		cv::Point center(std::round(circles[0][0]), std::round(circles[0][1]));
		int radius = std::round(circles[0][2]);

		glm::vec3 point = window.toScreenPoint(glm::vec2(center.x, center.y));
		point *= 3;

		if (scene.objects.size() <= 0)
		{
			lwar::Object3d cube = lwar::Object3d(lwar::Primitves::Cube);
			cube.material.texture = lwar::Material::ColorGradient();

			window.addObject(cube);
		}
		else {
			scene.objects[0].transform.translation = point;
			scene.objects[0].transform.scale = glm::vec3(0.2f, 0.2f, 0.2f);
			scene.objects[0].transform.rotation = glm::quat(glm::vec3(10, 1 * i / 100.0f, 1 * i / 100.0f));
		}
	}


	window.background.material.texture = camFrame;

	i++;

	int key = window.getLastKey();
	// escape == exit
	if (key == 256)
		window.stop();
}

int main()
{
	lwar::Renderer* renderer = new lwar::OpenGLRenderer(width, height, windowName);

	lwar::Window window = lwar::Window(renderer);
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

	// starts the main loop
	window.start();

	delete(renderer);

	return 0;
}

