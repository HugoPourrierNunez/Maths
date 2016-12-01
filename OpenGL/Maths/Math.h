#ifndef DEF_MATHS
#define DEF_MATHS

#include "Polygon.h"

class Math
{
	


public:
	static bool isPointVisible(maths::Point polPoint, maths::Point winPoint, maths::Point n);
	static maths::Point getIntersection(maths::Point p1, maths::Point p2, maths::Point p3, maths::Point p4);
};

#endif

