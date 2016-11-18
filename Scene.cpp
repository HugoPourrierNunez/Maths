#include "Scene.h"

using namespace maths;

Scene* Scene::currentInstance=nullptr;

void Scene::drawCallBack()
{
	Scene::currentInstance->mainLoop();
}

void Scene::lauchOpenGLLoop()
{
	glutMainLoop();
}

void Scene::initOpenGl(int argc, const char* argv)
{
	glutInit(&argc, (char **)argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(200, 100);
	glutInitWindowSize(width, height);
	glutCreateWindow("Triangle");

#ifndef NO_GLEW
	glewInit();
#endif

	g_BasicShader.LoadVertexShader("basic.vs");
	g_BasicShader.LoadFragmentShader("basic.fs");
	g_BasicShader.CreateProgram();

	glutDisplayFunc(Scene::drawCallBack);

	//glEnable(GL_CULL_FACE);

	input->init();
	
}

void Scene::mainLoop()
{
	glViewport(0, 0, width, height);
	glClearColor(1.f, 1.f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);

	auto program = g_BasicShader.GetProgram();
	glUseProgram(program);

	auto color_position = glGetAttribLocation(program, "a_Color");
	auto position_location = glGetAttribLocation(program, "a_Position");

	float position[] = {
		-0.7f, -.5f,
		-.5f, .5f,
		.5f, -.5f,
		.7f, .5f
	};

	float color[] = {
		1.0f,  0.0f,  0.0f,
		0.0f,  1.0f,  0.0f,
		0.0f,  0.0f,  1.0f,
		1.0f,  0.0f,  0.0f
	};



	switch (state)
	{
	case DRAW:
		glVertexAttribPointer(color_position, 3, GL_FLOAT, GL_FALSE, 0, color);
		glEnableVertexAttribArray(color_position);

		glVertexAttribPointer(position_location, 2, GL_FLOAT, GL_FALSE, 0, position);
		glEnableVertexAttribArray(position_location);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glDisableVertexAttribArray(position_location);
		glDisableVertexAttribArray(color_position);

		break;
	case ENTER_POINTS:

		break;
	}

	glUseProgram(0);

	glutSwapBuffers();
}

void Scene::changeState(State s)
{
	if (state == s)
		return;
	state = s;
	switch (state)
	{
	case ENTER_POINTS:
		polygons->push_back(*(new maths::Polygon()));
		break;
	case DRAW:

		break;
	}
}

State Scene::getState()
{
	return state;
}

void Scene::addPoint(maths::Point p)
{
	if(!polygons->empty())
	{
		polygons->back().addPoint(p);
	}
}




Scene::Scene(int w, int h)
{
	state = DRAW;
	height = h;
	width = w;
	Scene::currentInstance = this;
	input = new Input(this);
	polygons = new std::vector<maths::Polygon>();
}


Scene::~Scene()
{
	g_BasicShader.DestroyProgram();
}
