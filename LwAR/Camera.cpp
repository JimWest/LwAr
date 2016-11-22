#include "Camera.h"



Camera::Camera():Camera(0, 640, 480, 30)
{	
}

Camera::Camera(int id, int width, int height, int fps)
{	
	capture.open(0);
	isOpened = capture.isOpened();
}


Camera::~Camera()
{
	capture.release();
}


cv::Mat Camera::Retrieve()
{
	cv::Mat frame;
	capture.retrieve(frame, CV_CAP_OPENNI_BGR_IMAGE);
	return frame;
}
