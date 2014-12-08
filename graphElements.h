#ifndef GRAPH_ELEMENTS_H
#define GRAPH_ELEMENTS_H

namespace graphElements {

	/************
	POINT 2D
	************/
	struct Point2D {
		float x, y;
	};

	/************
	RECTANGLE with two diagonal points
	************/
	struct Rect {
		Point2D point[2]; // p[0] for lower left point and p[1] for upper right point
	};
	/************
	LINE
	************/
	struct Line{
		Point2D p1, p2;
	};

	/*****************
	DISTANCE BETWEEN TWO POINTS
	*****************/
	double distance(Point2D, Point2D);	
	
	/*****************
	LENGTH OF DIAGONAL OF RECT
	*****************/
	double diagonalLength(graphElements::Rect);
}

#endif
