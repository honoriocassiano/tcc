/*
 * Line.cpp
 *
 *  Created on: 28 de out de 2015
 *      Author: al09
 */

#include "Line.h"

Line::Line(geometry::Point3 origin, geometry::Point3 destiny) : origin(origin), destiny(destiny) {}

Line::~Line() {

}

void Line::drawObject() {
	glBegin(GL_LINES);
	/*
		glLineWidth(2.5);
		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(15, 0, 0);
		*/
		glVertex3f(origin.getX(), origin.getY(), origin.getZ());
		glVertex3f(destiny.getX(), destiny.getY(), destiny.getZ());
	glEnd();
}
