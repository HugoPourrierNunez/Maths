#include "Scene.h"

using namespace maths;

Scene* Scene::currentInstance=nullptr;
State state;
std::vector<maths::Polygon> *polygons;
std::vector<maths::Point>* allIntersection = new std::vector<maths::Point>();
int windowId;
int value;
int option;
int mainMenu;
void menu(int num);
bool isInPolygon = true;
std::vector<maths::Polygon>* stackPolygonClicked = new std::vector<maths::Polygon>();

int colors[9] = {
	255, 0, 0,
	0, 255, 0,
	0, 0, 255
};

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
	//variables utilis�es pour trouver l'intersection la plus proche
	for (int i=0; i < nbPoint+decallage; i++)
	{
		std::cout << std::endl << "test point n�" << i<<std::endl;
		Point p1 = pol.getPoints()->at(i);
		std::cout << "x=" << p1.x << "		y=" << p1.y << std::endl;
		Point p2;
		if(i== nbPoint + decallage -1)
			p2= pol.getPoints()->at(0);
		else
			p2 = pol.getPoints()->at(i+1);

		int nbPointWin = win.getPoints()->size();

		//variable necessaire � ne garder que l'intersection la plus proche
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
		//sort si aucun point entre les 2 points ajout�
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
			//si il y a des points entre les 2 points ajout�s mais aucun visible il faut ajouter les points de la fenetre
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
	//enl�ve les points qui ne sont pas visible
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
		/*
		while (!allIntersection->empty())
		{
			allIntersection->pop_back();
		}
		*/
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

	// ATTENDS JE SUIS AU TEL AVEC UN COLLEGUE
	mainMenu = glutCreateMenu(menu);

	glutAddMenuEntry("Exit", 0);
	glutAddMenuEntry("Draw points    A", 1);
	glutAddMenuEntry("Draw polygon   Z", 2);
	glutAddMenuEntry("Cut            C", 3);
	glutAddMenuEntry("Fill           F", 4);
	/*
	if (stackPolygonClicked->size() != 0)
	{
	glutAddMenuEntry("Fill           F", 4);
	}
	*/
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
		state = FILL;
		break;
	case 5:
		state = COLOR;
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

	case FILL:
	{
		if (allIntersection->size() == 0)
		{
			allIntersection = LCARemplissage(polygons->at(0));
		}

		unsigned int nbIntersection = allIntersection->size();

		if (nbIntersection != 0)
		{
			for (int i = 0; i<nbIntersection; i += 2)
			{
				glVertexAttribPointer(position_location, 2, GL_FLOAT, GL_FALSE, 0, &allIntersection->at(i));
				glEnableVertexAttribArray(position_location);

				glDrawArrays(GL_LINES, 0, 2);
				glDisableVertexAttribArray(position_location);
				glDisableVertexAttribArray(color_position);
			}

		}
		
		for (int i = 0; i < polygons->size(); i++)
		{
			const maths::Point *points = polygons->at(i).getPoints()->data();
			unsigned int size = polygons->at(i).getPoints()->size();


			glVertexAttribPointer(position_location, 2, GL_FLOAT, GL_FALSE, 0, points);
			glVertexAttribPointer(color_position, 3, GL_INT, GL_FALSE, 0, colors);
			glEnableVertexAttribArray(position_location);
			glEnableVertexAttribArray(color_position);
			glDrawArrays(GL_LINE_LOOP, 0, size);
			glDisableVertexAttribArray(position_location);
			glDisableVertexAttribArray(color_position);
		}
		break;
	}
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
	case FILL:
		//LCARemplissage(polygons->at(0));
		break;
	default:
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

// Param�tre : polygon que l'on veut colorier
// Retour : tableau de points (trait�s comme des segments) qui doivent �tre colori� dans la mainLoop
// But :  D�tecter quelles zones sont � colorier
std::vector<maths::Point>* Scene::LCARemplissage(maths::Polygon polygon)
{
	std::vector<maths::Point>* allPointToDraw = new std::vector<maths::Point>();

	for(int y = 1; y < height; y++)
	{
		int bitParite = 0;

		// On cr�� notre ligne de balayage
		std::vector<maths::Point>* ligneBallayage = new std::vector<maths::Point>();

		maths::Point posXLigneBalayage;
		posXLigneBalayage.x = 0;
		posXLigneBalayage.y = y;
		
		maths:Point posYLigneBalayage;
		posYLigneBalayage.x = width;
		posYLigneBalayage.y = y;

		ligneBallayage->push_back(posXLigneBalayage);
		ligneBallayage->push_back(posYLigneBalayage);

		// On r�cup�re les points du polygon
		std::vector<maths::Point>* pointsFromPolygon = polygon.getPoints();
		int nbPoint = polygon.getPoints()->size();

		// On test les intersections entre la ligne de balayage et tous les c�t�s du polygon
		std::vector<maths::Point>* pointsIntersection = new std::vector<maths::Point>();
		for (int i = 0; i < nbPoint; i++)
		{
			maths::Point pointIntersection;

			// Attention ici on converti les points du rep�re pixel de l'�cran au rep�re OpenGL
			maths::Point* pointXLigneBalayage = ConvertPointPixelToOpenGLUnit(ligneBallayage->at(0));
			maths::Point* pointYLigneBalayage = ConvertPointPixelToOpenGLUnit(ligneBallayage->at(1));

			// Petit cas particulier pour le dernier point que l'on associe au premier point pour tester le c�t� qui ferme le polygon
			// TODO : Trier intersection avec les segements ayant la norme la plus petite par rapport au bord gauche de la fen�tre
			if (i == nbPoint - 1)
			{
				pointIntersection = CVecteur::Intersection(*pointXLigneBalayage, *pointYLigneBalayage, pointsFromPolygon->at(i), pointsFromPolygon->at(0));
			}
			else
			{
				pointIntersection = CVecteur::Intersection(*pointXLigneBalayage, *pointYLigneBalayage, pointsFromPolygon->at(i), pointsFromPolygon->at(i+1));
			}

			if (pointIntersection.x != -1 && pointIntersection.y != -1)
			{
				pointsIntersection->push_back(pointIntersection);
			}
			// TODO: Faire le tri ici
		}
		
		if (pointsIntersection->size() != 0)
		{
			// On dessine entre chaque intersection avec la bonne couleur
			for (int i = 0; i < pointsIntersection->size(); i++)
			{
				// On teste le cas particulier du passage de la ligne de balayage sur un sommet du polygon
				maths::Point* peakDetect = isVertexFromPolygon(polygon, pointsIntersection->at(i));
				if (peakDetect != nullptr)
				{
					// On ajoute 2 fois car on traitra les points contenus dans le tableau 2 � 2 (segement)
					allPointToDraw->push_back(*peakDetect);
					allPointToDraw->push_back(*peakDetect);
				}
				else
				{
					allPointToDraw->push_back(pointsIntersection->at(i));
				}
			}
		}
	}

	return allPointToDraw;
}

maths::Point* Scene::isVertexFromPolygon(maths::Polygon polygon, maths::Point pointTest)
{
	for (int i = 0; i < polygon.getPoints()->size(); i++)
	{
		if (pointTest.x == polygon.getPoints()->at(i).x && pointTest.y == polygon.getPoints()->at(i).y)
		{
			return &(polygon.getPoints()->at(i));
		}
	}

	return nullptr;
}

maths::Point* Scene::ConvertPointPixelToOpenGLUnit(maths::Point point)
{
	maths::Point* pointOpenGL = new maths::Point();

	float v = 2.0 / (double)width;
	float w = 2.0 / (double)height;
	pointOpenGL->x = v * (double)point.x - 1.0;
	pointOpenGL->y = w * (double)point.y - 1.0;

	return pointOpenGL;
}

bool Scene::isPointInPol(maths::Polygon pol, maths::Point p)
{
	pol.calculateNormals();
	int nbPointWin = pol.getPoints()->size();

	for (int j = 0; j < nbPointWin; j++)
	{
		maths::Point p1 = pol.getPoints()->at(j);

		if (!Math::isPointVisible(p, p1, pol.getNormals()->at(j)))
			return false;
	}
	return true;
}

void Scene::cursorInPolygon(maths::Point p)
{
	for (int i = 0; i < polygons->size(); i++)
	{
		bool dog = isPointInPol(polygons->at(i), p);
		
		if (dog == true)
		{
			stackPolygonClicked->push_back(polygons->at(i));
		}
	}
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
