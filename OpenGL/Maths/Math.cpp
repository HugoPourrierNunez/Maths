#include "Math.h"



bool Math::isPointVisible(maths::Point polPoint, maths::Point winPoint, maths::Point n)
{
	return ((n.x*(polPoint.x - winPoint.x) + n.y*(polPoint.y - winPoint.y))>0);
}

maths::Point Math::getIntersection(maths::Point p1, maths::Point p2, maths::Point p3, maths::Point p4)
{
	maths::Point p;

	float x = p2.x - p1.x;
	float x2 = p2.y - p1.y;
	float y = p3.x - p4.x;
	float y2 = p3.y - p4.y;
	float z = p3.x - p1.x;
	float z2 = p3.y - p1.y;
	int s = (z2*x - z*x2) / (y2*x - y*x2);
	int t = (z - s*y) / x;

	if (s < 0 || s>1 || t < 0 || t>1)
	{
		p.x = -1;
		p.y = -1;
		return p;
	}
	p.x = p1.x + x*t;
	p.y = p1.y + x2*t;
	return p;
}
