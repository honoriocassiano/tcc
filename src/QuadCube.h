/*
 * QuadCube.h
 *
 *  Created on: 9 de set de 2016
 *      Author: aluno
 */

#ifndef SRC_QUADCUBE_H_
#define SRC_QUADCUBE_H_

#include "Quadtree2.h"
#include "structures/Halfedge/mesh.h"
#include "geometry/Vector3.h"
#include "QuadtreeMesh.h"

namespace directions {

enum Face : int {
	FRONT = 0, LEFT, RIGHT, TOP, BOTTOM, BACK
};
}

class QuadCube {
public:
	QuadCube(const Vec3f& position);
	virtual ~QuadCube();

	void draw(bool wireframe = false, bool generateNoise = true);

	void update(const Vec3f& cameraPosition);

//private:

	void initNeighbours();

	QuadtreeMesh * mesh;
	Vec3f position;
	Quadtree2* faces[6];
	directions::Face reference;
};

#endif /* SRC_QUADCUBE_H_ */
