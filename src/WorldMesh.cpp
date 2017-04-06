/*
 * WorldMesh.cpp
 *
 *  Created on: Apr 5, 2017
 *      Author: mint
 */

#include "WorldMesh.h"

#include "structures/Halfedge/vertex.h"
#include "structures/Halfedge/triangle.h"

#define SIZE(A) (sizeof(A) / sizeof(A[0]))

WorldMesh::WorldMesh(float _radius) :
		radius(_radius) {

	double t = (1.0 + sqrt(5.0)) / 2.0;

	Vec3f basePoints[] =
			{ { -1, t, 0 }, { 1, t, 0 }, { -1, -t, 0 }, { 1, -t, 0 },
					{ 0, -1, t }, { 0, 1, t }, { 0, -1, -t }, { 0, 1, -t }, { t,
							0, -1 }, { t, 0, 1 }, { -t, 0, -1 }, { -t, 0, 1 } };

	std::size_t baseIndices[] = { 0, 11, 5, 0, 5, 1, 0, 1, 7, 0, 7, 10, 0, 10,
			11, 1, 5, 9, 5, 11, 4, 11, 10, 2, 10, 7, 6, 7, 1, 8, 3, 9, 4, 3, 4,
			2, 3, 2, 6, 3, 6, 8, 3, 8, 9, 4, 9, 5, 2, 4, 11, 6, 2, 10, 8, 6, 7,
			9, 8, 1 };

//	std::size_t baseIndices[] = { { 0, 11, 5 }, { 0, 5, 1 }, { 0, 1, 7 }, { 0,
//			7, 10 }, { 0, 10, 11 }, { 1, 5, 9 }, { 5, 11, 4 }, { 11, 10, 2 }, {
//			10, 7, 6 }, { 10, 7, 6 }, { 7, 1, 8 }, { 3, 9, 4 }, { 3, 4, 2 }, {
//			3, 2, 6 }, { 3, 6, 8 }, { 3, 8, 9 }, { 4, 9, 5 }, { 2, 4, 11 }, { 6,
//			2, 10 }, { 8, 6, 7 }, { 9, 8, 1 } };

	Vertex* baseVertices[12] { nullptr };

	for (auto i = 0; i < SIZE(basePoints); ++i) {
		baseVertices[i] = addVertex(basePoints[i], 0);
	}

	for (auto i = 0; i < SIZE(baseIndices); i += 3) {
		addTriangle(baseVertices[baseIndices[i]],
				baseVertices[baseIndices[i + 1]],
				baseVertices[baseIndices[i + 2]]);
	}
}

WorldMesh::~WorldMesh() {

}

void WorldMesh::reset() {
//	triangles->DeleteAllElements();
//	edges->DeleteAllElements();

	for (auto& v : *vertices2) {
		v->setNormal(Vec3f(0.0f, 0.0f, 0.0f));
		v->setActive(false);
	}
}

//Vec3f computeNormal(const Vec3f &p1, const Vec3f &p2, const Vec3f &p3) {
//	Vec3f v12 = p2;
//	v12 -= p1;
//	Vec3f v23 = p3;
//	v23 -= p2;
//	Vec3f normal;
//	Vec3f::Cross3(normal, v12, v23);
//	normal.Normalize();
//	return normal;
//}

//void WorldMesh::updateNormals() {
//
//	auto it = triangles->StartIteration();
//
//	while (auto t = it->GetNext()) {
//		auto a = (*t)[0];
//		auto b = (*t)[1];
//		auto c = (*t)[2];
//
//		auto normal = computeNormal(a->get(), b->get(), c->get());
//
//		auto normalA = a->getNormal() + normal;
//		auto normalB = b->getNormal() + normal;
//		auto normalC = b->getNormal() + normal;
//
//		normalA.Normalize();
//		normalB.Normalize();
//		normalC.Normalize();
//
//		a->setNormal(normalA);
//		b->setNormal(normalB);
//		c->setNormal(normalC);
//
//		t->setNormal(normal);
//	}
//}

#undef SIZE

