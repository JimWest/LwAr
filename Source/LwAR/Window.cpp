#include  "Window.h"

using namespace std;

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

	void Window::addText(Text& text)
	{
		scene.texts.push_back(text);
	}
	
	void Window::start()
	{

		// prepare the variables for the delta time calculation
		std::chrono::time_point<std::chrono::system_clock> start, end;
		std::chrono::duration<float> deltaTime;
		start = std::chrono::system_clock::now();

		while (running && !renderer->getShouldClose())
		{
			// calculate the delta time (usable for processor indipendent speed of animations etc.)
			end = start;
			start = std::chrono::system_clock::now();
			deltaTime = start - end;

			// optional callback to control the update
			if (onUpdate)
				onUpdate(*this, deltaTime.count());

			renderer->preDraw();

			// always render background first and ignore depth so it will be always behind all other objects
			if (background.visible)
				renderer->drawObject(background, projectionMatrix, viewMatrix, true);

			// render each Object out of the object list
			for (int i = 0; i < scene.objects.size(); i++)
			{
				if (scene.objects.at(i).visible)
					renderer->drawObject(scene.objects.at(i), projectionMatrix, viewMatrix, false);
			}

			// render each Text out of the object list
			for (int i = 0; i < scene.texts.size(); i++)
			{
				renderer->drawText(scene.texts.at(i));				
			}

			renderer->postDraw();
		}

	}

	void Window::stop()
	{
		running = false;
	}

	void Window::onRendererKeyInput(int key)
	{
		//std::cout << std::to_string(key) << std::endl;
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
		glm::mat4 viewProjectionMatrix = projectionMatrix * viewMatrix;
		//transform world to clipping coordinates
		glm::mat4 translate = glm::translate(viewProjectionMatrix, position);
		glm::vec4 transformedVector = translate * glm::vec4(1,1,1,1);
		
		int winX = (int)round(((transformedVector.x + 1) / 2.0) *
			width);
		//we calculate -point3D.getY() because the screen Y axis is
		//oriented top->down 
		int winY = (int)round(((1 - transformedVector.y) / 2.0) *
			height);
		return glm::vec2(winX, winY);
	}

	glm::vec3 Window::screenToWorldPoint(glm::vec2 position)
	{
		double x = 2.0 * position.x / width - 1;
		double y = -2.0 * position.y / height + 1;

		glm::mat4 viewProjectionInverse =
			glm::inverse(projectionMatrix *	viewMatrix);

		glm::vec4 point3D = glm::vec4(x, y, 0, 1);
		point3D = viewProjectionInverse * point3D;
		return glm::vec3(point3D.x, point3D.y, 0) * worldCameraDist;
	}

	float Window::screenToWorldDistance(float distance)
	{
		return distance / (height / 2.0f);
	}

	float Window::worldToScreenDistance(float radius)
	{
		return radius * (height / 2.0f);
	}


}
