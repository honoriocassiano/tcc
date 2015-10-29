/*
 * Line.h
 *
 *  Created on: 28 de out de 2015
 *      Author: al09
 */

#ifndef OBJECTVIEW_LINE_H_
#define OBJECTVIEW_LINE_H_

#include "ObjectView.h"

class Line: public ObjectView {
public:
	Line();
	virtual ~Line();

protected:
	virtual void drawObject();
};

#endif /* OBJECTVIEW_LINE_H_ */
