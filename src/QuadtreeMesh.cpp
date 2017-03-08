/*
 * QuadtreeMesh.cpp
 *
 *  Created on: 11 de dez de 2016
 *      Author: cassiano
 */

#include "QuadtreeMesh.h"

#include <assert.h>
#include "Debug.h"

bool wasInited = false;

QuadtreeMesh::QuadtreeMesh() {

}

QuadtreeMesh::~QuadtreeMesh() {

}

Vertex* QuadtreeMesh::getOrCreateChildVertex(Vertex* p1, Vertex* p2) {

	auto child = getChildVertex(p1, p2);

	if (!child) {
		child = addVertex((p1->get() + p2->get()) * 0.5);

		setParentsChild(p1, p2, child);

		/*
		 if (p1->getLevel() != p2->getLevel()) {
		 Log("%ld %ld", p1->getLevel(), p2->getLevel());
		 Log("%s %s", p1->get().str().c_str(), p2->get().str().c_str());
		 assert(p1->getLevel() == p2->getLevel());
		 }
		 */

		child->setLevel(std::max(p1->getLevel(), p2->getLevel()) + 1);
	}

	return child;
}

bool QuadtreeMesh::deleteChildIfExist(Vertex* p1, Vertex* p2) {

	auto child = deleteParentsChildRelation(p1, p2);

	if (child) {
		removeVertex(child);

		return true;
	}

	return false;
}
