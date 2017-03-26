/*
 * QuadtreeMesh.h
 *
 *  Created on: 11 de dez de 2016
 *      Author: cassiano
 */

#ifndef SRC_QUADTREEMESH_H_
#define SRC_QUADTREEMESH_H_

#include "structures/Halfedge/mesh.h"
#include "structures/Halfedge/vertex.h"
#include "structures/Halfedge/vertex_parent.h"
#include "structures/Halfedge/edge.h"
#include "structures/Halfedge/triangle.h"

class Quadtree;

class QuadtreeMesh: public Mesh {
public:
	QuadtreeMesh();
	virtual ~QuadtreeMesh();

	Vertex* getOrCreateChildVertex(Vertex *p1, Vertex *p2);

	bool deleteChildIfExist(Vertex *p1, Vertex *p2);

	bool deleteVertex(MultiLevelArray<Vertex*>::Iterator& it);

private:
	Bag<Triangle*>* getTriangles() {
		return triangles;
	}

	Bag<Edge*>* getEdges() {
		return edges;
	}

	Array<Vertex*>* getVertices() {
		return vertices;
	}

	MultiLevelArray<Vertex*>* getVertices2() {
		return vertices2;
	}

	friend class Quadtree;
	friend class QuadCube;
};

#endif /* SRC_QUADTREEMESH_H_ */
