#pragma once

#include "opencv2/opencv.hpp"
#include "glm.hpp"




namespace lwar
{
	class Camera
	{
	public:
		Camera();
		Camera(int id, int width, int height, int fps);
		~Camera();

		void init();
		cv::Mat retrieve();

		bool isOpened;

	private:
		int id, width, height, fps;

		cv::VideoCapture capture;
	};
}

