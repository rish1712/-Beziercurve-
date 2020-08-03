#include<GL\freeglut.h>
#include<iostream>
#include<cmath>
#include<vector>
using namespace std;
#define eps 100
class Point
{
public:
	double x, y;
	/*!
	 Constructor of Point class with no arguments. The constructor sets the 2 variable x,y of the class as zero
	*/
	Point()
	{
	     x = 0;
	     y = 0;
    }
    /*!
	 Constructor of Point class with 2 arguments as x and y coordinate of a point. The constructor sets the 2 variables x,y of the class as given in the arguments
	*/
	Point(double xx, double yy)
	{
	    x = xx;
        y = yy;
    }
    /*!
	 function of Point class with 2 arguments as x and y coordinate of a point. The constructor sets the 2 variables x,y of the class as given in the arguments
	 the return type of this function is void
	*/
	 void setxy(float x2, float y2)
    {
        x = x2;
        y = y2;
    }
};
bool boo = false;
int moving = -1;
int points = 0;
int Height = 480;
Point click[100];
int flag=1;
void myInit()
{
    /*!glClearColor gives the background color of the window
	*/
	glClearColor(0, 0, 0, 0);
	/*!glColor sets the color of diffrent objects to be drawn on screen
	*/
	glColor3f(1, 0, 0);
	/*!glPointSize sets the point size of the points on the screen
	*/
	glPointSize(10);
	/*!glMatrixMode sets the Matrix Mode for the given project
	*/
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 640, 0, Height);
}
/*!Drawing vertex of the control polygon. This function takes 2 integers as arguments which are the x and y coordinate of the vertex of the control polygon
   the return type of this function is void
*/
void drawDot(int x, int y)
{
	glBegin(GL_POINTS);
	glColor3f(1,0,0);
	glVertex2i(x, y);
	glEnd();
	glFlush();
}
/*!Drawing line or edges of the control polygon.This function takes 2 objects of Point class as arguments and draws the edge of the control polygon
   the return type of this function is void
*/
void drawLine(Point p1, Point p2)
{
	glBegin(GL_LINES);
	glVertex2d(p1.x, p1.y);
	glVertex2d(p2.x, p2.y);
	glEnd();
	glFlush();
}
/*!Drawing the Bezier curve using deCasteljau algorithm. This function acts recursive. This function takes 3 arguments 2 integers and 1 double. The argument r is used to check the recursive call
   The variable t is used to make interpolations.
   the return type of this function is an object of Point class
*/
Point deCasteljauPoint(int r, int i, double t)
{
    Point temp;

    if(r == 0)
        return click[i];

    Point po1 = deCasteljauPoint(r - 1, i, t);
    Point po2 = deCasteljauPoint(r - 1, i + 1, t);

    temp.setxy((float) ((1 - t) * po1.x + t * po2.x), (float) ((1 - t) * po1.y + t * po2.y));
    return temp;
}

/*!Calling the deCasteljau algorithm for rawing the curve. This function takes an array of type Point and a variable t for making the curve
   the return type of this function is an object of Point class
*/
Point Bezier(Point PT[], double t)
{
    Point P;

    P.x = 0;
    P.y = 0;
    P = deCasteljauPoint(points-1,0,t);
    return P;
}
/*!
    this function is used to draw the curve after a point is moved from its initial point. Its arguments are void
    the return type of this function is void
*/
void drag()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
	for (int i = 0; i < points; i++) drawDot(click[i].x, click[i].y);
	if (boo)
	{
		glColor3f(1, 0, 1);
		for (int i = 0; i < points - 1; i++)
			drawLine(click[i], click[i + 1]);
		Point POld = click[0];
		for (double t = 0; t < 1.0; t += 0.01)
		{
			Point P = Bezier(click,t);
			drawLine(POld, P);
			POld = P;
		}
		glColor3f(1, 0, 0);
	}
	glFlush();
}
/*!
    this function is used to draw the curve if any new point is added or deleted from the curve . The first one is to decide whether left button or right button is clicked
    one to check whether there is release or press of the mouse pointer. The other 2 arguments are used to bood the x and y coordinates of the point clicked on the screen.
    the return type of this function is void
*/
void myMouse(int Button, int state, int x, int y)
{
	if (Button == GLUT_LEFT_BUTTON && state == GLUT_UP && moving != -1) moving = -1;
	if (flag==0&& Button==GLUT_LEFT_BUTTON&&state==GLUT_DOWN)
    {
                int pointer=-1;
                Point P=Point(x,Height-y);
                glClear(GL_COLOR_BUFFER_BIT);
                for(int i=0;i<points;i++)
                {
                    if((click[i].x<=P.x+20&&click[i].x>=P.x-20)&&(click[i].y<=P.y+20&&click[i].y>=P.y-20))
                    {
                        pointer=i;
                        break;
                    }
                }
                if(pointer!=-1)
                {
                    for(int i=pointer;i<points-1;i++)
                    {
                        click[i].x=click[i+1].x;
                        click[i].y=click[i+1].y;
                    }
                    points--;
                    glColor3f(1, 0, 1);
                    for(int i=0;i<points;i++)
                        drawDot(click[i].x,click[i].y);
                    for (int i = 0; i < points - 1; i++)
                        drawLine(click[i], click[i + 1]);
                    Point POld = click[0];
                    for (double t = 0; t < 1.0; t += 0.01)
                    {
                        Point P = Bezier(click,t);
                        drawLine(POld, P);
                        POld = P;
                    }
                    glColor3f(1, 0, 0);
                }
                flag=1;
                if(pointer==-1)
                {
                    cout<<"Please give a valid point for deletion";
                }
    }
	else if (Button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		for (int i = 0; i < points; i++)
		{
			if (pow(x - click[i].x, 2) + pow(Height - y - click[i].y, 2) < eps)
			{
				moving = i;
				return;
			}
		}
		if (boo)
		{
			boo = false;
			points = 0;
			glClear(GL_COLOR_BUFFER_BIT);
			glFlush();
		}
		click[points++] = Point(x, Height - y);
		drawDot(x, Height - y);
	}
	else if (Button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		if (boo || points==0)
		{
			glClear(GL_COLOR_BUFFER_BIT);
			glFlush();
			boo = false;
			return;
		}
		glColor3f(1, 0, 1);
		for(int i=0;i<points;i++)
            drawDot(click[i].x,click[i].y);
		for (int i = 0; i < points - 1; i++)
			drawLine(click[i], click[i + 1]);
		Point POld = click[0];
		for (double t = 0; t < 1.0; t += 0.01)
		{
			Point P = Bezier(click,t);
			drawLine(POld, P);
			POld = P;
		}
		glColor3f(1, 0, 0);
		boo = true;
	}
}
void myDisplay()
{
	glFlush();
}
/*!
    this function is used when mouse cursor is moved from one point to the another. Its arguments are are the x and y coordinates of the desired point
    the return type of this function is void
*/
void myMotion(int x, int y)
{
	if (moving != -1)
	{
		click[moving].x = x;
		click[moving].y = Height - y;
		drag();
	}
}
/*!
    this function is used when we have to delete any vertex of control Polygon. This function has 2 arguments used for capturing the keyboard input
    the return type of this function is void
*/

void deletes(unsigned char key, int x, int y)
{
    switch(key)
    {
    case 'd':
        flag=0;
        break;
    }
}
int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	/*!glutInitDisplayMode sets the display mode as GLUT_SINGLE orGLUT_RGB
	*/
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	/*!glutInitWindowSize sets the windows size according to the arguments given
	*/
	glutInitWindowSize(640, Height);
	/*!glutInitWindowSize sets the windows position according to the arguments given
	*/
	glutInitWindowPosition(200, 50);
	/*! glutCreateWindow gives the title to the window which is given as string as arguments
    */
	glutCreateWindow("Bezier Curve");
	/*!register callbacks for Mouse events that occur when you press a key
    */
	glutMouseFunc(myMouse);
	/*! determines the normal plane for the windows that need to be displayed
    */
	glutDisplayFunc(myDisplay);
	/*!register callbacks for keyboard events that occur when you click mouse
    */
	glutKeyboardFunc(deletes);
	/*!register callbacks for  events when mouse cursor is moved from point A to point B
    */
	glutMotionFunc(myMotion);
	myInit();
	/*!glutMainLoop() is a function that loops within itself, processing events and triggering your callback functions when necessary
    */
	glutMainLoop();
	return 0;

}

