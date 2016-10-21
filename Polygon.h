
#include <vector>

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
public:
	Polygon();
	~Polygon();

private:
	std::vector<Point> points;
};