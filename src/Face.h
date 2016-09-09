/*
 * Face.h
 *
 *  Created on: 9 de set de 2016
 *      Author: aluno
 */

#ifndef SRC_FACE_H_
#define SRC_FACE_H_

#include "Quadtree.h"

class Face {
public:
	Face(const Vec3f& origin, const Vec3f& xAxis, const Vec3f& yAxis,
			int level, int maxLevel);
	virtual ~Face();

	void setTop(Face* _top) {
		top = _top;
	}

	void setRight(Face* _right) {
		right = _right;
	}

	void setBottom(Face* _bottom) {
		bottom = _bottom;
	}

	void setLeft(Face* _left) {
		left = _left;
	}

private:
	Face* top;
	Face* right;
	Face* bottom;
	Face* left;

	Quadtree* tree;
};

#endif /* SRC_FACE_H_ */
