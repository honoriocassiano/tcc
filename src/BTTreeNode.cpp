/*
 * BTTreeNode.cpp
 *
 *  Created on: 20 de jul de 2016
 *      Author: cassiano
 */

#include "BTTreeNode.h"

BTTreeNode::BTTreeNode(Triangle* triangle) :
		mBaseNeighbor(nullptr), mRightChild(nullptr), mLeftChild(nullptr), mRightNeighbor(
				nullptr), mLeftNeighbor(nullptr), mTriangle(triangle) {

}

BTTreeNode::~BTTreeNode() {

}

