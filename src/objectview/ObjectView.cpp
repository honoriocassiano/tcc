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
	initdraw();

	glColor4f(color.getR(), color.getG(), color.getB(), color.getAlpha());

	drawObject();
	enddraw();
}

void ObjectView::initdraw() {
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
}

void ObjectView::enddraw() {
	glPopMatrix();
}

void ObjectView::setColor(objectview::Color color) {
	this->color = color;
}
