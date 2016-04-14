/*
 * ViewObject.cpp
 *
 *  Created on: 28 de out de 2015
 *      Author: al09
 */

#include "ObjectView.h"

ObjectView::ObjectView() {
	//this->color = objectview::Color(1.0f, 1.0f, 1.0f);
}

ObjectView::~ObjectView() {
	// TODO Auto-generated destructor stub
}

void ObjectView::draw() {
	GLint mode  = initdraw();

	glColor4f(color.getR(), color.getG(), color.getB(), color.getAlpha());

	drawObject();
	enddraw(mode);
}

GLint ObjectView::initdraw() {

	GLint mode = 0;
	glGetIntegerv(GL_MATRIX_MODE, &mode);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	return mode;
}

void ObjectView::enddraw(GLint matrixMode) {
	glFlush();
	glPopMatrix();
	glMatrixMode(matrixMode);
}

void ObjectView::setColor(color::Color color) {
	this->color = color;
}
