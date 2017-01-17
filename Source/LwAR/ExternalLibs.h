#pragma once
// Loads all the needed libs depending on the configuration (Debug or Release)

#ifdef _DEBUG 
/*
#pragma comment(lib, "opencv_calib3d2413d.lib")
#pragma comment(lib, "opencv_core2413d.lib")
#pragma comment(lib, "opencv_highgui2413d.lib")
#pragma comment(lib, "opencv_imgproc2413d.lib")
#pragma comment(lib, "opencv_objdetect2413d.lib")*/
#pragma comment(lib, "opencv_world310d.lib")


#else
/*
#pragma comment(lib, "opencv_calib3d2413.lib")
#pragma comment(lib, "opencv_core2413.lib")
#pragma comment(lib, "opencv_highgui2413.lib")
#pragma comment(lib, "opencv_imgproc2413.lib")
#pragma comment(lib, "opencv_objdetect2413.lib")*/
#pragma comment(lib, "opencv_world310d.lib")
#endif

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glfw3.lib")
