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

class Quadtree2;

class QuadtreeMesh : public Mesh {
public:
	QuadtreeMesh();
	virtual ~QuadtreeMesh();

	Vertex* getOrCreateChildVertex(Vertex *p1, Vertex *p2);

private:
	Bag<Triangle*>* getTriangles() {
		return triangles;
	}

	friend class Quadtree2;
};

#endif /* SRC_QUADTREEMESH_H_ */
