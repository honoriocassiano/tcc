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
#include "CardinalDirection.h"

#include "Quadtree.h"

class Quadtree2 {
public:
	Quadtree2(Vertex* nw, Vertex* ne, Vertex* sw, Vertex* se, Mesh * mesh =
			new Mesh());

	virtual ~Quadtree2();

	void update(const Vec3f& cameraPosition);
	void render();

	Mesh* getMesh() {
		return mesh;
	}

private:

	Quadtree2(Vertex* nw, Vertex* ne, Vertex* sw, Vertex* se, Vertex* center,
			Mesh * mesh, const DirectionArray<CardinalDirection, bool>& marked, Quadtree2* parent);
//	Quadtree2(Vertex* nw, Vertex* ne, Vertex* sw, Vertex* se, Vertex* center, Mesh * mesh,
//			const DirectionArray<CardinalDirection, bool>& marked, const DirectionArray<CardinalDirection, bool>& willHaveNeighbor);

	Quadtree2* parent;
	DirectionArray<CardinalDirection, Quadtree2*> neighbors;
	DirectionArray<IntercardinalDirection, Quadtree2*> children;

	Mesh* mesh;
	Vertex* center;
	DirectionArray<IntercardinalDirection, Vertex*> intercardinals;

	static float C;
};

#endif /* SRC_QUADTREE2_H_ */
