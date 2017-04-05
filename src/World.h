/*
 * World.h
 *
 *  Created on: 1 de abr de 2017
 *      Author: cassiano
 */

#ifndef SRC_WORLD_H_
#define SRC_WORLD_H_

#include "structures/vectors.h"

class World {
public:
	World();
	virtual ~World();

	static int tris_rendered;

	static void draw_triangle(Vec3f p1, Vec3f p2, Vec3f p3);

	static void draw_recursive(Vec3f p1, Vec3f p2, Vec3f p3, Vec3f center,
			double size = 1);
	static void draw(Vec3f center);
};

#endif /* SRC_WORLD_H_ */
