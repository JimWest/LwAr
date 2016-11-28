#pragma once

#include "opencv2/opencv.hpp"

class Camera
{
public:
	Camera();
	Camera(int id, int width, int height, int fps);
	~Camera();

	cv::Mat Retrieve();

	bool isOpened;

private:
	int width, height, fps;

	cv::VideoCapture capture;
};

