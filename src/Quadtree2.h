/*
 * Quadtree2.h
 *
 *  Created on: 19 de nov de 2016
 *      Author: cassiano
 */

#ifndef SRC_QUADTREE2_H_
#define SRC_QUADTREE2_H_

#include "structures/Halfedge/mesh.h"
#include "DirectionArray.h"
#include "IntercardinalDirection.h"

#include "Quadtree.h"

//namespace directions {
//enum Intercardinal2
//	: int {
//		NW = 0, NE, SE, SW
//};
//}

class Quadtree2 {
public:
	Quadtree2(Vertex* nw, Vertex* ne, Vertex* sw, Vertex* se, Mesh * mesh =
			new Mesh());

	virtual ~Quadtree2();

	void update(const Vec3f& cameraPosition);
	void render();

private:

//	Quadtree2(Vertex* nw, Vertex* ne, Vertex* sw, Vertex* se, Mesh * mesh);



	Quadtree2* parent;
//	Quadtree2* children[4];
	DirectionArray<IntercardinalDirection, Quadtree2*> children;

	Mesh* mesh;
	Vertex* center;
//	Vertex* intercardinals[4];
	DirectionArray<IntercardinalDirection, Vertex*> intercardinals;


	static float C;
};

#endif /* SRC_QUADTREE2_H_ */
