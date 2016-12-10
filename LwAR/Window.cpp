// LwAR.cpp : Defines the entry point for the console application.
//

#include  "Window.h"

using namespace std;

// TODO: Auto copy needed dlls into the output folder


namespace lwar
{

	Window::Window()
	{
		_running = false;
	}

	Window::Window(Renderer* renderer)
	{
		_running = true;
		_renderer = renderer;
		_renderer->initObject(background);

		// calculate the needed scale of the background so it fits on the whole screen
		float windowHeight = _renderer->getWindowHeight();
		float windowWidth = _renderer->getWindowWidth();
		float aspectRatio = (windowHeight) ? float(windowWidth) / float(windowHeight) : float(windowHeight) / float(windowWidth);

		// size of the frostum at the distance of the background
		float distance = 3.0f;
		float frustumHeight = distance * tan(glm::radians(45.0f * 0.5f));
		float frustumWidth = frustumHeight * aspectRatio;

		background.transform.scale = glm::vec3(frustumWidth, frustumHeight, 1.0f);
	}


	Window::~Window()
	{
	}

	void Window::AddObject(Object3d& object)
	{
		Object3d* objectPtr = &object;
		_objects.push_back(objectPtr);
		_renderer->initObject(object);
	}

	// only returns if the application should be closed
	void Window::Start()
	{
		while (_running)
		{
			if (onUpdate)
				onUpdate(*this);

			_renderer->preDraw();

			// always render background first and ignore depth so other objects cant intercept with it
			if (background.visible)
				_renderer->drawObject(background, true);

			for (auto object = _objects.begin(); object != _objects.end(); object++)
			{
				Object3d currentObject = **object;
				if (currentObject.visible)
					_renderer->drawObject(currentObject);
			}

			_renderer->postDraw();
			//Sleep(100);
		}

	}

	Renderer* Window::GetRenderer()
	{
		return _renderer;
	}


	Camera& Window::GetCamera()
	{
		return _camera;
	}
}
