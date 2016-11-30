
#ifdef _MSC_VER
#pragma comment(lib, "opengl32.lib")
#include <Windows.h>
#endif

#include <iostream>
#include <vector>

using namespace std;

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

// Variables globales
EsgiShader g_BasicShader;
int width = 800;
int height = 600;
int g_nWindowID;
bool g_bDetectLeftClic = false;
float g_posXClic = 0;
float g_posYClic = 0;
float g_lastPosXClic = 0;
float g_lastPosYClic = 0;
float v = 2.0f / (float)width;
float w = 2.0f / (float)height;
int nbSommet = 2;
// Tableau de segment d'un polygone
vector<float> arrayLine;


void animate()
{
	glViewport(0, 0, width, height);
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);

	auto program = g_BasicShader.GetProgram();

	glUseProgram(program);
	
	auto color_location = glGetAttribLocation(program, "a_Color");
	auto position_location = glGetAttribLocation(program, "a_Position");

	if (g_bDetectLeftClic == true)
	{
		if (g_lastPosXClic == 0 && g_lastPosYClic == 0)
		{
			g_lastPosXClic = g_posXClic;
			g_lastPosYClic = g_posYClic;

			arrayLine.push_back(g_lastPosXClic);
			arrayLine.push_back(g_lastPosYClic);
		}
		else
		{	
			// On assigne ca au vertex buffer
			arrayLine.push_back(g_posXClic);
			arrayLine.push_back(g_posYClic);

			GLfloat vertex_buffer_data[] = {g_lastPosXClic , g_lastPosYClic , g_posXClic , g_posYClic };	

			// POUR AFFICHER TOUTES LES LIGNES, ON LES METS TOUS LES SOMMETS DANS LE VERTEX BUFFER 
			/*
			GLfloat *vertex_buffer_data = new GLfloat[arrayLine.size()];

			for (int i = 0; i < arrayLine.size(); i++)
			{
				vertex_buffer_data[i] = arrayLine[i];
			}
			*/

			// Identificateurs des sommets
			GLuint vertexbuffer;

			// Génère un tampon et place l'identifiant dans 'vertexbuffer'
			glGenBuffers(1, &vertexbuffer);

			// Les commandes suivantes vont parler de notre tampon 'vertexbuffer'
			glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);

			// Fournit les sommets à OpenGL.
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW);

			// premier tampon d'attributs : les sommets
			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
			glVertexAttribPointer(
				0,                   
				2,                  // taille
				GL_FLOAT,           // type 
				GL_FALSE,            
				0,                  // nombre d'octets séparant deux sommets dans le tampon
				(void*)0           
			);

			// Dessine la ligne
			glDrawArrays(GL_LINES, 0, nbSommet); // Démarre à partir du sommet 0; 2 sommets au total
			//nbSommet += 2;
			glDisableVertexAttribArray(0);

			g_lastPosXClic = g_posXClic;
			g_lastPosYClic = g_posYClic;
		}
		
		cout << "Click gauche detecte \n" << endl;
		// Attention ici on récupère des valeurs de x tel que 0 < x < 800 et des valeurs de y tel que 0 < y < 600
		cout << "X : " << g_posXClic << " - Y : " << g_posYClic << endl;
		g_bDetectLeftClic = false;
		g_posXClic = 0;
		g_posYClic = 0;
	}

	glUseProgram(0);

	glutSwapBuffers();
}

// fonction de gestion du clavier
void KeyBoardDetect(unsigned char touche, int x, int y)
{
	// traitement des touches q et echap
	if (touche == 'q' || touche == 27)
	{
		// destruction de la fenetre GLUT
		glutDestroyWindow(g_nWindowID);
		// on quitte le programme
		exit(EXIT_SUCCESS);
	}
}

// fonction de gestion de la souris
void MouseDetect(int button, int state, int x, int y)
{
	switch (button)
	{
		case GLUT_LEFT_BUTTON:   //gestion du bouton gauche*/
			if (state == GLUT_DOWN)   //Si le bouton gauche est clicker
				g_bDetectLeftClic = true;
				
			// Attention important ici, on convertit en unité opengl
				g_posXClic = (v * (double)x - 1.0f);
				g_posYClic = (w * (double)y - 1.0f);
			break;
		case GLUT_RIGHT_BUTTON:  //gestion du bouton droit de la souris
			if (stat == GLUT_DOWN)  //si le bouton est enfonce
				cout << "Bouton droit de la souris clicker\n" << endl;  //Affichage du message
			break;
	}
}


int main(int argc, const char* argv)
{
	glutInit(&argc, (char **)argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(200, 100);
	glutInitWindowSize(width, height);
	g_nWindowID = glutCreateWindow("Maths");

	#ifndef NO_GLEW
	glewInit();
	#endif

	g_BasicShader.LoadVertexShader("basic.vs");
	g_BasicShader.LoadFragmentShader("basic.fs");
	g_BasicShader.CreateProgram();

	glutDisplayFunc(animate);
	glutMouseFunc(MouseDetect);
	glutKeyboardFunc(KeyBoardDetect);
	glutMainLoop();

	g_BasicShader.DestroyProgram();

	return 1;
}