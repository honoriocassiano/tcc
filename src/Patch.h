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

class Patch {
public:
	Patch();
	virtual ~Patch();

	void Split(BTTreeNode* node);

	Mesh* getMesh() { return mMesh; }

private:
	Mesh* mMesh;
};

#endif /* SRC_PATCH_H_ */
