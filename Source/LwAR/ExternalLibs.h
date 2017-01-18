#pragma once
// Loads all the needed libs depending on the configuration (Debug or Release)

#ifdef _DEBUG 
#pragma comment(lib, "opencv_world310d.lib")
#pragma comment(lib, "aruco201.libd")

#else
#pragma comment(lib, "opencv_world310.lib")
#pragma comment(lib, "aruco201.lib")
#endif

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glfw3.lib")
