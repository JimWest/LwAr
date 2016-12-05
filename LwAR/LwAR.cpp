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


LwAR::LwAR()
{
	_running = true;
}


LwAR::~LwAR()
{
}

void LwAR::AddObject(Object3d* object)
{
	_objects.insert(object);
}

// only returns if the application should be closed
void LwAR::Start()
{
	cv::Mat camFrame;
	int i = 0;
	while (_running)
	{
		if (_cam.isOpened)
		{
			camFrame = _cam.Retrieve();
			// mirror on y axis so its like a mirror
			cv::flip(camFrame, camFrame, 1);
		}

		_renderer.preDraw();

		//renderer.drawObject(&background, camFrame);
		//cube.transform.rotation = glm::quat(glm::vec3(10, 1 * i / 100.0f, 1 * i / 100.0f));
		//_renderer.drawObject(&cube, camFrame);

		_renderer.postDraw();

		i++;
	}

}
