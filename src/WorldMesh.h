/*
 * WorldMesh.h
 *
 *  Created on: Apr 5, 2017
 *      Author: mint
 */

#ifndef SRC_WORLDMESH_H_
#define SRC_WORLDMESH_H_

#include "structures/Halfedge/mesh.h"
#include "structures/matrix.h"

class WorldMesh: public Mesh {
public:
	WorldMesh(float radius, const Vec3f& center = Vec3f());
	virtual ~WorldMesh();

	void reset() override;

	void recursiveUpdate(Vertex* v1, Vertex* v2, Vertex* v3,
			const Vec3f& center, double size = 1);

	void update(const Vec3f& position);

	void rotate(float dTheta);

	void deleteUnusedVertices();

	Vertex* getOrCreateChildVertex(Vertex *p1, Vertex *p2);
//	void updateNormals();

	Matrix getTransform() override {
		return transform;
	}

protected:

	virtual Vec3f getVertexPositionWithTransform(Vertex* v);

	float radius;

	Vec3f axis;

	Vec3f center;

	Vertex* baseVertices[12];

	std::size_t baseIndices[20][3];

	Matrix transform;
};

#endif /* SRC_WORLDMESH_H_ */
