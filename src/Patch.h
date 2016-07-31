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

constexpr size_t VARIANCE_DEPTH = 9;

class Patch {
public:
	Patch(float minDistance);
	virtual ~Patch();

	void merge(BTTreeNode* node);
	void split(BTTreeNode* node);

	const Mesh* getMesh() const {
		return mMesh;
	}

	void computeVariance();
	void tessellate(const Vec3f& cameraPosition);

	void toggleWireframe();

	void render();

private:
	float recursiveComputeVariance(float* currentVariance, size_t index,
			const Vec3f& left, float nLeft, const Vec3f& right, float nRight,
			const Vec3f& apex, float nApex);

//	float recursivelyComputeVariance(BTTreeNode* node);

	void recursiveTessellate(BTTreeNode* node, float* currentVariance, size_t index, const Vec3f& left,
			const Vec3f& right, const Vec3f& apex, const Vec3f& cameraPosition);

private:
	bool mWireframe;

	Mesh* mMesh;

	BTTreeNode* mLeftNode;
	BTTreeNode* mRightNode;

	float mLeftVariance[1 << VARIANCE_DEPTH];
	float mRightVariance[1 << VARIANCE_DEPTH];
};

#endif /* SRC_PATCH_H_ */
