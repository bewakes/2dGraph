#ifndef Curve_H
#define Curve_H

#include "expression.h"
#include "list.h"
#include "headers.h"

class Curve {
private:
	graphElements::Rect bigRect; // the initial big rectangle
	vector<graphElements::Rect> initialDivisions; // contains initially subdivided boxes
	vector<Token> postfix;
	int rangeX[2], rangeY[2];
    Expression * e;
	int STEP;
    int RANGE;
public:
    Curve(char * s);
    vector<graphElements::Point2D>finalPoints;
    //void initialize(Expression &e);
	void initialDivide();
	void render();
	void subdivide(graphElements::Rect r);
	double evaluateValue(graphElements::Point2D);
	bool sameSign(graphElements::Point2D, graphElements::Point2D); // returns true if the two points lie on the same side of curve/surface
    void setStep(int n=40) { STEP = n; }
    void drawPoints();
    void setRange(int n = 1500){ RANGE = n; }
};

#endif
