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
		child = addVertex((p1->get() + p2->get()) * 0.5, std::max(p1->getLevel(), p2->getLevel()) + 1);

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

	if (child->get().Length() < 0.01) {
		int a = *((int*) 0x0);
	}

	return child;
}

bool QuadtreeMesh::deleteChildIfExist(Vertex* p1, Vertex* p2) {

	auto child = this->getChildVertex(p1, p2);

	if (child) {
		deleteParentsChildRelation(child->getParents1()->getParent1(),
				child->getParents1()->getParent2());

		if (child->getParents2()) {
			deleteParentsChildRelation(child->getParents2()->getParent1(),
					child->getParents2()->getParent2());
		}

		removeVertex(child);
		return true;
	}

	return false;
}

bool QuadtreeMesh::deleteVertex(MultiLevelArray<Vertex*>::Iterator& it) {
//	auto child = this->getChildVertex(p1, p2);
	auto child = *it;

	if (child) {
		deleteParentsChildRelation(child->getParents1()->getParent1(),
				child->getParents1()->getParent2());

		if (child->getParents2()) {
			deleteParentsChildRelation(child->getParents2()->getParent1(),
					child->getParents2()->getParent2());
		}

		delete *it;

		vertices2->remove(it);
		return true;
	}

	return false;
}
