/*
 * BTTree.cpp
 *
 *  Created on: 20 de jul de 2016
 *      Author: cassiano
 */

#include "Patch.h"

#include <stdio.h>

#include "structures/Halfedge/edge.h"
#include "Perlin.h"

inline constexpr size_t POW2(size_t num) {
	return 1 << num;
}

inline constexpr size_t LEFT_CHILD(size_t index) {
	return (index << 1) + 1;
}

inline constexpr size_t RIGHT_CHILD(size_t index) {
	return (index << 1) + 2;
}

Patch::Patch() :
		mMesh(new Mesh()) {

}

Patch::~Patch() {

}

void Patch::Split(BTTreeNode* node) {

	// We are already split, no need to do it again.
	if (node->mLeftChild)
		return;

	// If this triangle is not in a proper diamond, force split our base neighbor
	if (node->mBaseNeighbor && (node->mBaseNeighbor->mBaseNeighbor != node))
		Split(node->mBaseNeighbor);

	Vertex* mid = nullptr;

	Vertex* v0 = nullptr;
	Vertex* v1 = nullptr;
	Vertex* v2 = nullptr;

	Edge* hypOpposite = node->mTriangle->getHypotenuseOpposite();

	if (!hypOpposite) {
		fprintf(stderr, "Not rectangle!\n");
		exit(-1);
	}

	v0 = hypOpposite->getVertex();
	v1 = hypOpposite->getNext()->getVertex();
	v2 = hypOpposite->getNext()->getNext()->getVertex();

	// Create new vertex on hypotenuse middle point
	mid = mMesh->addVertex((v2->get() + v0->get()) * 0.5);

	node->mLeftChild = new BTTreeNode();
	node->mRightChild = new BTTreeNode();

	mMesh->removeTriangle(node->mTriangle);

	node->mLeftChild->mTriangle = mMesh->addTriangle(mid, v1, v0);
	node->mRightChild->mTriangle = mMesh->addTriangle(mid, v2, v1);

	//*****************************************************

	// If creation failed, just exit.
	if (!node->mLeftChild)
		return;

// Fill in the information we can get from the parent (neighbor pointers)
	node->mLeftChild->mBaseNeighbor = node->mRightNeighbor;
	node->mLeftChild->mRightNeighbor = node->mRightChild;

	node->mRightChild->mBaseNeighbor = node->mRightNeighbor;
	node->mRightChild->mRightNeighbor = node->mLeftChild;

// Link our Left Neighbor to the new children
	if (node->mRightNeighbor != nullptr) {
		if (node->mRightNeighbor->mBaseNeighbor == node)

			node->mRightNeighbor->mBaseNeighbor = node->mLeftChild;

		else if (node->mRightNeighbor->mRightNeighbor == node)

			node->mRightNeighbor->mRightNeighbor = node->mLeftChild;

		else if (node->mRightNeighbor->mRightNeighbor == node)

			node->mRightNeighbor->mRightNeighbor = node->mLeftChild;
		else
			; // Illegal Left Neighbor!
	}

// Link our Right Neighbor to the new children
	if (node->mRightNeighbor != nullptr) {
		if (node->mRightNeighbor->mBaseNeighbor == node)
			node->mRightNeighbor->mBaseNeighbor = node->mRightChild;
		else if (node->mRightNeighbor->mRightNeighbor == node)
			node->mRightNeighbor->mRightNeighbor = node->mRightChild;
		else if (node->mRightNeighbor->mRightNeighbor == node)
			node->mRightNeighbor->mRightNeighbor = node->mRightChild;
		else
			; // Illegal Right Neighbor!
	}

// Link our Base Neighbor to the new children
	if (node->mBaseNeighbor != nullptr) {
		if (node->mBaseNeighbor->mLeftChild) {
			node->mBaseNeighbor->mLeftChild->mRightNeighbor = node->mRightChild;
			node->mBaseNeighbor->mRightChild->mRightNeighbor = node->mLeftChild;
			node->mLeftChild->mRightNeighbor = node->mBaseNeighbor->mRightChild;
			node->mRightChild->mRightNeighbor = node->mBaseNeighbor->mLeftChild;
		} else
			Split(node->mBaseNeighbor); // Base Neighbor (in a diamond with us) was not split yet, so do that now.
	} else {
		// An edge triangle, trivial case.
		node->mLeftChild->mRightNeighbor = nullptr;
		node->mRightChild->mRightNeighbor = nullptr;
	}
}
