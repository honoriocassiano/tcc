/*
 * WorldMesh.h
 *
 *  Created on: Apr 5, 2017
 *      Author: mint
 */

#ifndef SRC_WORLDMESH_H_
#define SRC_WORLDMESH_H_

#include "structures/Halfedge/mesh.h"

class WorldMesh: public Mesh {
public:
	WorldMesh(float radius);
	virtual ~WorldMesh();

	void reset() override;

	void recursiveUpdate(Vertex* v1, Vertex* v2, Vertex* v3,
			const Vec3f& center, double size = 1);

	void update(const Vec3f& position);

	Vertex* getOrCreateChildVertex(Vertex *p1, Vertex *p2);
//	void updateNormals();

protected:
	float radius;

	Vertex* baseVertices[12];

	std::size_t baseIndices[20][3];
};

#endif /* SRC_WORLDMESH_H_ */
