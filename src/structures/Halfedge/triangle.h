#ifndef _TRIANGLE_H
#define _TRIANGLE_H

#include <limits.h>
#include "boundingbox.h"
#include "edge.h"

#include "vertex.h"

// ===========================================================

class Triangle {

public:

	// ========================
	// CONSTRUCTOR & DESTRUCTOR
	Triangle() {
		edge = nullptr;
		hypotenuseOpposite = nullptr;
	}
	~Triangle() {
	}

	// here's the hash function to use for triangles so they
	// can be efficiently accessed within the Bag data structure
	static void extract_func(Triangle *t, int &a, int &b, int &c) {
		a = (*t)[0]->getIndex();
		b = (*t)[1]->getIndex();
		c = (*t)[2]->getIndex();
	}

	// Return edge what contains vertex opposite to hypotenuse
	Edge* getHypotenuseOpposite() {
		if (!hypotenuseOpposite) {
			Edge* currentEdge = edge;

			Vertex* mid = nullptr;

			Vertex* v0 = nullptr;
			Vertex* v1 = nullptr;
			Vertex* v2 = nullptr;

			do {
				v0 = currentEdge->getVertex();
				v1 = currentEdge->getNext()->getVertex();
				v2 = currentEdge->getNext()->getNext()->getVertex();

				auto v01 = v1->get() - v0->get();
				auto v12 = v2->get() - v1->get();

				// TODO Check this comment
				// Check if the vectors are "ortoghonals"
				if (v01.Dot3(v12) == 0.0) {
					hypotenuseOpposite = currentEdge;
					break;
				} else {
					currentEdge = currentEdge->getNext();
				}
			} while (currentEdge != edge);
		}

		return hypotenuseOpposite;
	}

	// =========
	// ACCESSORS
	Vertex* operator[](int i) const {
		assert(edge != NULL);
		if (i == 0)
			return edge->getVertex();
		if (i == 1)
			return edge->getNext()->getNext()->getVertex();
		if (i == 2)
			return edge->getNext()->getVertex();
		assert(0);
	}
	Edge* getEdge() {
		assert(edge != NULL);
		return edge;
	}
	void setEdge(Edge *e) {
		assert(edge == NULL);
		edge = e;

		hypotenuseOpposite = nullptr;
	}

	const Vec3f& getNormal() {
		return normal;
	}
	void setNormal(Vec3f n) {
		normal = n;
	}

	// NOTE: If you want to modify a triangle, remove it from the mesh,
	// delete it, create a new copy with the changes, and re-add it.
	// This will ensure the edges get updated appropriately.

protected:

	// don't use this constructor
	Triangle& operator =(const Triangle &t) {
		assert(0);
	}

	// ==============
	// REPRESENTATION
	Edge *edge;
	Vec3f normal;
	Edge *hypotenuseOpposite;
};

// ===========================================================

#endif
