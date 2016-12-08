// LwARTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../LwAR/LwAR.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/quaternion.hpp"
#include "gtx/quaternion.hpp"
#include "gtx/euler_angles.hpp"
#include <opencv2/opencv.hpp>

int width = 640;
int height = 480;
std::string windowName = "Augmented Reality Test";

Renderer* renderer;
LwAR lwar;
Object3d cube;
Camera cam;
cv::Mat camFrame;

int i = 0;


std::vector<cv::Vec3f> detectRedCircle(cv::Mat& image)
{
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
	std::vector<cv::Vec3f> circles;
	cv::HoughCircles(red_hue_image, circles, CV_HOUGH_GRADIENT, 2, red_hue_image.rows / 4, 200, 100);

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


// modell matrix
//glm::mat4 identyMatrix = glm::mat4(1.0f);
//glm::mat4 translationMatrix = glm::translate(identyMatrix, object.transform.translation);
//glm::mat4 scaleMatrix = glm::scale(identyMatrix, object.transform.scale);
//glm::mat4 rotationMatrix = glm::toMat4(object.transform.rotation);
//glm::mat4 modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;



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



void onUpdate()
{
	if (cam.isOpened)
	{
		camFrame = cam.Retrieve();
		// mirror on y axis so its like a mirror
		cv::flip(camFrame, camFrame, 1);
	}

	std::vector<cv::Vec3f> circles = detectRedCircle(camFrame);

	float windowHeight = renderer->getWindowHeight();
	float windowWidth = renderer->getWindowWidth();
	float aspectRatio = (windowHeight) ? float(windowWidth) / float(windowHeight) : float(windowHeight) / float(windowWidth);

	// size of the frostum at the distance of the background
	float distance = 3.0f;
	float frustumHeight = distance * tan(glm::radians(45.0f * 0.5f));
	float frustumWidth = frustumHeight * aspectRatio;

	glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 200.0f);
	glm::mat4 viewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));

	// Loop over all detected circles and outline them on the original image
	if (circles.size() != 0)
	{
		cv::Point center(std::round(circles[0][0]), std::round(circles[0][1]));
		int radius = std::round(circles[0][2]);
			
		glm::vec3 point = get3dPoint(glm::vec2(center.x, center.y), 640, 480, viewMatrix, projectionMatrix);
		point *= 3;
		cube.transform.translation = point;		
	}


	lwar.background.material.texture = camFrame;
	//cube.transform.translation = glm::vec3(0, 1 * i / 100.0f, 0);
	cube.transform.rotation = glm::quat(glm::vec3(10, 1 * i / 100.0f, 1 * i / 100.0f));
	i++;
}

int main()
{
	renderer = new OpenGLRenderer(width, height, windowName);

	lwar = LwAR(renderer);
	lwar.onUpdate = onUpdate;

	std::cout << "Opening Webcam device ..." << std::endl;

	cam = Camera(0, width, height, 60);
	cam.init();
	if (!cam.isOpened)
	{
		std::cout << "Error openeing camera" << std::endl;
		//return -1;
	}

	cube = Object3d(Primitves::Cube);
	cube.transform.translation = glm::vec3(0.0f, 0.0f, 0.0f);
	cube.transform.scale = glm::vec3(0.2f, 0.2f, 0.2f);
	cube.transform.rotation = glm::quat(glm::vec3(10, 10, 10));

	cube.material.texture = Material::ColorGradient();

	lwar.AddObject(cube);

	// starts the main loop
	lwar.Start();

	delete(renderer);

	return 0;
}

