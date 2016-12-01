#ifndef  DEF_SCENE
#define DEF_SCENE

#ifdef _MSC_VER
#pragma comment(lib, "opengl32.lib")
#include <Windows.h>
#endif
#include <iostream>
#ifdef NO_GLEW
#include <gl/GL.h>
#include "glext.h"
#else
#include "glew.h"
#ifdef _MSC_VER
#pragma comment(lib, "glew32.lib")
#endif
#endif // !NO_GLEW

#include "freeglut.h"

#include "CVecteur.h"
#include "../common/EsgiShader.h"
#include "Input.h"

#include "Polygon.h"
#include "Math.h"

class Scene
{
	static Scene* currentInstance;
	maths::Point radiusPoint;
	static void drawCallBack();
	State state;
	Input *input;
	EsgiShader g_BasicShader;
	int width;
	int height;
	std::vector<maths::Polygon> *polygons;

	void drawChar(const char c, const maths::Point position, const maths::Color color);


public:
	void cut();
	void cut2();
	void flush();
	void lauchOpenGLLoop();
	void initOpenGl(int argc, const char* argv);
	void mainLoop();
	void changeState(State state);
	State getState();
	void addPoint(maths::Point p);
	Scene(int w, int h);
	~Scene();
};

#endif // ! SCENE






