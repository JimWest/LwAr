#pragma once

#include "opencv2/opencv.hpp"
#include "glm.hpp"




namespace lwar
{
	class Camera
	{
	public:
		Camera();
		Camera(int id, int width, int height);
		~Camera();

		void init();
		cv::Mat retrieve();

		bool isOpened;

	private:
		int id, width, height;

		cv::VideoCapture capture;
	};
}

