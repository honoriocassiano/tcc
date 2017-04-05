/*
 * WorldMesh.h
 *
 *  Created on: Apr 5, 2017
 *      Author: mint
 */

#ifndef SRC_WORLDMESH_H_
#define SRC_WORLDMESH_H_

#include "structures/Halfedge/mesh.h"

class WorldMesh : public Mesh {
public:
	WorldMesh(float radius);
	virtual ~WorldMesh();

protected:
	float radius;
};

#endif /* SRC_WORLDMESH_H_ */
