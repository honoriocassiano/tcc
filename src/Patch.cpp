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

#include "Utils.h"

#include <float.h>

inline constexpr size_t POW2(size_t num) {
	return 1 << num;
}

inline constexpr size_t LEFT_CHILD(size_t index) {
	return (index << 1) + 1;
}

inline constexpr size_t RIGHT_CHILD(size_t index) {
	return (index << 1) + 2;
}

typedef struct _Diamond {
	BTTreeNode* m_Node;
	BTTreeNode* m_Base;
	//bool m_PriorityOnNode;
	float m_Priority;

	//_Diamond(BTTreeNode* node, BTTreeNode* base, float priority) :
	_Diamond(BTTreeNode* node, BTTreeNode* base) :
			m_Node(node), m_Base(base), m_Priority(m_Node->getPriority()) {

		if (m_Base != nullptr
				&& m_Base->getPriority() > m_Node->getPriority()) {
			m_Priority = m_Base->getPriority();
		}
	}

	friend bool operator==(const Diamond& lhs, const Diamond& rhs) {
		return (lhs.m_Node == rhs.m_Node) && (lhs.m_Base == rhs.m_Base);
	}

} Diamond;

// true - after
// false - before
bool NodeGreater::operator()(const BTTreeNode* lhs,
		const BTTreeNode* rhs) const {
	return lhs->getPriority() > rhs->getPriority();
}

bool DiamondLess::operator()(const Diamond &lhs, const Diamond &rhs) const {
	return lhs.m_Priority < rhs.m_Priority;
}

Patch::Patch(float minDistance) :
		m_Wireframe(false), m_MinDistance(minDistance), m_CurrentMinDistance(
		FLT_MAX), m_Mesh(new Mesh()), m_LeftNode(new BTTreeNode()), m_RightNode(
				new BTTreeNode()), m_FrameVariance(50.0f), m_LeftVariance { 0.0f }, m_RightVariance {
				0.0f } {

	m_LeftNode->mBaseNeighbor = m_RightNode;
	m_RightNode->mBaseNeighbor = m_LeftNode;

	//***********************************
//	auto sw = m_Mesh->addVertex(Vec3f(0, 0, 0));
//	auto nw = m_Mesh->addVertex(Vec3f(0, 5, 0));
//	auto ne = m_Mesh->addVertex(Vec3f(5, 5, 0));
//	auto se = m_Mesh->addVertex(Vec3f(5, 0, 0));
	auto sw = m_Mesh->addVertex(Vec3f(0, 0, 0));
	auto nw = m_Mesh->addVertex(Vec3f(0, 1, 0));
	auto ne = m_Mesh->addVertex(Vec3f(1, 1, 0));
	auto se = m_Mesh->addVertex(Vec3f(1, 0, 0));
	//***********************************

//	auto nw = mMesh->addVertex(Vec3f(0, 100, 0));
//	auto ne = mMesh->addVertex(Vec3f(100, 100, 0));
//	auto se = mMesh->addVertex(Vec3f(100, 0, 0));

	m_LeftNode->mTriangle = m_Mesh->addTriangle(sw, ne, nw);
	m_RightNode->mTriangle = m_Mesh->addTriangle(sw, se, ne);
}

Patch::~Patch() {
	delete m_Mesh;
	delete m_LeftNode;
	delete m_RightNode;
}

void Patch::merge(BTTreeNode* node) {
	if (!node->mLeftChild) {
		return;
	}

	if (node->mBaseNeighbor && (node->mBaseNeighbor->mBaseNeighbor != node)) {
		merge(node->mBaseNeighbor);
	}

	auto extra = node->mLeftChild->mTriangle->getEdge();

	do {
		if (extra->getOpposite()
				&& extra->getOpposite()->getTriangle()
						== node->mRightChild->mTriangle) {
			break;
		} else {
			extra = extra->getNext();
		}
	} while (extra != node->mLeftChild->mTriangle->getEdge());

	Vertex* v0 = extra->getNext()->getVertex();
	Vertex* v1 = extra->getVertex();
	Vertex* v2 = extra->getOpposite()->getNext()->getVertex();

	Vertex* mid = m_Mesh->getChildVertex(v2, v0);

	if (mid == extra->getVertex()) {
		v1 = extra->getNext()->getNext()->getVertex();
	}

	// Delete children
	m_Mesh->removeTriangle(node->mLeftChild->mTriangle);
	m_Mesh->removeTriangle(node->mRightChild->mTriangle);

	node->mLeftNeighbor = node->mLeftChild->mBaseNeighbor;
	node->mRightNeighbor = node->mRightChild->mBaseNeighbor;

	node->mLeftChild->mTriangle = nullptr;
	node->mRightChild->mTriangle = nullptr;

	node->mTriangle = m_Mesh->addTriangle(v0, v1, v2);

	if (!node->mTriangle) {
		return;
	}

	delete node->mLeftChild;
	delete node->mRightChild;

	//**********************************************
	m_SplitQueue.remove(node->mLeftChild);
	m_SplitQueue.remove(node->mRightChild);
	//**********************************************

	node->mLeftChild = nullptr;
	node->mRightChild = nullptr;

	if (node->mBaseNeighbor->mBaseNeighbor == node) {
		// Base not merged
		if (node->mBaseNeighbor->mLeftChild) {
			merge(node->mBaseNeighbor);
		} else {
			// Remove parents-child relation
			m_Mesh->deleteParentsChildRelation(v0, v2);

			// Delete the middle vertex
			m_Mesh->removeVertex(mid);
		}
	}

	//**********************************************
	m_SplitQueue.push(node);
	//**********************************************
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

	Edge* hypOpposite = node->mTriangle->getHypotenuse();

	if (!hypOpposite) {
		Error("Not rectangle!");
		exit(-1);
	}

	v0 = hypOpposite->getVertex();
	v1 = hypOpposite->getNext()->getVertex();
	v2 = hypOpposite->getNext()->getNext()->getVertex();

	if ((mid = m_Mesh->getChildVertex(v2, v0)) == nullptr) {
		// Create new vertex on hypotenuse middle point
		mid = m_Mesh->addVertex((v2->get() + v0->get()) * 0.5);

		m_Mesh->setParentsChild(v2, v0, mid);
	}

	node->mLeftChild = new BTTreeNode();
	node->mRightChild = new BTTreeNode();

	m_Mesh->removeTriangle(node->mTriangle);

	node->mLeftChild->mTriangle = m_Mesh->addTriangle(mid, v1, v0);
	node->mRightChild->mTriangle = m_Mesh->addTriangle(mid, v2, v1);

	//*****************************************************

	// If creation failed, just exit.
	if (!node->mLeftChild) {
		return;
	}

	{
		float distance = min((v1->get() - v0->get()).Length(), (mid->get() - v0->get()).Length());

		if(distance < m_CurrentMinDistance) {
			m_CurrentMinDistance = distance;
		}
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
			Error("Illegal Left Neighbor!");
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
			Error("Illegal Right Neighbor!");
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

float Patch::recursiveComputeVariance(BTTreeNode* currentNode,
		const Vec3f& left, float nLeft, const Vec3f& right, float nRight,
		const Vec3f& apex, float nApex) {

	auto center = (left + right) * 0.5;

//	float nCenter = Perlin::generate(center.x(), center.y(), center.z());
	float nCenter = Perlin::generate(center.x(), center.y());

	float priority = fabsf(nCenter - ((nLeft + nRight) / 2));

	float mmax = 0;

	// TODO Check this condition: just x and y will be tested?
//	if (fabsf(left.x() - right.x()) >= 0.1
//			|| fabsf(left.y() - right.y()) > 0.1) {
	if (fabsf(left.x() - right.x()) > m_MinDistance
			|| fabsf(left.y() - right.y()) > m_MinDistance) {
		//|| fabsf(left.z() - right.z()) >= m_MinDistance) {

		BTTreeNode* leftChild = currentNode ? currentNode->mLeftChild : nullptr;
		BTTreeNode* rightChild =
				currentNode ? currentNode->mRightChild : nullptr;

		mmax = max(
				recursiveComputeVariance(leftChild, apex, nApex, left, nLeft,
						center, nCenter),
				recursiveComputeVariance(rightChild, right, nRight, apex, nApex,
						center, nCenter));

//		variance += max(
//				recursiveComputeVariance(currentVariance, LEFT_CHILD(index),
//						apex, nApex, left, nLeft, center, nCenter),
//				recursiveComputeVariance(currentVariance, RIGHT_CHILD(index),
//						right, nRight, apex, nApex, center, nCenter));
	}

	if (currentNode) {

		float mind = min(fabsf(left.x() - right.x()),
				fabsf(left.y() - right.y()));

		if (mind < m_CurrentMinDistance) {
			m_CurrentMinDistance = mind;
		}

		currentNode->setPriority(priority);

//		Log("center: %f, max: %f", nCenter, mmax);
		//Log("recursion: %d", (recursion / 2) + 1);
//		Log("priority: %f", priority);
	}

	return priority;
}

/*
 float Patch::recursiveComputeVariance(float* currentVariance, size_t index,
 const Vec3f& left, float nLeft, const Vec3f& right, float nRight,
 const Vec3f& apex, float nApex) {

 float variance = 0.0f;

 auto center = (left + right) * 0.5;

 //variance = (nLeft + nRight) / 2;

 float nCenter = Perlin::generate(center.x(), center.y(), center.z());

 float variance = fabsf(nCenter - variance);

 // TODO Check this condition: just x and y will be tested?
 //	if (fabsf(left.x() - right.x()) >= 0.1
 //			|| fabsf(left.y() - right.y()) > 0.1) {
 if (fabsf(left.x() - right.x()) >= mMinDistance
 || fabsf(left.y() - right.y()) >= mMinDistance
 || fabsf(left.z() - right.z()) >= mMinDistance) {

 variance += max(
 recursiveComputeVariance(currentVariance, LEFT_CHILD(index),
 apex, nApex, left, nLeft, center, nCenter),
 recursiveComputeVariance(currentVariance, RIGHT_CHILD(index),
 right, nRight, apex, nApex, center, nCenter));
 //		variance = max(variance,
 //				recursiveComputeVariance(currentVariance, LEFT_CHILD(index),
 //						apex, nApex, left, nLeft, center, nCenter));
 //		variance = max(variance,
 //				recursiveComputeVariance(currentVariance, RIGHT_CHILD(index),
 //						right, nRight, apex, nApex, center, nCenter));
 }

 //************************************
 //************************************
 //************************************
 //************************************
 //	if (index < (1 << VARIANCE_DEPTH)) {
 //		currentVariance[index] = 1 + variance;
 //	}
 //************************************
 //************************************
 //************************************
 //************************************

 return variance;
 }
 */

/*
 float Patch::recursivelyComputeVariance(BTTreeNode* node) {
 auto hypotenuse = node->mTriangle->getHypotenuse();

 auto apex = hypotenuse->getVertex()->get();
 auto right = hypotenuse->getNext()->getVertex()->get();
 auto left = hypotenuse->getNext()->getNext()->getVertex()->get();

 auto center = (left + right) * 0.5;

 float variance = fabs(
 Perlin::generate(center)
 - (Perlin::generate(left) + Perlin::generate(right) * 0.5));

 if (fabsf(left.x() - right.x()) > mMinDistance
 || fabsf(left.y() - right.y()) > mMinDistance
 || fabsf(left.z() - right.z()) > mMinDistance) {

 }

 return 0;
 }
 */

// TODO Fix it (x and y coordinates)
// Link: http://stackoverflow.com/questions/1560492/how-to-tell-whether-a-point-is-to-the-right-or-left-side-of-a-line
//bool isLeft(const Vec2f& line1, const Vec2f& line2, const Vec2f& point) {
//	return ((line2.x() - line1.x()) * (point.y() - line1.y())
//			- (line2.y() - line1.y()) * (point.x() - line1.x())) > 0;
//}
bool isLeft(const Vec2f& reference, const Vec2f& vec) {
	auto dot = reference.x() * (-vec.y()) + reference.y() * vec.x();

	return dot < 0;
}

void Patch::computeVariance() {

	// Left node
	auto hypotenuse = m_LeftNode->mTriangle->getHypotenuse();

//	auto apex = hypotenuseOpposite->getVertex()->get();
//	auto right = hypotenuseOpposite->getNext()->getVertex()->get();
//	auto left = hypotenuseOpposite->getNext()->getNext()->getVertex()->get();

	auto apex = hypotenuse->getNext()->getVertex();
	auto right = hypotenuse->getVertex();
	auto left = hypotenuse->getNext()->getNext()->getVertex();

	auto apexPosition = apex->get();
	auto rightPosition = right->get();
	auto leftPosition = left->get();

	auto mid = (leftPosition + rightPosition) * 0.5;

	// TODO Fix it: x and y coordinates
	// Check if right is on left side

	if (isLeft(Vec2f(apexPosition.x() - mid.x(), apexPosition.y() - mid.y()),
			Vec2f(rightPosition.x() - mid.x(), rightPosition.y() - mid.y()))) {

		swap(left, right);

		rightPosition = right->get();
		leftPosition = left->get();
	}

//	recursiveComputeVariance(mLeftVariance, 0, left, Perlin::generate(left),
//			right, Perlin::generate(right), apex, Perlin::generate(apex));

//	recursiveComputeVariance(mLeftNode, left, Perlin::generate(left), right,
//			Perlin::generate(right), apex, Perlin::generate(apex));

	Log("LEFT!");

	recursiveComputeVariance(m_LeftNode, leftPosition,
			Perlin::generate(leftPosition.x(), leftPosition.y()), rightPosition,
			Perlin::generate(rightPosition.x(), rightPosition.y()),
			apexPosition, Perlin::generate(apexPosition.x(), apexPosition.y()));

	// Right node
//	hypotenuse = mRightNode->mTriangle->getHypotenuse();
//
//	apexPosition = hypotenuse->getVertex()->get();
//	rightPosition = hypotenuse->getNext()->getVertex()->get();
//	leftPosition = hypotenuse->getNext()->getNext()->getVertex()->get();

//	Log("HERE! %p, %s\n", hypotenuse, hypotenuse->getVertex()->get().str().c_str());

	hypotenuse = m_RightNode->mTriangle->getHypotenuse();

	apex = hypotenuse->getNext()->getVertex();

	// Invert left and right
	swap(right, left);

	apexPosition = apex->get();
	rightPosition = right->get();
	leftPosition = left->get();

	// TODO Set Vertex position here
//	apex.Set(apex.x(), apex.y(), Perlin::generate(apex));
//	right.Set(right.x(), right.y(), Perlin::generate(right));
//	left.Set(left.x(), left.y(), Perlin::generate(left));

//	recursiveComputeVariance(mRightVariance, 0, left, Perlin::generate(left),
//			right, Perlin::generate(right), apex, Perlin::generate(apex));

//	recursiveComputeVariance(mRightNode, left, Perlin::generate(left), right,
//			Perlin::generate(right), apex, Perlin::generate(apex));

	Log("RIGHT!");

	recursiveComputeVariance(m_RightNode, leftPosition,
			Perlin::generate(leftPosition.x(), leftPosition.y()), rightPosition,
			Perlin::generate(rightPosition.x(), rightPosition.y()),
			apexPosition, Perlin::generate(apexPosition.x(), apexPosition.y()));

	//********************************************************
	// Check if is the first time
	/*
	 if (m_SplitQueue.size() == 0) {
	 m_SplitQueue.push(m_LeftNode);

	 m_MergeQueue.push(
	 Diamond(m_LeftNode, m_RightNode,
	 max(m_LeftNode->getPriority(),
	 m_RightNode->getPriority())));
	 }
	 */
	//********************************************************
}

void Patch::processGeometry(const Vec3f& cameraPosition,
		const Vec3f& cameraDirection) {

	if (m_SplitQueue.size() == 0) {
		m_SplitQueue.push(m_LeftNode);
	}

	m_CurrentMinDistance = FLT_MAX;
	computeVariance();

	auto distance = (Vec3f(0, 0, 0) - cameraPosition).Length() / 2.0f;

	if (m_Mesh->numVertices() != DESIRED_TRIANGLES) {
		m_FrameVariance += (m_Mesh->numVertices() - DESIRED_TRIANGLES)
				/ (float) DESIRED_TRIANGLES;

		if (m_FrameVariance < 0) {
			m_FrameVariance = 0;
		}
	}

	auto mergePriority =
			m_MergeQueue.size() > 0 ? m_MergeQueue.top().m_Priority : 0;

	//while (/* TODO First size/accuracy condition here || */m_SplitQueue.top()->getPriority()
	while (	//(m_Mesh->numVertices() != DESIRED_TRIANGLES)||
	/* TODO Check this condition */
	((m_CurrentMinDistance > m_MinDistance)
			&& (distance != m_CurrentMinDistance))
			|| (m_SplitQueue.top()->getPriority() > mergePriority)) {
		// Condition to merge
		//if (/* TODO Accuracy condition here */false) {
		//if(m_SplitQueue.top()->getPriority() > 1.0) {

		Log("BBB: %d, %f",
				(m_CurrentMinDistance > m_MinDistance), m_CurrentMinDistance);
						//&& (distance != m_CurrentMinDistance));

		if (distance > m_CurrentMinDistance) {
			/*
			 auto lower = m_MergeQueue.top();

			 merge(lower.m_Node);

			 // Update queues
			 // Remove children
			 m_SplitQueue.pop();
			 m_SplitQueue.pop();

			 m_MergeQueue.pop();
			 */
			Log("AAA");

			if (m_MergeQueue.size() > 0) {

				Diamond lower = m_MergeQueue.top();

				m_MergeQueue.pop();

				merge(lower.m_Base);
			} else {
				break;
			}

			// TODO See algorithm

		} else {
			Log("AAA");

			BTTreeNode* higher = m_SplitQueue.top();

			m_SplitQueue.pop();

			split(higher);

			// Add children to split queue
			m_SplitQueue.push(higher->mLeftChild);
			m_SplitQueue.push(higher->mRightChild);

			// Add base children to split queue
			if (higher->mBaseNeighbor) {
				m_SplitQueue.push(higher->mBaseNeighbor->mLeftChild);
				m_SplitQueue.push(higher->mBaseNeighbor->mRightChild);
			}

			// Add diamond to merge queue
			m_MergeQueue.push(Diamond(higher, higher->mBaseNeighbor));

		}
	}
}

void Patch::recursiveTessellate(BTTreeNode* node, float* currentVariance,
		size_t index, const Vec3f& left, const Vec3f& right, const Vec3f& apex,
		const Vec3f& cameraPosition) {

	float triVariance;

	auto center = (left + right) * 0.5;

	// TODO Implement this condition
	if (index < POW2(VARIANCE_DEPTH)) {
		float distance = (cameraPosition - center).Length();

		// TODO Check this value (mapSize: 5)
		triVariance = ((float) currentVariance[index] * 5 * 2) / distance;

//		Log("%s triVariance: %.5f\n", (node == mLeftNode ? "LEFT" : "RIGHT"),
//				triVariance);
	}

	// TODO Implement this condition
	if (index >= POW2(VARIANCE_DEPTH) || (triVariance > frameVariance)) {
		split(node);

		// TODO Check this condition: just x and y will be tested?
		if (node->mLeftChild
				&& ((fabsf(left.x() - right.x()) >= m_MinDistance)
						|| (fabsf(left.y() - right.y()) >= m_MinDistance))) {
			recursiveTessellate(node->mLeftChild, currentVariance,
					LEFT_CHILD(index), apex, left, center, cameraPosition);
			recursiveTessellate(node->mRightChild, currentVariance,
					RIGHT_CHILD(index), right, apex, center, cameraPosition);
		}
	}

//	Log("triVariance: %.5f, frameVariance: %.5f\n", triVariance, frameVariance);
}

void Patch::tessellate(const Vec3f& cameraPosition) {
	auto hypotenuse = m_LeftNode->mTriangle->getHypotenuse();

	auto apex = hypotenuse->getNext()->getVertex()->get();
	auto right = hypotenuse->getNext()->getNext()->getVertex()->get();
	auto left = hypotenuse->getVertex()->get();
	//auto left = hypotenuse->getNext()->getNext()->getVertex()->get();

//	recursiveComputeVariance(left, Perlin::generate(left), right,
//			Perlin::generate(right), apex, Perlin::generate(apex));
	recursiveTessellate(m_LeftNode, m_LeftVariance, 0, left, right, apex,
			cameraPosition);

	// Right node
	hypotenuse = m_RightNode->mTriangle->getHypotenuse();

	apex = hypotenuse->getNext()->getVertex()->get();
	right = hypotenuse->getNext()->getNext()->getVertex()->get();
	left = hypotenuse->getVertex()->get();

	apex.Set(apex.x(), apex.y(), Perlin::generate(apex));
	right.Set(right.x(), right.y(), Perlin::generate(right));
	left.Set(left.x(), left.y(), Perlin::generate(left));

	recursiveTessellate(m_RightNode, m_RightVariance, 0, left, right, apex,
			cameraPosition);
//	recursiveComputeVariance(left, Perlin::generate(left), right,
//			Perlin::generate(right), apex, Perlin::generate(apex));
}

void Patch::render() {
	MeshDrawer::draw(m_Mesh, m_Wireframe);
}

void Patch::toggleWireframe() {
	m_Wireframe = !m_Wireframe;
}

