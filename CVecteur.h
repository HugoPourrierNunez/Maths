#ifndef CVECTEUR_H
#define CVECTEUR_H

#include <math.h>
#include "Polygon.h"
#include <iostream>

using namespace std;

class CVecteur
{
    public:
        CVecteur(float x, float y);
		CVecteur();
        virtual ~CVecteur();
        float getX();
        float getY();
        void setX(float x);
        void setY(float y);
		Point Intersection(Point p1, Point p2, Point p3, Point p4);
		static void OrganizePoint(Point* p1, Point* p2);

	protected:
    private:
        float m_dX;
        float m_dY;
        float m_dZ;
};

#endif // CVECTEUR_H
