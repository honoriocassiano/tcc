/*
 * BTTree.h
 *
 *  Created on: 20 de jul de 2016
 *      Author: cassiano
 */

#ifndef SRC_PATCH_H_
#define SRC_PATCH_H_

#include "BTTreeNode.h"
#include "structures/Halfedge/mesh.h"

#include <queue>
#include <vector>

constexpr size_t VARIANCE_DEPTH = 9;

typedef struct _Diamond Diamond;

struct NodeGreater {
	bool operator()(const BTTreeNode* lhs, const BTTreeNode* rhs) const;
};

struct DiamondLess {
	bool operator()(const Diamond &lhs, const Diamond &rhs) const;
};

class Patch {
public:
	Patch(float minDistance);
	virtual ~Patch();

	void merge(BTTreeNode* node);
	void split(BTTreeNode* node);

//	const Mesh* getMesh() const {
//		return mMesh;
//	}
	Mesh* getMesh() {
		return m_Mesh;
	}

	BTTreeNode* getLeft() {
		return m_LeftNode;
	}
	BTTreeNode* getRight() {
		return m_RightNode;
	}

	void computeVariance();
	void tessellate(const Vec3f& cameraPosition);

	void toggleWireframe();

	void render();

	void processGeometry();

private:
//	float recursiveComputeVariance(float* currentVariance, size_t index,
//			const Vec3f& left, float nLeft, const Vec3f& right, float nRight,
//			const Vec3f& apex, float nApex);

	float recursiveComputeVariance(BTTreeNode* currentNode, const Vec3f& left,
			float nLeft, const Vec3f& right, float nRight, const Vec3f& apex,
			float nApex);

	void recursiveTessellate(BTTreeNode* node, float* currentVariance,
			size_t index, const Vec3f& left, const Vec3f& right,
			const Vec3f& apex, const Vec3f& cameraPosition);

private:
	bool m_Wireframe;

	float m_MinDistance;

	Mesh* m_Mesh;

	BTTreeNode* m_LeftNode;
	BTTreeNode* m_RightNode;

	float m_LeftVariance[1 << VARIANCE_DEPTH];
	float m_RightVariance[1 << VARIANCE_DEPTH];

	std::priority_queue<BTTreeNode*, std::vector<BTTreeNode*>,
				struct NodeGreater> m_SplitQueue;


	std::priority_queue<Diamond, std::vector<Diamond>,
				struct DiamondLess> m_MergeQueue;

//	std::priority_queue<BTTreeNode*, std::vector<BTTreeNode*>,
//			struct NodeGreater> m_SplitQueue;
//
//	std::priority_queue<BTTreeNode*, std::vector<BTTreeNode*>,
//			struct NodeGreater> m_SplitQueueRight;
//
//	std::priority_queue<Diamond, std::vector<Diamond>, struct DiamondLess> m_MergeQueue;
//
//	std::priority_queue<Diamond, std::vector<Diamond>, struct DiamondLess> m_MergeQueueRight;
};

#endif /* SRC_PATCH_H_ */
