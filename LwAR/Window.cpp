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

	Window::Window(int width, int height, std::string title, RenderType renderType)
	{
		_running = true;

		switch (renderType)
		{
		case RenderType::OpenGL:
			_renderer = new lwar::OpenGLRenderer(width, height, title);
			break;
		}

		// bin is needed so we can call  member functions
		_renderer->onKeyboardInput = std::bind(&Window::onRendererKeyInput, this, std::placeholders::_1);
		_renderer->initObject(background);

		// calculate the needed scale of the background so it fits on the whole screen
		float aspectRatio = (height) ? float(width) / float(height) : float(height) / float(width);

		// size of the frostum at the distance of the background
		float distance = 3.0f;
		float frustumHeight = distance * tan(glm::radians(45.0f * 0.5f));
		float frustumWidth = frustumHeight * aspectRatio;

		background.transform.scale = glm::vec3(frustumWidth, frustumHeight, 1.0f);

		projectionMatrix = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 200.0f);
		viewMatrix = glm::mat3(1.0f);
	}


	Window::~Window()
	{
		delete(_renderer);
	}

	void Window::addObject(Object3d& object)
	{
		_renderer->initObject(object);
		_scene.objects.push_back(object);
	}

	// only returns if the application should be closed
	void Window::start()
	{
		while (_running)
		{
			if (onUpdate)
				onUpdate(*this);

			_renderer->preDraw();

			// always render background first and ignore depth so other objects cant intercept with it
			if (background.visible)
				_renderer->drawObject(background, true);


			//_renderer->drawText("Test", 0, 0, 16);
			_renderer->drawText("!ABCDEFGHAJKLMOPQRSTUVWXYZ", 10, 500, 30);

			for (int i = 0; i < _scene.objects.size(); i++)
			{
				if (_scene.objects.at(i).visible)
					_renderer->drawObject(_scene.objects.at(i));
			}

			_renderer->postDraw();
			//Sleep(100);
		}

	}

	void Window::stop()
	{
		_running = false;
	}

	void Window::onRendererKeyInput(int key)
	{
		std::cout << std::to_string(key) << std::endl;
		_lastKey = key;
	}


	Renderer* Window::getRenderer() const
	{
		return _renderer;
	}

	Scene& Window::getScene()
	{
		return _scene;
	}

	int Window::getLastKey()
	{
		return _lastKey;
	}

	glm::vec3 Window::toScreenPoint(glm::vec2 position)
	{
		double x = 2.0 * position.x / _width - 1;
		double y = -2.0 * position.y / _height + 1;
		glm::mat4 viewProjectionInverse =
			glm::inverse(projectionMatrix *	viewMatrix);

		glm::vec4 point3D = glm::vec4(x, y, 0, 1);
		point3D = viewProjectionInverse * point3D;
		return  glm::vec3(point3D.x, point3D.y, 0);
	}

	glm::vec2 Window::toScenePoint(glm::vec3 position)
	{
		return glm::vec2();
	}
}
