/*
 * Line.h
 *
 *  Created on: 28 de out de 2015
 *      Author: al09
 */

#ifndef OBJECTVIEW_LINE_H_
#define OBJECTVIEW_LINE_H_

#include "ObjectView.h"
#include "../geometry/Point3.h"

class Line: public ObjectView {
public:
	explicit Line(geometry::Point3, geometry::Point3);
	virtual ~Line();

protected:
	virtual void drawObject();

private:
	geometry::Point3 origin;
	geometry::Point3 destiny;
};

#endif /* OBJECTVIEW_LINE_H_ */
