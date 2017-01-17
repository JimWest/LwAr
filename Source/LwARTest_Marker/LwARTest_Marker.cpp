// LwARTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../LwAR/Window.h"
#include "glm.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <aruco/aruco.h>
#include <aruco/cvdrawingutils.h>


int width = 640;
int height = 480;
std::string windowName = "Augmented Reality Test";

int thresh = 100;
int max_thresh = 255;
cv::RNG rng(12345);

aruco::MarkerDetector MDetector;
std::vector<aruco::Marker> Markers;
lwar::Camera camera;

void onUpdate(lwar::Window& window, float deltaTime);

int main()
{
	lwar::Window window = lwar::Window(width, height, windowName);
	window.onUpdate = onUpdate;

	camera = lwar::Camera(0, width, height);
	camera.init();

	if (!camera.isOpened)
	{
		return -1;
	}

	// starts the main loop
	window.start();

	return 0;
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

	cv::Mat gray;
	cv::cvtColor(camFrame, gray, cv::COLOR_RGB2GRAY);
	cv::equalizeHist(gray, gray);
	//cv::Mat canny_output;
	//std::vector<std::vector<cv::Point> > contours;
	//std::vector<cv::Vec4i> hierarchy;

	//// Detect edges using canny
	//cv::Canny(gray, canny_output, thresh, thresh * 2, 3);
	//// Find contours
	//findContours(canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

	//// Draw contours
	//cv::Mat drawing = cv::Mat::zeros(canny_output.size(), CV_8UC3);
	//for (int i = 0; i< contours.size(); i++)
	//{
	//	cv::Scalar color = cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
	//	drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, cv::Point());
	//}


	MDetector.detect(gray, Markers);
	//for each marker, draw info and its boundaries in the image

	if (Markers.size() > 0)
	{

		for (unsigned int i = 0; i < Markers.size(); i++) {
			//cout << Markers[i] << endl;
			Markers[i].draw(camFrame, cv::Scalar(0, 0, 255), 2);

			cv::Point center = Markers[i].getCenter();
			float perimter = Markers[i].getPerimeter();


			double position[3];
			double rotation[4];

			Markers[i].OgreGetPoseParameters(position, rotation);


			glm::vec3 point = window.screenToWorldPoint(glm::vec2(center.x, center.y));

			if (scene.objects.size() < Markers.size())
			{
				lwar::Object3d cube = lwar::Object3d(lwar::Primitves::Cube);
				cube.material.texture = lwar::Material::ColorGradient();
				//cube.transform.scale = glm::vec3(0.1f, 0.1f, 0.1f);
				window.addObject(cube);
			}

			lwar::Object3d& cube = scene.objects[i];

			// move the object to the position of the circle
			cube.transform.translation = point;

			// set the scale to the radius so it fits the whole circle
			cube.transform.scale = glm::vec3(1.0f) *  window.screenToWorldDistance(perimter / 8.0f);

			//// set everything to the same rotation
			cube.transform.rotation = scene.objects[0].transform.rotation;

			//// rotate it a bit
			//cube.transform.rotate(glm::radians(2.0f), glm::vec3(0, 1, 1));
			cube.transform.rotation = glm::quat(rotation[0], rotation[1], rotation[2], rotation[3]);

			cube.visible = true;
		}
	}
	else
	{
		scene.hideAllObjects();
	}

	// set the background of the window to the current camera image
	window.setBackground(camFrame);

	// check for keys
	int key = window.getLastKey();
	// escape == exit
	if (key == 256)
		window.stop();
}
