#include "Camera.h"

namespace lwar
{
	Camera::Camera() :Camera(0, 640, 480, 30)
	{
	}

	Camera::Camera(int id, int width, int height, int fps)
	{
		this->id = id;
		this->width = width;
		this->height = height;
		this->fps = fps;
	}

	Camera::~Camera()
	{
		capture.release();
	}


	void Camera::init()
	{
		capture = cv::VideoCapture(id);
		//	capture.open(id);
		isOpened = capture.isOpened();

		capture.set(CV_CAP_PROP_FRAME_WIDTH, width);
		capture.set(CV_CAP_PROP_FRAME_HEIGHT, height);
		capture.set(CV_CAP_PROP_FPS, fps);
	}


	cv::Mat Camera::retrieve()
	{
		cv::Mat frame;
		//capture.retrieve(frame, CV_CAP_OPENNI_BGR_IMAGE);
		capture.read(frame);
		return frame;
	}
}