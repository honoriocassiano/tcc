/*
 * Face.cpp
 *
 *  Created on: 9 de set de 2016
 *      Author: aluno
 */

#include <src/Face.h>

Face::Face(const Vec3f& origin, const Vec3f& xAxis, const Vec3f& yAxis,
		int level, int maxLevel) :
		top(nullptr), right(nullptr), bottom(nullptr), left(nullptr), tree(
				new Quadtree(origin, xAxis, yAxis, level, maxLevel)) {
}

Face::~Face() {
	// TODO Auto-generated destructor stub
}

