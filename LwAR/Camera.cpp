#include "Camera.h"



Camera::Camera() :Camera(0, 640, 480, 30)
{
}

Camera::Camera(int id, int width, int height, int fps)
{
	capture = cv::VideoCapture(id);
	//	capture.open(id);
	isOpened = capture.isOpened();

	this->width = width;
	this->height = height;
	this->fps = fps;

	capture.set(CV_CAP_PROP_FRAME_WIDTH, width);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT, height);
	capture.set(CV_CAP_PROP_FPS, fps);

}


Camera::~Camera()
{
	capture.release();
}


cv::Mat Camera::Retrieve()
{	
	cv::Mat frame;
	//capture.retrieve(frame, CV_CAP_OPENNI_BGR_IMAGE);
	capture.read(frame);
	return frame;
}
