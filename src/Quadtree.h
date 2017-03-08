/*
 * Quadtree.h
 *
 *  Created on: 19 de nov de 2016
 *      Author: cassiano
 */

#ifndef SRC_QUADTREE_H_
#define SRC_QUADTREE_H_

#include "DirectionArray.h"
#include "QuadtreeMesh.h"
#include "IntercardinalDirection.h"
#include "CardinalDirection.h"

#include <string>

class Quadtree {
public:
	Quadtree(Vertex* nw, Vertex* ne, Vertex* sw, Vertex* se,
			QuadtreeMesh * mesh = new QuadtreeMesh());

	virtual ~Quadtree();

	void update(const Vec3f& cameraPosition, const std::string& tag = "main");

	Vertex* getCenter() {
		return center;
	}

	float calculateRoughness(Vertex* center,
			DirectionArray<IntercardinalDirection, Vertex*>& intercardinals);

	void setNeighbours(Vertex* n, Vertex* e, Vertex* s, Vertex* w);

	Mesh* getMesh() {
		return mesh;
	}

	void updateRoughness();
	void updateRoughness2();
	void updateD2();

	static float C;
	static float c;

private:

	void remesh(Vertex* center,
			DirectionArray<IntercardinalDirection, Vertex*>& intercardinals,
			const DirectionArray<CardinalDirection, Vertex*>& neighbors,
			const std::string& tag = "main");

	void deleteUnusedVertices();

	void recursiveUpdateRoughness(Vertex* center,
			DirectionArray<IntercardinalDirection, Vertex*>& intercardinals);

	void recursiveUpdateD2(Vertex* center,
			DirectionArray<IntercardinalDirection, Vertex*>& intercardinals,
			DirectionArray<CardinalDirection, Vertex*>& neighbors);

	void recursiveDeleteVertices(Vertex* center,
			const IntercardinalDirection& direction,
			DirectionArray<IntercardinalDirection, Vertex*>& intercardinals,
			std::size_t level = 0);

	float recursiveUpdateRoughness2(Vertex* center,
			DirectionArray<IntercardinalDirection, Vertex*>& intercardinals,
			DirectionArray<CardinalDirection, Vertex*>* parentNeighbors,
			DirectionArray<CardinalDirection, Vertex*>* neighbors);

	DirectionArray<CardinalDirection, Vertex*> getNeighborhood(Vertex* center,
			const IntercardinalDirection& direction,
			const DirectionArray<IntercardinalDirection, Vertex*>& intercardinals,
			const DirectionArray<CardinalDirection, Vertex*>& neighbors);

	DirectionArray<IntercardinalDirection, Vertex*> getRelativeIntercardinals(
			const IntercardinalDirection& direction, Vertex* center,
			DirectionArray<IntercardinalDirection, Vertex*>& parentIntercardinals);

	DirectionArray<IntercardinalDirection, Vertex*> getRelativeIntercardinalsWithoutCreate(
			const IntercardinalDirection& direction, Vertex* center,
			DirectionArray<IntercardinalDirection, Vertex*>& parentIntercardinals);

	void updateActiveCenters(const Vec3f& cameraPosition, Vertex* center,
			DirectionArray<IntercardinalDirection, Vertex*>& intercardinals,
			const DirectionArray<CardinalDirection, Vertex*>& neighbors);

	void updateActiveIntercardinals(Vertex* center,
			DirectionArray<IntercardinalDirection, Vertex*>& intercardinals);

	Quadtree* parent;
	DirectionArray<CardinalDirection, Vertex*> neighbors;
	DirectionArray<IntercardinalDirection, Quadtree*> children;

	QuadtreeMesh* mesh;
	Vertex* center;
	DirectionArray<IntercardinalDirection, Vertex*> intercardinals;
};

#endif /* SRC_QUADTREE_H_ */
