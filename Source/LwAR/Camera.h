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

		// Returns the camera opening state.
		// Needs to be called before retrieving pictures.
		void init();

		// Returns the camera opening state.
		//
		// @return The current as Camera-Image cv::Mat
		cv::Mat retrieve();

		// Returns the camera opening state.
		//
		// @return true, if the camera is opened.
		bool isOpened;

	private:
		int id, width, height;

		cv::VideoCapture capture;
	};
}

