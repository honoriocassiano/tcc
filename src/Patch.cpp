/*
 * BTTree.cpp
 *
 *  Created on: 20 de jul de 2016
 *      Author: cassiano
 */

#include "Patch.h"

#include <stdio.h>

#include "structures/Halfedge/edge.h"

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

	// Create children and link into mesh
	//	//tri->mLeftChild = Landscape::AllocateTri();
	//	tri->mRightChild = Landscape::AllocateTri();

	//*****************************************************

	Edge* firstEdge = node->mTriangle->getEdge();
	Edge* currentEdge = firstEdge;

	Vertex* mid = nullptr;

	Vertex* v0 = nullptr;
	Vertex* v1 = nullptr;
	Vertex* v2 = nullptr;

	do {
		v0 = currentEdge->getVertex();
		v1 = currentEdge->getNext()->getVertex();
		v2 = currentEdge->getNext()->getNext()->getVertex();

		// Verifica se os vetores fazem um ângulo reto
		if ((v1->get() - v0->get()).Dot3(v2->get() - v1->get()) == 0.0) {

			// Cria um novo vetor a partir da hipotenusa
			mid = mMesh->addVertex((v2->get() + v0->get()) * 0.5);

			node->mLeftChild = new BTTreeNode();
			node->mRightChild = new BTTreeNode();

			mMesh->removeTriangle(node->mTriangle);

			node->mLeftChild->mTriangle = mMesh->addTriangle(mid, v1, v0);
			node->mRightChild->mTriangle = mMesh->addTriangle(mid, v2, v1);
			break;
		} else {
			currentEdge = currentEdge->getNext();
		}
	} while (currentEdge != firstEdge);

	if (!mid) {
		fprintf(stderr, "Not isoceles!\n");
		exit(-1);
	}



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
