#pragma once

#include "stdafx.h"
#include <stdio.h>  
#include <stdlib.h>  
#include <Windows.h>
#include <iostream>
#include <functional>
#include <set>

#include "OpenGLRenderer.h"
#include "Camera.h"
#include "Object3d.h"

typedef std::set<Object3d *> ObjectSet;

class LwAR
{

public:

	LwAR();
	~LwAR();
	void AddObject(Object3d * object);
	void Start();
	std::function<void()> onUpdate;

private:

	int _width = 640;
	int _height = 480;
	bool _running;
	Renderer _renderer;
	Camera _cam;
	ObjectSet _objects;
};
