// LwAR.cpp : Defines the entry point for the console application.
//

#include  "LwAR.h"

using namespace std;

// TODO: Auto copy needed dlls into the output folder


cv::Mat ColorGradient()
{
	int taille = 500;
	cv::Mat image(taille, taille, CV_8UC3);
	for (int y = 0; y < taille; y++) {
		cv::Vec3b val;
		val[0] = 0; val[1] = (y * 255) / taille; val[2] = (taille - y) * 255 / taille;
		for (int x = 0; x < taille; x++)
			image.at<cv::Vec3b>(y, x) = val;
	}
	return image;
}



LwAR::LwAR(Renderer* renderer)
{
	_running = true;
	_renderer = renderer;
	gradientTexture = ColorGradient();
	//imwrite("test.png", gradientTexture);
}


LwAR::~LwAR()
{
}

void LwAR::AddObject(Object3d& object)
{
	Object3d* objectPtr = &object;
	_objects.push_back(objectPtr);
	_renderer->initObject(object);
}

// only returns if the application should be closed
void LwAR::Start()
{
	while (_running)
	{
		if (onUpdate)
			onUpdate();

		_renderer->preDraw();

		for (auto object = _objects.begin(); object != _objects.end(); object++)
		{
			_renderer->drawObject(**object);
		}

		_renderer->postDraw();
		//Sleep(100);
	}

}
