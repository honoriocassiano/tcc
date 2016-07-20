/*
 * BTTreeNode.h
 *
 *  Created on: 20 de jul de 2016
 *      Author: cassiano
 */

#ifndef SRC_STRUCTURES_BTTREENODE_H_
#define SRC_STRUCTURES_BTTREENODE_H_

#include "structures/Halfedge/triangle.h"

class Patch;

class BTTreeNode {
public:
	BTTreeNode(Triangle* triangle = nullptr);
	virtual ~BTTreeNode();

	BTTreeNode* getRightChild() { return mRightChild; }

private:
	BTTreeNode* mBaseNeighbor;

	BTTreeNode* mRightChild;
	BTTreeNode* mLeftChild;

	BTTreeNode* mRightNeighbor;
	BTTreeNode* mLeftNeighbor;

	Triangle * mTriangle;

	friend class Patch;
};

#endif /* SRC_STRUCTURES_BTTREENODE_H_ */
