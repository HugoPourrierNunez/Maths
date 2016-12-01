#include "Scene.h"

using namespace maths;

Scene* Scene::currentInstance=nullptr;

void Scene::drawCallBack()
{
	Scene::currentInstance->mainLoop();
}

void Scene::cut()
{
	std::cout << "start cut"<< std::endl;

	if (polygons->size() < 2)
		return;

	maths::Polygon pol = polygons->at(1);
	maths::Polygon win = polygons->at(0);

	//maths::Polygon *newPol = new maths::Polygon();

	win.calculateNormals();
	int nbPoint = pol.getPoints()->size();
	int decallage = 0;
	for (int i=0; i < nbPoint+decallage; i++)
	{
		std::cout << "test point n�" << i<<std::endl;
		Point p1 = pol.getPoints()->at(i);
		std::cout << "x=" << p1.x << "		y=" << p1.y << std::endl;
		Point p2;
		if(i== nbPoint + decallage -1)
			p2= pol.getPoints()->at(0);
		else
			p2 = pol.getPoints()->at(i+1);

		int nbPointWin = win.getPoints()->size();

		for (int j=0; j < nbPointWin -1; j++)
		{
			Point p3 = win.getPoints()->at(j);
			Point p4;
			if (j == nbPointWin - 1)
				p4 = win.getPoints()->at(0);
			else
				p4 = win.getPoints()->at(j + 1);

			bool p1Visibility = Math::isPointVisible(p1, p3, win.getNormals()->at(j));
			if (!p1Visibility)
				pol.setVisibility(i, false);
			bool p2Visibility = Math::isPointVisible(p2, p3, win.getNormals()->at(j));
			if (p1Visibility != p2Visibility)
			{
				maths::Point intersection = CVecteur::Intersection(p1, p2, p3,p4);

				if (intersection.x != -1.0 && intersection.y != -1.0 /*&& (p1.x==intersection.x && p1.y==intersection.y)!=true*/)
				{
					pol.addPoint(intersection, i+1);
					std::cout << "ajout d'un point" << std::endl;
					std::cout << "x=" << intersection .x<<"		y="<< intersection.y<< std::endl;
					i++;
					decallage++;
				}
			}
		}
	}

	nbPoint = pol.getPoints()->size();
	std::cout << "nbpoint=" << nbPoint << std::endl;
	decallage = 0;
	for (int i = 0; i < nbPoint-decallage; i++)
	{
		std::cout << "test visibility point n�" << i << std::endl;
		if (!pol.isPointVisible(i))
		{
			pol.removePoint(i);
			i--;
			decallage++;

		}
	}
	nbPoint = pol.getPoints()->size();
	std::cout << "nbpoint=" << nbPoint << std::endl;
}

void Scene::cut2()
{
	/*if (polygons->size() < 2)
		return;

	maths::Polygon pol = polygons->at(1);
	maths::Polygon win = polygons->at(0);

	int n3 = win.getPoints()->size();
	int n1 = pol.getPoints()->size();
	int n2;
	maths::Point s, f, i;

	std::vector<maths::Point> *ps;

	for (int i = 0; i < n3-1 ;)
	{
		n2 = 0;
		ps = new std::vector<maths::Point>();
		for (int j = 0; j < n1;)
		{
			if(j==0)
				f=pol.getPoints()->at()
		}
	}*/
}

void Scene::flush()
{
	if (state != ENTER_POINTS)
	{
		while (!polygons->empty())
		{
			polygons->pop_back();
		}
		glutPostRedisplay();
	}
	
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
	glClearColor(1.f, 1.f, 1.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);

	auto program = g_BasicShader.GetProgram();
	glUseProgram(program);

	auto color_position = glGetAttribLocation(program, "a_Color");
	auto position_location = glGetAttribLocation(program, "a_Position");

	float position[] = {
		-0.7, -.5f,
		-.5f, .5f,
		.7f, .5f,
		.5f, -.5f
	};

	float color[] = {
		1.0f,  0.0f,  0.0f,
		0.0f,  1.0f,  0.0f,
		1.0f,  0.0f,  0.0f,
		0.0f,  0.0f,  1.0f
	};



	switch (state)
	{
	case DRAW:
		/*glVertexAttribPointer(color_position, 3, GL_FLOAT, GL_FALSE, 0, color);
		glEnableVertexAttribArray(color_position);*/
		for (int i = 0; i < polygons->size(); i++)
		{
			const maths::Point *points =  polygons->at(i).getPoints()->data();
			unsigned int size = polygons->at(i).getPoints()->size();

			 glVertexAttribPointer(position_location, 2, GL_FLOAT, GL_FALSE, 0, points);
			glEnableVertexAttribArray(position_location);

			glDrawArrays(GL_LINE_LOOP, 0, size);
			glDisableVertexAttribArray(position_location);
			glDisableVertexAttribArray(color_position);
		}

		

		break;
	case ENTER_POINTS:

		for (int i = 0; i < polygons->size()-1; i++)
		{
			const maths::Point *points = polygons->at(i).getPoints()->data();
			unsigned int size = polygons->at(i).getPoints()->size();

			glVertexAttribPointer(position_location, 2, GL_FLOAT, GL_FALSE, 0, points);
			glEnableVertexAttribArray(position_location);

			glDrawArrays(GL_LINE_LOOP, 0, size);
			glDisableVertexAttribArray(position_location);
			glDisableVertexAttribArray(color_position);
		}
		
		const maths::Point *points = polygons->back().getPoints()->data();
		unsigned int size = polygons->back().getPoints()->size();
		
		maths::Point tmpPoints[4];
		for (int i = 0; i < size; i++)
		{
			
			tmpPoints[0].x = points[i].x - radiusPoint.x;
			tmpPoints[0].y = points[i].y - radiusPoint.y;
			tmpPoints[1].x = points[i].x - radiusPoint.x;
			tmpPoints[1].y = points[i].y + radiusPoint.y;
			tmpPoints[2].x = points[i].x + radiusPoint.x;
			tmpPoints[2].y = points[i].y - radiusPoint.y;
			tmpPoints[3].x = points[i].x + radiusPoint.x;
			tmpPoints[3].y = points[i].y + radiusPoint.y;

			std::cout << "draw point " << i << std::endl;

			std::cout << "x= " << tmpPoints[1].x << "	y="<< tmpPoints[1].y  << std::endl;

			glVertexAttribPointer(position_location, 2, GL_FLOAT, GL_FALSE, 0, tmpPoints);
			glEnableVertexAttribArray(position_location);

			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			glDisableVertexAttribArray(position_location);
			glDisableVertexAttribArray(color_position);

			/*Color c;
			c.r = 1;
			glUseProgram(0);
			drawChar('1', points[i], c);
			glUseProgram(program);*/
		}

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

		std::cout << "point added x=" << p.x << " y=" << p.y << std::endl;
		p.x -= width / 2;
		p.x /= width/2;
		p.y = height - p.y;
		p.y -= height / 2;
		p.y /= height/2;
		std::cout << "point normalized x=" << p.x << " y=" << p.y << std::endl;

		polygons->back().addPoint(p);
	}
}

void Scene::drawChar(const char c, const maths::Point position, const maths::Color color)
{
	//gluOrtho2D(0.0, width, 0.0, height);
	glColor3f(color.r, color.g, color.b);
	maths::Point p;
	p.x = (position.x*(width / 2)) + (width / 2);
	p.y= height -(position.y*(height/ 2)) + (height/ 2);
	
	glRasterPos2i(20, 20);
	void * font = GLUT_BITMAP_9_BY_15;
	glutBitmapCharacter(font, c);
}




Scene::Scene(int w, int h)
{
	state = DRAW;
	height = h;
	width = w;
	Scene::currentInstance = this;
	input = new Input(this);
	polygons = new std::vector<maths::Polygon>();

	radiusPoint.x = 10.0f/ width;
	radiusPoint.y = 10.0f /height;
}


Scene::~Scene()
{
	g_BasicShader.DestroyProgram();
}
