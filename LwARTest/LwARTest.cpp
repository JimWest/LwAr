// LwARTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../LwAR/Window.h"
#include "glm.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <direct.h>

#include <gtc/quaternion.hpp>
#include <gtx/quaternion.hpp>
#include <gtx/euler_angles.hpp>

int width = 640;
int height = 480;
std::string windowName = "Augmented Reality Test";

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
			point *= 3;

			if (scene.objects.size() < circles.size())
			{
				lwar::Object3d& cube = lwar::Object3d(lwar::Primitves::Cube);
				cube.material.texture = lwar::Material::ColorGradient();

				window.addObject(cube);
			}

			lwar::Object3d& cube = scene.objects[i];

			// move the object to the position of the circle
			cube.transform.translation = point;

			// set the scale to the radius so it fits the whole circle
			cube.transform.scale = glm::vec3(1.0f) *  window.screenToWorldRadius(radius);

			// set everything to the same rotation
			cube.transform.rotation = scene.objects[0].transform.rotation;

			// rotate it a bit
			cube.transform.rotate(glm::radians(2.0f), glm::vec3(0, 1, 1));

			cube.visible = true;
		}
	}
	else
	{
		scene.hideAllObjects();
	}
}


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


void getFace(lwar::Window& window, lwar::Scene& scene, cv::Mat& camFrame)
{

	float faceScale = 1.8f;
	int faceMinNeightbors = 5;
	cv::Size size = cv::Size(50, 50);


	cv::CascadeClassifier faceCascade = cv::CascadeClassifier("D:\\Eigene Dateien\\HSKA\\1. Semester\\Projektarbeit\\LwAR\\ExternalLibs\\opencv2\\build\\share\\OpenCV\\haarcascades\\haarcascade_frontalcatface.xml");

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
		0, size);


	for (int i = 0; i < objects.size(); ++i)
	{

		cv::Point faceCenter = cv::Point2d((int)(objects[i].x + objects[i].width * 0.5), (int)(objects[i].x + objects[i].width * 0.5));
		cv::Size faceAxes = cv::Size((int)(objects[i].width * 0.4), (int)(objects[i].height * 0.5));
		cv::ellipse(camFrame, faceCenter, faceAxes, 0, 0, 360, cv::Scalar(255, 0, 255), 4);

	}
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


	cubesOnCircles(window, scene, camFrame);
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

