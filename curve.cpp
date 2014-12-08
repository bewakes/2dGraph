#include "curve.h"
#include "list.h"
#include "list.cpp"

using namespace graphElements;

mathError math;


/***********************************
OVERLOADING OPERATORS FOR MIDPOINT
***********************************/
Point2D operator+(Point2D p1, Point2D p2)
{
	Point2D p;
	p.x = p1.x+p2.x;
	p.y = p1.y+p2.y;
	return p;
}

/**********************************
DIVISION OF POINT BY FLOATING POINT
**********************************/
Point2D operator/(Point2D p, float n)
{
	Point2D temp;
	temp.x = (float)p.x/n;
	temp.y = (float)p.y/n;
	return temp;
}


/************************************
FUNCTION DEFINITIONS FOR Curve CLASS
************************************/

// CONSTRUCTOR

Curve:: Curve(char * s) {

    setStep(15);
    setRange();
	e = new Expression(s);

    bigRect.point[0].x = -RANGE; bigRect.point[1].x = RANGE;
    bigRect.point[0].y = -RANGE; bigRect.point[1].y = RANGE;

    initialDivide();
}

double Curve::evaluateValue(Point2D point)
{
	double x = point.x, y = point.y;

	List<double> l;
	for(int i = 0; i<e->postfix.size(); i++) {
		if(e->postfix[i].type==VALUE) l.append(e->postfix[i].numVal);
		else if(e->postfix[i].type==OPERATOR and strcmp("#", e->postfix[i].str)!=0) {
			double a, b;
			b = l.pop();
			a = l.pop();
			if(strcmp(e->postfix[i].str, "+")==0) l.append(a+b);
			else if(strcmp(e->postfix[i].str, "-")==0) l.append(a-b);
			else if(strcmp(e->postfix[i].str, "*")==0) l.append(a*b);
			else if(strcmp(e->postfix[i].str, "/")==0) l.append(a/b);
            else if(strcmp(e->postfix[i].str, "^")==0){
                if(b!=(int)b/1.0 and a<0)throw math;
                l.append(pow(a, b));
            }
		}
		else if(e->postfix[i].type==OPERATOR and strcmp("#", e->postfix[i].str)==0) {
			double a;
			a = l.pop();
			l.append(a*(-1));
		}

		else if(e->postfix[i].type==FUNCTION) {
			double a;
			a = l.pop();
			if(strcmp(e->postfix[i].str, "sin")==0) l.append(sin(a));
			else if(strcmp(e->postfix[i].str, "cos")==0) l.append(cos(a));
			else if(strcmp(e->postfix[i].str, "tan")==0) l.append(tan(a));
			else if(strcmp(e->postfix[i].str, "sinh")==0) l.append(sinh(a));
			else if(strcmp(e->postfix[i].str, "cosh")==0) l.append(cosh(a));
			else if(strcmp(e->postfix[i].str, "tanh")==0) l.append(tanh(a));
            else if(strcmp(e->postfix[i].str, "log")==0) { if(a<0)a=0; l.append(log10(a)); }
            else if(strcmp(e->postfix[i].str, "ln")==0) {if(a<0) a=0;  l.append(log10(a)/log10(2.71828));}
		}

		else if(e->postfix[i].type==VARIABLE) {
            if(strcmp(e->postfix[i].str, "x")==0) l.append(x/(200));
            if(strcmp(e->postfix[i].str, "y")==0) l.append(y/(200));
		}

		else if(e->postfix[i].type==CONSTANT) {
			if(strcmp(e->postfix[i].str, "p")==0) l.append(3.141592);
			if(strcmp(e->postfix[i].str, "e")==0) l.append(2.71828);
		}
	}
    return l.pop();
}


void Curve::initialDivide() // dividing the big area to small ones
{
	Rect temp;
	int x, y;

	for(x = 0;x<(bigRect.point[1].x-bigRect.point[0].x)/STEP; x++) {

		for(y=0;y<(bigRect.point[1].y-bigRect.point[0].y)/STEP; y++) {

			temp.point[0].x = bigRect.point[0].x + x*STEP;
			temp.point[1].x = temp.point[0].x + STEP;
			temp.point[0].y = bigRect.point[0].y + y*STEP;
			temp.point[1].y = temp.point[0].y + STEP;
			initialDivisions.push_back(temp);
		}
	}
	for (int i =0; i<initialDivisions.size(); i++) {
		subdivide(initialDivisions[i]);
	}
}

bool Curve::sameSign(Point2D p1, Point2D p2)
{
	double a = evaluateValue(p1);
	double b = evaluateValue(p2);
	return ((a>=0 and b>=0) or (a<0 and b<0));
}

void Curve::subdivide(Rect r)
{
	Point2D p1, p2, p3, p4; // point 1 for lower left, 2 for lower right, 3 for upper right and 4 for upper left

	// assigning the four points of rect to p1, p2, p3 and p4
	p1.x = r.point[0].x; p1.y = r.point[0].y;
	p2.x = r.point[1].x; p2.y = r.point[0].y;
	p3.x = r.point[1].x; p3.y = r.point[1].y;
	p4.x = r.point[0].x; p4.y = r.point[1].y;

	Line tempLine;
	if(diagonalLength(r) < 0.2) {  /* box in the range of pixel */
		finalPoints.push_back(r.point[0]);

		// check which line to be drawn( horizonatal, vertical, right_diagonal or left_diagonal
/*
		//  1,4 -> same and 2,3 -> same then vertical line
		if(sameSign(p1, p4) and sameSign(p2, p3) and !sameSign(p1, p2)) {
			tempLine.p1 = (p1+p2)/2;
			tempLine.p2 = (p3+p4)/2;
		}

		// if 1,2 -> same sign and 3,4 -> same sign then horizontal line
		else if (sameSign(p1, p2) and sameSign(p3, p4) and !sameSign(p1, p3)) {
			tempLine.p1 = (p1+p4)/2;
			tempLine.p2 = (p3+p2)/2;
		}

		// if 1,3 -> same sign and 2,4 -> opposite sign then right diagonal (from 1 to 3)
		 else if (sameSign(p1, p3) and sameSign(p2, p4) and !sameSign(p1, p2)) {
			tempLine.p1 = p1;
			tempLine.p2 = p3;
		}

		// if 2,4 -> same sign and 1,3 -> opposite sign then left diagonal (from 4 to 2)
		else if(sameSign(p2, p4) and sameSign(p1, p3) and !sameSign(p2, p1)) {
			tempLine.p1 = p2;
			tempLine.p2 = p4;
		}

	else */tempLine.p1=r.point[0];tempLine.p2=r.point[1];
        //finalLines.push_back(tempLine);
        //cout << finalLines.size()<<" ";
		//displayLine(tempLine);cout << diagonalLength(r)<<endl;
		return;
	}

    //try{
        if(sameSign(p1, p3) and sameSign(p2,p4) ) { /* the box doesnot contain a part of the Curve/curve */
            // return
            return;
      //  }
	}
    /*catch(mathError) {
        int cnt = 0;
        try{ evaluateValue(p1); } catch(mathError) { cnt++; }
        try{ evaluateValue(p2); } catch(mathError) { cnt++; }
        try{ evaluateValue(p3); } catch(mathError) { cnt++; }
        try{ evaluateValue(p4); } catch(mathError) { cnt++; }
        if(cnt==4)return;
        else {
            Rect temp[4];
            double stepX = (r.point[1].x - r.point[0].x)/2.f;
            double stepY = (r.point[1].y - r.point[0].y)/2.f;
            int cnt = 0;

            for(int x = 0; x<2; x++) {
                for(int y = 0; y<2; y++) {
                    temp[cnt].point[0].x = r.point[0].x + x*stepX;
                    temp[cnt].point[1].x = temp[cnt].point[0].x + stepX;
                    temp[cnt].point[0].y = r.point[0].y + y*stepY;
                    temp[cnt].point[1].y = temp[cnt].point[0].y + stepY;
                    cnt++;
                }
            }
            // and subdivide() each part
            for(int x = 0; x<4; x++) subdivide(temp[x]);
            return;
        }
    } */

	if( !sameSign(p1, p3) or !sameSign(p2, p4) ) {	/* the box contains the part of the Curve/curve */
		// divide the box into four parts
		Rect temp[4];
		double stepX = (r.point[1].x - r.point[0].x)/2.f;
		double stepY = (r.point[1].y - r.point[0].y)/2.f;
		int cnt = 0;

		for(int x = 0; x<2; x++) {
			for(int y = 0; y<2; y++) {
				temp[cnt].point[0].x = r.point[0].x + x*stepX;
				temp[cnt].point[1].x = temp[cnt].point[0].x + stepX;
				temp[cnt].point[0].y = r.point[0].y + y*stepY;
				temp[cnt].point[1].y = temp[cnt].point[0].y + stepY;
				cnt++;
			}
		}
		// and subdivide() each part
		for(int x = 0; x<4; x++) subdivide(temp[x]);
	}
	return;
}
void Curve::drawPoints()
{

glScalef(1/8.f,1/8.f,1);
glBegin(GL_POINTS);
glColor3f(0.4, 0.3, 0.2);
for(int i=0;i<finalPoints.size();i++) {
    glVertex2f(finalPoints[i].x, finalPoints[i].y);
}
glEnd();


}
