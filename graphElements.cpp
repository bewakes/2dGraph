#include "graphElements.h"
#include <cmath>
double graphElements::distance(graphElements::Point2D p1, graphElements::Point2D p2)
	{
		float c;
		c = (p2.x-p1.x)*(p2.x-p1.x) + (p2.y-p1.y)*(p2.y-p1.y);
		return pow(c, 0.5);
	}

double graphElements::diagonalLength(graphElements::Rect r)
	{
		return distance(r.point[0], r.point[1]);
	}
