#include "Polygon.h"


void maths::Polygon::addPoint(maths::Point p)
{
	points->push_back(p);
}

void maths::Polygon::removePoint()
{
	if (!points->empty())
		points->pop_back();
}

const std::vector<maths::Point>* maths::Polygon::getPoints()
{
	return points;
}

maths::Polygon::Polygon()
{
	points = new std::vector<maths::Point>();
}


maths::Polygon::~Polygon()
{
}
