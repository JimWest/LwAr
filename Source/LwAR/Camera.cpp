#include "Camera.h"

namespace lwar
{
	Camera::Camera() :Camera(0, 640, 480)
	{
	}

	Camera::Camera(int id, int width, int height)
	{
		this->id = id;
		this->width = width;
		this->height = height;
	}

	Camera::~Camera()
	{
		capture.release();
	}


	void Camera::init()
	{
		std::cout << "Opening Webcam device ..." << std::endl;
		capture = cv::VideoCapture(id);
		//	capture.open(id);
		isOpened = capture.isOpened();

		if (isOpened)
		{
			capture.set(CV_CAP_PROP_FRAME_WIDTH, width);
			capture.set(CV_CAP_PROP_FRAME_HEIGHT, height);
		}
		else
		{
			std::cout << "Opening Webcam device ..." << std::endl;
		}
	}


	cv::Mat Camera::retrieve()
	{
		cv::Mat frame;
		capture.read(frame);
		return frame;
	}
}