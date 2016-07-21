/*
 * BTTree.cpp
 *
 *  Created on: 20 de jul de 2016
 *      Author: cassiano
 */

#include "Patch.h"

#include "Perlin.h"
#include "MeshDrawer.h"
#include "structures/Halfedge/edge.h"

#include "Debug.h"

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
		mMesh(new Mesh()), mLeftNode(new BTTreeNode()), mRightNode(new BTTreeNode()), mLeftVariance{0.0f}, mRightVariance{0.0f} {

	mLeftNode->mBaseNeighbor = mRightNode;
	mRightNode->mBaseNeighbor = mLeftNode;

	//***********************************
	auto sw = mMesh->addVertex(Vec3f(0, 0, 0));
	auto nw = mMesh->addVertex(Vec3f(0, 5, 0));
	auto ne = mMesh->addVertex(Vec3f(5, 5, 0));
	auto se = mMesh->addVertex(Vec3f(5, 0, 0));
	//***********************************

	mLeftNode->mTriangle = mMesh->addTriangle(sw, ne, nw);
	mRightNode->mTriangle = mMesh->addTriangle(sw, se, ne);
}

Patch::~Patch() {
	delete mMesh;
	delete mLeftNode;
	delete mRightNode;
}

void Patch::split(BTTreeNode* node) {

	// We are already split, no need to do it again.
	if (node->mLeftChild) {
		return;
	}

	// If this triangle is not in a proper diamond, force split our base neighbor
	if (node->mBaseNeighbor && (node->mBaseNeighbor->mBaseNeighbor != node)) {
		split(node->mBaseNeighbor);
	}

	Vertex* mid = nullptr;

	Vertex* v0 = nullptr;
	Vertex* v1 = nullptr;
	Vertex* v2 = nullptr;

	Edge* hypOpposite = node->mTriangle->getHypotenuseOpposite();

	if (!hypOpposite) {
		Error("Not rectangle!\n");
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
	if (!node->mLeftChild) {
		return;
	}

	// Fill in the information we can get from the parent (neighbor pointers)
	node->mLeftChild->mBaseNeighbor = node->mLeftNeighbor;
	node->mLeftChild->mLeftNeighbor = node->mRightChild;

	node->mRightChild->mBaseNeighbor = node->mRightNeighbor;
	node->mRightChild->mRightNeighbor = node->mLeftChild;

	// Link our Left Neighbor to the new children
	if (node->mLeftNeighbor != nullptr) {
		if (node->mLeftNeighbor->mBaseNeighbor == node) {
			node->mLeftNeighbor->mBaseNeighbor = node->mLeftChild;

		} else if (node->mLeftNeighbor->mLeftNeighbor == node) {
			node->mLeftNeighbor->mLeftNeighbor = node->mLeftChild;

		} else if (node->mLeftNeighbor->mRightNeighbor == node) {
			node->mLeftNeighbor->mRightNeighbor = node->mLeftChild;

		} else {
			Error("Illegal Left Neighbor!\n");
		}
	}

	// Link our Right Neighbor to the new children
	if (node->mRightNeighbor != nullptr) {
		if (node->mRightNeighbor->mBaseNeighbor == node) {
			node->mRightNeighbor->mBaseNeighbor = node->mRightChild;

		} else if (node->mRightNeighbor->mRightNeighbor == node) {
			node->mRightNeighbor->mRightNeighbor = node->mRightChild;

		} else if (node->mRightNeighbor->mLeftNeighbor == node) {
			node->mRightNeighbor->mLeftNeighbor = node->mRightChild;

		} else {
			Error("Illegal Right Neighbor!\n");
		}
	}

	// Link our Base Neighbor to the new children
	if (node->mBaseNeighbor != nullptr) {

		if (node->mBaseNeighbor->mLeftChild) {

			node->mBaseNeighbor->mLeftChild->mRightNeighbor = node->mRightChild;
			node->mBaseNeighbor->mRightChild->mLeftNeighbor = node->mLeftChild;

			node->mLeftChild->mRightNeighbor = node->mBaseNeighbor->mRightChild;
			node->mRightChild->mLeftNeighbor = node->mBaseNeighbor->mLeftChild;
		} else {
			split(node->mBaseNeighbor); // Base Neighbor (in a diamond with us) was not split yet, so do that now.}
		}
	} else {
		// An edge triangle, trivial case.
		node->mLeftChild->mRightNeighbor = nullptr;
		node->mRightChild->mLeftNeighbor = nullptr;
	}
}

float Patch::recursiveComputeVariance(float* currentVariance, size_t index,
		const Vec3f& left, float nLeft, const Vec3f& right, float nRight,
		const Vec3f& apex, float nApex) {

	float variance = 0.0f;

	auto center = (left + right) * 0.5;

	variance = (nLeft + nRight) / 2;

	float nCenter = Perlin::generate(center.x(), center.y(), center.z());

	variance = fabsf(nCenter - variance);

	// TODO Check this condition: just x and y will be tested?
	if (fabsf(left.x() - right.x()) >= 0.1
			|| fabsf(left.y() - right.y()) > 0.1) {

		variance = max(variance,
				recursiveComputeVariance(currentVariance, LEFT_CHILD(index),
						apex, nApex, left, nLeft, center, nCenter));
		variance = max(variance,
				recursiveComputeVariance(currentVariance, RIGHT_CHILD(index),
						right, nRight, apex, nApex, center, nCenter));
	}

	//************************************
	//************************************
	//************************************
	//************************************
	if (index < (1 << VARIANCE_DEPTH)) {
		currentVariance[index] = variance;
	}
	//************************************
	//************************************
	//************************************
	//************************************

	return variance;
}

void Patch::computeVariance() {

	// Left node
	auto hypotenuseOpposite = mLeftNode->mTriangle->getHypotenuseOpposite();

	auto apex = hypotenuseOpposite->getVertex()->get();
	auto right = hypotenuseOpposite->getNext()->getVertex()->get();
	auto left = hypotenuseOpposite->getNext()->getNext()->getVertex()->get();

	recursiveComputeVariance(mLeftVariance, 0, left, Perlin::generate(left),
			right, Perlin::generate(right), apex, Perlin::generate(apex));

	// Right node
	hypotenuseOpposite = mRightNode->mTriangle->getHypotenuseOpposite();

	apex = hypotenuseOpposite->getVertex()->get();
	right = hypotenuseOpposite->getNext()->getVertex()->get();
	left = hypotenuseOpposite->getNext()->getNext()->getVertex()->get();

	apex.Set(apex.x(), apex.y(), Perlin::generate(apex));
	right.Set(right.x(), right.y(), Perlin::generate(right));
	left.Set(left.x(), left.y(), Perlin::generate(left));

	recursiveComputeVariance(mRightVariance, 0, left, Perlin::generate(left),
			right, Perlin::generate(right), apex, Perlin::generate(apex));
//	recursivelyComputeVariance(mRightNode);
}

void Patch::recursiveTessellate(BTTreeNode* node, float* currentVariance,
		size_t index, const Vec3f& left, const Vec3f& right, const Vec3f& apex,
		const Vec3f& cameraPosition) {

	float triVariance;

	auto center = (left + right) * 0.5;

	// TODO Implement this condition
	if (index < POW2(VARIANCE_DEPTH)) {
		float distance = (cameraPosition - center).Length();

		triVariance = ((float) currentVariance[index] * /* TODO Map size * */2)
				/ distance;

		Log("triVariance: %.5f\n", triVariance);
	}

	// TODO Implement this condition
	if (index >= POW2(VARIANCE_DEPTH) /* || (TriVariance > gFrameVariance) */) {
		split(node);

		// TODO Check this condition: just x and y will be tested?
		if (node->mLeftChild
				&& ((fabsf(left.x() - right.x()) >= 0.1)
						|| (fabsf(left.y() - right.y()) >= 0.1))) {

			recursiveTessellate(node->mLeftChild, currentVariance,
					LEFT_CHILD(index), apex, left, center, cameraPosition);
			recursiveTessellate(node->mRightChild, currentVariance,
					RIGHT_CHILD(index), right, apex, center, cameraPosition);
		}
	}
}

void Patch::tessellate(const Vec3f& cameraPosition) {
	auto hypotenuseOpposite = mLeftNode->mTriangle->getHypotenuseOpposite();

	auto apex = hypotenuseOpposite->getVertex()->get();
	auto right = hypotenuseOpposite->getNext()->getVertex()->get();
	auto left = hypotenuseOpposite->getNext()->getNext()->getVertex()->get();

//	recursiveComputeVariance(left, Perlin::generate(left), right,
//			Perlin::generate(right), apex, Perlin::generate(apex));
	recursiveTessellate(mLeftNode, mLeftVariance, 0, left, right, apex,
			cameraPosition);

	// Right node
	hypotenuseOpposite = mRightNode->mTriangle->getHypotenuseOpposite();

	apex = hypotenuseOpposite->getVertex()->get();
	right = hypotenuseOpposite->getNext()->getVertex()->get();
	left = hypotenuseOpposite->getNext()->getNext()->getVertex()->get();

	apex.Set(apex.x(), apex.y(), Perlin::generate(apex));
	right.Set(right.x(), right.y(), Perlin::generate(right));
	left.Set(left.x(), left.y(), Perlin::generate(left));

	recursiveTessellate(mRightNode, mRightVariance, 0, left, right, apex,
			cameraPosition);
//	recursiveComputeVariance(left, Perlin::generate(left), right,
//			Perlin::generate(right), apex, Perlin::generate(apex));
}

void Patch::render() {
	MeshDrawer::draw(mMesh, mWireframe);
}

void Patch::toggleWireframe() {
	mWireframe = !mWireframe;
}
