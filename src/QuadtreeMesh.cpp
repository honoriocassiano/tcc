/*
 * QuadtreeMesh.cpp
 *
 *  Created on: 11 de dez de 2016
 *      Author: cassiano
 */

#include "QuadtreeMesh.h"

#include <assert.h>

QuadtreeMesh::QuadtreeMesh() {


}

QuadtreeMesh::~QuadtreeMesh() {

}

Vertex* QuadtreeMesh::getOrCreateChildVertex(Vertex* p1, Vertex* p2) {

	auto child = getChildVertex(p1, p2);

	if(!child) {
		child = addVertex( (p1->get() + p2->get()) * 0.5 );

		setParentsChild(p1, p2, child);

		assert(p1->getLevel() == p2->getLevel());

		child->setLevel(p1->getLevel() + 1);
	}

	return child;
}
