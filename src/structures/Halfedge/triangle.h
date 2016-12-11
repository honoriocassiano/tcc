#ifndef _TRIANGLE_H
#define _TRIANGLE_H

#include <sstream>
#include <string>
#include <limits.h>
#include "boundingbox.h"
#include "edge.h"

#include "vertex.h"

#include "Debug.h"

// ===========================================================

class Triangle {

public:

	// ========================
	// CONSTRUCTOR & DESTRUCTOR
	Triangle() {
		edge = nullptr;
		hypotenuse = nullptr;
		userData = nullptr;
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
	Edge* getHypotenuse() {
		if (!hypotenuse) {
			Edge* currentEdge = edge;

			Vertex* mid = nullptr;

			Vertex* v0 = nullptr;
			Vertex* v1 = nullptr;
			Vertex* v2 = nullptr;

			do {
				v0 = currentEdge->getVertex();
				v1 = currentEdge->getNext()->getVertex();
				v2 = currentEdge->getNext()->getNext()->getVertex();

				auto v01 = v0->get() - v1->get();
				auto v12 = v2->get() - v1->get();

				// TODO Check this comment
				// Check if the vectors are "ortoghonals"
				if (v01.Dot3(v12) == 0.0) {
					hypotenuse = currentEdge;

					break;
				} else {
					currentEdge = currentEdge->getNext();
				}
			} while (currentEdge != edge);
		}

		return hypotenuse;
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

		hypotenuse = nullptr;
	}

	const Vec3f& getNormal() {
		return normal;
	}
	void setNormal(Vec3f n) {
		normal = n;
	}

	void setUserData(void * data) {
		this->userData = data;
	}

	void * getUserData() {
		return this->userData;
	}

	const std::string str() const {
		std::stringstream ss;

		ss << (*this)[0]->get().str();
		ss << " ";
		ss << (*this)[1]->get().str();
		ss << " ";
		ss << (*this)[2]->get().str();
		ss << " ";

		return ss.str();
	}

	// NOTE: If you want to modify a triangle, remove it from the mesh,
	// delete it, create a new copy with the changes, and re-add it.
	// This will ensure the edges get updated appropriately.

	/*
	 float getOrientation(const Vec3f& viewPoint) {
	 //(B - A) x (C - A)

	 auto v0 = (*this)[0]->get();
	 auto v1 = (*this)[1]->get();
	 auto v2 = (*this)[2]->get();

	 Vec3f::Cross3(normal, v1 - v0, v2 - v0);


	 }
	 */

protected:

	// don't use this constructor
	Triangle& operator =(const Triangle &t) {
		assert(0);
	}

	// ==============
	// REPRESENTATION
	Edge *edge;
	Vec3f normal;
	Edge *hypotenuse;

	// Must be deleted manually
	void * userData;
};

// ===========================================================

#endif
