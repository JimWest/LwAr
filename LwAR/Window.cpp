// LwAR.cpp : Defines the entry point for the console application.
//

#include  "Window.h"

using namespace std;

// TODO: Auto copy needed dlls into the output folder


namespace lwar
{

	Window::Window()
	{
		running = false;
	}

	Window::Window(int width, int height, std::string title, RenderType renderType)
	{
		running = true;

		switch (renderType)
		{
		case RenderType::OpenGL:
			renderer = new lwar::OpenGLRenderer(width, height, title, fov, zNear, zFar, worldCameraDist);
			break;
		}

		// bind is needed so we can call  member functions
		renderer->onKeyboardInput = std::bind(&Window::onRendererKeyInput, this, std::placeholders::_1);
		renderer->initObject(background);
		background.material.shaderType = ShaderType::Unlit;

		// calculate the needed scale of the background so it fits on the whole screen
		float aspectRatio = (height) ? float(width) / float(height) : float(height) / float(width);

		// size of the frostum at the distance of the background	
		float frustumHeight = worldCameraDist * tan(glm::radians(fov * 0.5f));
		float frustumWidth = frustumHeight * aspectRatio;

		// scale it
		background.transform.scale = glm::vec3(frustumWidth, frustumHeight, 1.0f);

		projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, zNear, zFar);
		viewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, worldCameraDist),
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f));

		renderer->light.transform.translation = glm::vec3(3, 3, 3);
	}


	Window::~Window()
	{
		delete(renderer);
	}

	void Window::addObject(Object3d& object)
	{
		renderer->initObject(object);
		scene.objects.push_back(object);
	}

	// only returns if the application should be closed
	void Window::start()
	{
		std::chrono::time_point<std::chrono::system_clock> start, end;
		std::chrono::duration<float> deltaTime;		
		start = std::chrono::system_clock::now();

		while (running)
		{
			// calculate the delta time (usable for processor indipendent speed of animations etc.)
			end = start;
			start = std::chrono::system_clock::now();			
			deltaTime = start - end;

			if (onUpdate)
				onUpdate(*this, deltaTime.count());

			renderer->preDraw();

			// always render background first and ignore depth so other objects cant intercept with it
			if (background.visible)
				renderer->drawObject(background, projectionMatrix, viewMatrix, true);


			//_renderer->drawText("Test", 0, 0, 16);
			renderer->drawText("!ABCDEFGHAJKLMOPQRSTUVWXYZ", 10, 500, 30);

			for (int i = 0; i < scene.objects.size(); i++)
			{
				if (scene.objects.at(i).visible)
					renderer->drawObject(scene.objects.at(i), projectionMatrix, viewMatrix, false);
			}

			renderer->postDraw();
			//Sleep(100);
		}

	}

	void Window::stop()
	{
		running = false;
	}

	void Window::onRendererKeyInput(int key)
	{
		std::cout << std::to_string(key) << std::endl;
		lastKey = key;
	}


	Renderer* Window::getRenderer() const
	{
		return renderer;
	}

	Scene& Window::getScene()
	{
		return scene;
	}

	int Window::getLastKey()
	{
		return lastKey;
	}

	cv::Mat Window::getBackground()
	{
		return background.material.texture;
	}

	void Window::setBackground(cv::Mat background)
	{
		this->background.material.texture = background;
	}

	glm::vec2 Window::worldToScreenPoint(glm::vec3 position)
	{
		return glm::vec2();
	}

	glm::vec3 Window::screenToWorldPoint(glm::vec2 position)
	{
		double x = 2.0 * position.x / width - 1;
		double y = -2.0 * position.y / height + 1;
		glm::mat4 viewProjectionInverse =
			glm::inverse(projectionMatrix *	viewMatrix);

		glm::vec4 point3D = glm::vec4(x, y, 0, 1);
		point3D = viewProjectionInverse * point3D;
		return  glm::vec3(point3D.x, point3D.y, 0) * worldCameraDist;
	}

	float Window::screenToWorldDistance(float radius)
	{
		return radius / (height / 2.0f);
	}

	float Window::worldToScreenDistance(float radius)
	{
		return radius * (height / 2.0f);
	}


}
