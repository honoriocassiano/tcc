/*
 * ViewObject.h
 *
 *  Created on: 28 de out de 2015
 *      Author: al09
 */

#include <GL/gl.h>
#include <GL/glu.h>
#include "Color.h"

#ifndef OBJECTVIEW_H_
#define OBJECTVIEW_H_

class ObjectView {
public:
	ObjectView();
	virtual ~ObjectView();

	void draw();
	void setColor(objectview::Color);

protected:
	virtual void drawObject() = 0;

private:

	objectview::Color color = objectview::Color(1.0f, 1.0f, 1.0f);

	void initdraw();
	void enddraw();

};

#endif /* OBJECTVIEW_H_ */
