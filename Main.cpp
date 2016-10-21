
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

using namespace std;
#include "freeglut.h"

#include "CVecteur.h"
#include "../common/EsgiShader.h"

EsgiShader g_BasicShader;

static const float g_tab[] = {
	-0.7f, -.5f,
	1.0f,  0.0f,  0.0f,
	-.5f, .5f, 
	0.0f,  1.0f,  0.0f,
	.5f, -.5f,
	0.0f,  0.0f,  1.0f,
	.7f, .5f,
	1.0f,  0.0f,  0.0f
};

int width = 800;
int height = 600;

void animate()
{
	glViewport(0, 0, width, height);
	glClearColor(1.f, 1.f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);

	auto program = g_BasicShader.GetProgram();
	glUseProgram(program);

	auto color_position = glGetAttribLocation(program, "a_Color");
	//glVertexAttrib3f(program, .0f, 1.f, .0f);
	auto position_location = glGetAttribLocation(program, "a_Position");

	glVertexAttribPointer(color_position, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), &g_tab[2]);
	glEnableVertexAttribArray(color_position);

	glVertexAttribPointer(position_location, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), g_tab);
	glEnableVertexAttribArray(position_location);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDisableVertexAttribArray(position_location);
	glEnable(GL_CULL_FACE);
	glUseProgram(0);

	glutSwapBuffers();
}

int main(int argc, const char* argv)
{
	/*
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

	glutDisplayFunc(animate);
	glutMainLoop();

	g_BasicShader.DestroyProgram();*/

	Point p1, p2, p3, p4;
	p1.x = 0;
	p1.y = 0;

	p2.x = 1;
	p2.y = 1;

	p3.x = 0;
	p3.y = 1;

	p4.x = 1;
	p4.y = 0;



	CVecteur* myVecteur = new CVecteur();

	Point pResult = myVecteur->Intersection(p1, p2, p3, p4);

	cout << "x : " << pResult.x  << " y : " <<  pResult.y << endl;
	int i;
	cin >>  i;
	return 1;
}