#include "Scene.h"

using namespace maths;

Scene* Scene::currentInstance=nullptr;
State state;
std::vector<maths::Polygon> *polygons;
int windowId;
int value;
int option;
int mainMenu;
void menu(int num);
bool isInPolygon = true;

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

	std::vector<int> *indexAddedPoints = new std::vector<int>();
	std::vector<int> *addedPointWinPointIndex = new std::vector<int>();

	win.calculateNormals();
	int nbPoint = pol.getPoints()->size();
	int decallage = 0;
	//variables utilisées pour trouver l'intersection la plus proche
	for (int i=0; i < nbPoint+decallage; i++)
	{
		std::cout << std::endl << "test point n°" << i<<std::endl;
		Point p1 = pol.getPoints()->at(i);
		std::cout << "x=" << p1.x << "		y=" << p1.y << std::endl;
		Point p2;
		if(i== nbPoint + decallage -1)
			p2= pol.getPoints()->at(0);
		else
			p2 = pol.getPoints()->at(i+1);

		int nbPointWin = win.getPoints()->size();

		//variable necessaire à ne garder que l'intersection la plus proche
		float minNorme = -1;
		Point minIntersection;
		int minWinIndexPoint;

		for (int j=0; j < nbPointWin ; j++)
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

				maths::Point intersection = Math::getIntersection2(p1, p2, p3,p4);
				
				if (intersection.x != -1 && intersection.y != -1 && (p1.x==intersection.x && p1.y==intersection.y)==false)
				{
					if (minNorme == -1)
					{
						minNorme = (intersection.x - p1.x)*(intersection.x - p1.x) + (intersection.y - p1.y)*(intersection.y - p1.y);
						minIntersection = intersection;
						minWinIndexPoint = j;
					}
					else
					{
						float norme = (intersection.x - p1.x)*(intersection.x - p1.x) + (intersection.y - p1.y)*(intersection.y - p1.y);
						if (norme < minNorme)
						{
							minNorme = norme;
							minIntersection = intersection;
							minWinIndexPoint = j;
						}
					}
				}
		}

		if (minNorme != -1)
		{
			pol.addPoint(minIntersection, i + 1);
			indexAddedPoints->push_back(i + 1);
			std::cout << "ajout index added point = " << i + 1<< std::endl;
			addedPointWinPointIndex->push_back(minWinIndexPoint);
			std::cout << "ajout d'un point" << std::endl;
			std::cout << "x=" << minIntersection.x << "		y=" << minIntersection.y << std::endl;
			//i++;
			decallage++;
		}

	}

	int nbPointWin = win.getPoints()->size();
	nbPoint = pol.getPoints()->size();
	
	//block pour ajouter les points manquants
	int nbAddedPoint = indexAddedPoints->size();
	decallage = 0;
	for (int i = 0; i < nbAddedPoint; i++)
	{
		int index;
		int index1 = indexAddedPoints->at(i);
		index = index1 + 1;
		if (index == nbPoint + decallage)
			index = 0;
		int index2;
		if (i == nbAddedPoint - 1)
			index2 = indexAddedPoints->at(0);
		else
			index2 = indexAddedPoints->at(i + 1);
		//sort si aucun point entre les 2 points ajouté
		if (index == index2)
			continue;

		bool findVisiblePoint = false;
		while (index != index2 && !findVisiblePoint)
		{
			if (pol.isPointVisible(index + decallage))
			{
				findVisiblePoint = true;
			}
			index++;
			if (index == nbPoint + decallage)
				index = 0;
		}
		if (!findVisiblePoint)
		{
			//si il y a des points entre les 2 points ajoutés mais aucun visible il faut ajouter les points de la fenetre
			int indexWin1 = addedPointWinPointIndex->at(i) + 1;
			if (indexWin1 == nbPointWin)
				indexWin1 = 0;
			int indexWin2;
			if (i == nbAddedPoint - 1)
				indexWin2 = addedPointWinPointIndex->at(0);
			else
				indexWin2 = addedPointWinPointIndex->at(i + 1);
			indexWin2++;
			if (indexWin2 == nbPointWin)
				indexWin2 = 0;

			while (indexWin1 != indexWin2)
			{
				pol.addPoint(win.getPoints()->at(indexWin1), index1 +decallage+ 1);
				decallage++;
				indexWin1++;
				if (indexWin1 == nbPointWin)
					indexWin1 = 0;
			}
		}

	}
	//enlève les points qui ne sont pas visible
	nbPoint = pol.getPoints()->size();
	std::cout << "nbpoint=" << nbPoint << std::endl; 
	decallage = 0;
	for (int i = 0; i < nbPoint-decallage; i++)
	{
		if (!pol.isPointVisible(i))
		{
			std::cout << "hidden" << std::endl; 
			pol.removePoint(i);
			i--;
			decallage++;
		}
		else
		{
			std::cout << "visible" << std::endl;
		}
	}
	nbPoint = pol.getPoints()->size();
	std::cout << "nbpoint=" << nbPoint << std::endl;
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
/*
void Scene::menuCallBack(int num)
{
	Scene::currentInstance->menu(num);
}*/

void Scene::initOpenGl(int argc, const char* argv)
{
	glutInit(&argc, (char **)argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(200, 100);
	glutInitWindowSize(width, height);
	windowId = glutCreateWindow("Math");

	createMenu();

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

void Scene::createMenu()
{
	option = glutCreateMenu(menu);

	glutAddMenuEntry("Draw points    A", 1);
	glutAddMenuEntry("Draw polygon   Z", 2);
	glutAddMenuEntry("Cut            C", 3);
	glutAddMenuEntry("Fill           F", 4);

	
	mainMenu = glutCreateMenu(menu);

	glutAddSubMenu("Options", option);
	glutAddMenuEntry("Exit", 0);
	
	if (isInPolygon)
	{
		glutAddMenuEntry("Coloring polygon", 5);
	}

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// On traite ici le choix de l'utilisateur dans le menu contextuel
void menu(int num) {
	switch (num)
	{
	case 0:
		glutDestroyWindow(windowId);
		exit(0);
		break;
	case 1:
		state = ENTER_POINTS;
		polygons->push_back(*(new maths::Polygon()));
		break;
	case 2:
		state = DRAW;
		break;
	case 3:
		break;
	case 4:
		break;
	case 5:
		break;
	default:
		break;
	}

	glutPostRedisplay();
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

	switch (state)
	{
	case DRAW:
		/*glVertexAttribPointer(color_position, 3, GL_FLOAT, GL_FALSE, 0, color);
		glEnableVertexAttribArray(color_position);*/
		if (!polygons->empty())
		{
			for (int i = drawWindow ? 0 : 1; i < polygons->size(); i++)
			{
				const maths::Point *points = polygons->at(i).getPoints()->data();
				unsigned int size = polygons->at(i).getPoints()->size();

				glVertexAttribPointer(position_location, 2, GL_FLOAT, GL_FALSE, 0, points);
				glEnableVertexAttribArray(position_location);

				glDrawArrays(GL_LINE_LOOP, 0, size);
				glDisableVertexAttribArray(position_location);
				glDisableVertexAttribArray(color_position);
			}
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

			//std::cout << "draw point " << i << std::endl;

			//std::cout << "x= " << tmpPoints[1].x << "	y="<< tmpPoints[1].y  << std::endl;

			glVertexAttribPointer(position_location, 2, GL_FLOAT, GL_FALSE, 0, tmpPoints);
			glEnableVertexAttribArray(position_location);

			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			glDisableVertexAttribArray(position_location);
			glDisableVertexAttribArray(color_position);
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

		//std::cout << "point added x=" << p.x << " y=" << p.y << std::endl;
		p.x -= width / 2;
		p.x /= width/2;
		p.y = height - p.y;
		p.y -= height / 2;
		p.y /= height/2;
		p.x = Math::round(p.x);
		p.y = Math::round(p.y);
		//std::cout << "point normalized x=" << p.x << " y=" << p.y << std::endl;

		polygons->back().addPoint(p);
	}
}

void Scene::setDrawWindow()
{
	drawWindow = !drawWindow;
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
	value = 0;
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
