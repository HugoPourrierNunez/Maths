
#ifdef _MSC_VER
#pragma comment(lib, "opengl32.lib")
#include <Windows.h>
#endif

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

#include "../common/EsgiShader.h"

EsgiShader g_BasicShader;

static const float g_Triangle[] = {
	-0.8f, .8f,
	0.f, -.8f,
	.8f, .8f
};

static const float g_Color[] = {
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f
};

static const float g_TriangleAttribs[] = {
	-0.8f, .8f,
	1.0f, 0.0f, 0.0f,
	0.f, -.8f,
	0.0f, 1.0f, 0.0f,
	.8f, .8f,
	0.0f, 0.0f, 1.0f
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
	
	auto color_location = glGetAttribLocation(program, "a_Color");
	auto position_location = glGetAttribLocation(program, "a_Position");

	glVertexAttribPointer(color_location, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), &g_TriangleAttribs[2]);
	glVertexAttribPointer(position_location, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), &g_TriangleAttribs[0]);

	//glVertexAttribPointer(position_location, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), g_Triangle);
	//glVertexAttribPointer(color_location, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), g_Color);

	glEnableVertexAttribArray(position_location);
	glEnableVertexAttribArray(color_location);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glDisableVertexAttribArray(position_location);
	glDisableVertexAttribArray(color_location);

	glUseProgram(0);

	glutSwapBuffers();
}

int main(int argc, const char* argv)
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

	glutDisplayFunc(animate);
	glutMainLoop();

	g_BasicShader.DestroyProgram();

	return 1;
}