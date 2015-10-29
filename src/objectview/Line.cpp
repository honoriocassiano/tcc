/*
 * Line.cpp
 *
 *  Created on: 28 de out de 2015
 *      Author: al09
 */

#include "Line.h"

Line::Line() {
	// TODO Auto-generated constructor stub

}

Line::~Line() {
	// TODO Auto-generated destructor stub
}

void Line::drawObject() {
	glBegin(GL_LINES);
		glLineWidth(2.5);
		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(15, 0, 0);
	glEnd();
}
