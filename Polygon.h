#ifndef DEF_MATHS_POLYGON
#define DEF_MATHS_POLYGON

#include <vector>

namespace maths
{
	struct Point
	{
		float x;
		float y;
		float z;
	};
	typedef struct Point Point;

	struct Color
	{
		float r;
		float g;
		float b;
	};
	typedef struct Color Color;

	class Polygon
	{
		std::vector<maths::Point> *points;

	public:
		void addPoint(maths::Point p);
		void removePoint();
		const std::vector<maths::Point>* getPoints();
		Polygon();
		~Polygon();
	};
}


#endif // !MATHS_POLYGON