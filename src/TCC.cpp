//============================================================================
// Name        : TCC.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "objectview/Line.h"
#include <iostream>
#include "objectview/Color.h"
#include "geometry/Point3.h"
#include <GL/glut.h>

using namespace std;
using namespace geometry;

void display(void)
{

	/*
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_POLYGON);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(0.5, 0.0, 0.0);
        glVertex3f(0.5, 0.5, 0.0);
        glVertex3f(0.0, 0.5, 0.0);
    glEnd();
    glFlush();
*/

	glClear(GL_COLOR_BUFFER_BIT);

	/*
	glColor3f(1.0, 0.0, 0.0);
			glVertex3f(0.0, 0.0, 0.0);
			glVertex3f(15, 0, 0);
			*/

	Point3 origin = Point3(0.0f, 0.0f, 0.0f);
	Point3 destiny = Point3(5.0f, 0.0f, 0.0f);

	Line *l =  new Line(origin, destiny);
	l->setColor(color::Color::BLUE);

	l->draw();

	//glFlush();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(300, 300);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Hello world");
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}
