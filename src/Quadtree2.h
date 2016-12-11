/*
 * Quadtree2.h
 *
 *  Created on: 19 de nov de 2016
 *      Author: cassiano
 */

#ifndef SRC_QUADTREE2_H_
#define SRC_QUADTREE2_H_

#include "DirectionArray.h"
#include "QuadtreeMesh.h"
#include "IntercardinalDirection.h"
#include "CardinalDirection.h"

#include "Quadtree.h"

#include <string>

class Quadtree2 {
public:
	Quadtree2(Vertex* nw, Vertex* ne, Vertex* sw, Vertex* se,
			QuadtreeMesh * mesh = new QuadtreeMesh());

	virtual ~Quadtree2();

	void update2(const Vec3f& cameraPosition, const std::string& tag = "main");
	void update(const Vec3f& cameraPosition, const std::string& tag = "main");
	void render();

	Mesh* getMesh() {
		return mesh;
	}

private:

	void remesh(Vertex* center,
			const DirectionArray<IntercardinalDirection, Vertex*>& intercardinals,
			const DirectionArray<CardinalDirection, Vertex*>& neighbors,
			const std::string& tag = "main");

	float calculateRoughness(const Vertex* center,
			const DirectionArray<IntercardinalDirection, Vertex*>& intercardinals);

	DirectionArray<CardinalDirection, Vertex*> getNeighborhood(
			Vertex* center, const IntercardinalDirection& direction,
			const DirectionArray<IntercardinalDirection, Vertex*>& intercardinals,
			const DirectionArray<CardinalDirection, Vertex*>& neighbors);

	void updateActives(const Vec3f& cameraPosition, Vertex* center,
			DirectionArray<IntercardinalDirection, Vertex*>& intercardinals,
			const DirectionArray<CardinalDirection, Vertex*>& neighbors);

	Quadtree2(Vertex* nw, Vertex* ne, Vertex* sw, Vertex* se, Vertex* center,
			QuadtreeMesh * mesh,
			const DirectionArray<CardinalDirection, bool>& marked,
			Quadtree2* parent);
//	Quadtree2(Vertex* nw, Vertex* ne, Vertex* sw, Vertex* se, Vertex* center, Mesh * mesh,
//			const DirectionArray<CardinalDirection, bool>& marked, const DirectionArray<CardinalDirection, bool>& willHaveNeighbor);

	Quadtree2* parent;
	DirectionArray<CardinalDirection, Quadtree2*> neighbors;
	DirectionArray<IntercardinalDirection, Quadtree2*> children;

	QuadtreeMesh* mesh;
	Vertex* center;
	DirectionArray<IntercardinalDirection, Vertex*> intercardinals;

	static float C;
};

#endif /* SRC_QUADTREE2_H_ */
