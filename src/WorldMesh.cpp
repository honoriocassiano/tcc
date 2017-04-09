/*
 * WorldMesh.cpp
 *
 *  Created on: Apr 5, 2017
 *      Author: mint
 */

#include "WorldMesh.h"

#include <cmath>
#include <algorithm>

#include "structures/Halfedge/vertex.h"
#include "structures/Halfedge/triangle.h"

#include "Perlin.h"

#define SIZE(A) (sizeof(A) / sizeof(A[0]))

#define EL(V) (Perlin::generateTurbulence(8, 1, 2, V->get() + Vec3f(1, 1, 1)) * 0.1f)

WorldMesh::WorldMesh(float _radius) :
		radius(_radius), baseVertices { nullptr }, baseIndices { { 0, 11, 5 }, {
				0, 5, 1 }, { 0, 1, 7 }, { 0, 7, 10 }, { 0, 10, 11 },
				{ 1, 5, 9 }, { 5, 11, 4 }, { 11, 10, 2 }, { 10, 7, 6 }, { 7, 1,
						8 }, { 3, 9, 4 }, { 3, 4, 2 }, { 3, 2, 6 }, { 3, 6, 8 },
				{ 3, 8, 9 }, { 4, 9, 5 }, { 2, 4, 11 }, { 6, 2, 10 },
				{ 8, 6, 7 }, { 9, 8, 1 } } {

	float t = (1.0 + sqrt(5.0)) / 2.0;

	Vec3f basePoints[] =
			{ { -1, t, 0 }, { 1, t, 0 }, { -1, -t, 0 }, { 1, -t, 0 },
					{ 0, -1, t }, { 0, 1, t }, { 0, -1, -t }, { 0, 1, -t }, { t,
							0, -1 }, { t, 0, 1 }, { -t, 0, -1 }, { -t, 0, 1 } };

	for (auto& p : basePoints) {
		p.Normalize();
	}

	for (auto i = 0; i < SIZE(basePoints); ++i) {
		baseVertices[i] = addVertex(basePoints[i], 0);

//		baseVertices[i]->setElevation(Perlin::generate(baseVertices[i]->get() + Vec3f(1, 1, 1)));
//		baseVertices[i]->setElevation(Perlin::generateTurbulence(8, 2, 2, baseVertices[i]->get() + Vec3f(1, 1, 1)));
		baseVertices[i]->setElevation(EL(baseVertices[i]));

		baseVertices[i]->setActive(true);

	}

	for (const auto& idx : baseIndices) {
		addTriangle(baseVertices[idx[0]], baseVertices[idx[1]],
				baseVertices[idx[2]]);
	}
}

WorldMesh::~WorldMesh() {

}

void WorldMesh::reset() {
	triangles->DeleteAllElements();
	edges->DeleteAllElements();

	for (auto& v : *vertices2) {
		v->setNormal(Vec3f(0.0f, 0.0f, 0.0f));

		if (v->getLevel() > 0) {
			v->setActive(false);
		}
	}
}

void WorldMesh::recursiveUpdate(Vertex* v1, Vertex* v2, Vertex* v3,
		const Vec3f& center, double size) {

	double ratio_size = size * 20; // default : 1
	double minsize = 0.01;    // default : 0.01

	Vertex* edge_center[3] = { getOrCreateChildVertex(v1, v2),
			getOrCreateChildVertex(v2, v3), getOrCreateChildVertex(v3, v1) };

	{
		Vec3f tempCenters[3] = { edge_center[0]->get(), edge_center[1]->get(),
				edge_center[2]->get() };

		for (auto i = 0; i < 3; ++i) {
			tempCenters[i].Normalize();
			edge_center[i]->set(tempCenters[i]);

//			edge_center[i]->setElevation(Perlin::generate(edge_center[i]->get() + Vec3f(1, 1, 1)));
//			edge_center[i]->setElevation(Perlin::generateTurbulence(8, 2, 2, edge_center[i]->get() + Vec3f(1, 1, 1)));

			edge_center[i]->setElevation(EL(edge_center[i]));

			edge_center[i]->setActive(true);
		}
	}

	bool edge_test[3];
	double angle[3];

	for (auto i = 0; i < 3; ++i) {

		Vec3f d = center + edge_center[i]->get();

		edge_test[i] = d.Length() > ratio_size;
		d.Normalize();

		double dot = edge_center[i]->get().Dot3(d);

		angle[i] = std::acos(std::max(-1.0, std::min(dot, 1.0)));
	}

//	if (*std::max_element(angle, angle + 3) < M_PI / 2 + 0.2) {
//		return;
//	}

	// draw
	if ((edge_test[0] && edge_test[1] && edge_test[2]) || size < minsize) {

		addTriangle(v1, v2, v3);

		return;
	}

	Vertex* p[6] =
			{ v1, v2, v3, edge_center[0], edge_center[1], edge_center[2] };
	int idx[4][3] = { { 0, 3, 5 }, { 5, 3, 4 }, { 3, 1, 4 }, { 5, 4, 2 } };
	bool valid[4] = { true, true, true, true };

	if (edge_test[0]) {
		p[3] = v1;
		valid[0] = false;
	} // skip triangle 0 ?
	if (edge_test[1]) {
		p[4] = v2;
		valid[2] = false;
	} // skip triangle 2 ?
	if (edge_test[2]) {
		p[5] = v3;
		valid[3] = false;
	} // skip triangle 3 ?

	for (auto i = 0; i < 4; ++i) {

		if (valid[i]) {
			int i1 = idx[i][0], i2 = idx[i][1], i3 = idx[i][2];

			auto n1 = p[i1]->get();
			auto n2 = p[i2]->get();
			auto n3 = p[i3]->get();

			n1.Normalize();
			n2.Normalize();
			n3.Normalize();

			p[i1]->setNormal(n1);
			p[i2]->setNormal(n2);
			p[i3]->setNormal(n3);

			recursiveUpdate(p[i1], p[i2], p[i3], center, size / 2);
		}
	}
}

void WorldMesh::update(const Vec3f& position) {

	reset();

	for (const auto& idx : baseIndices) {
		recursiveUpdate(baseVertices[idx[0]], baseVertices[idx[1]],
				baseVertices[idx[2]], position);
	}

	deleteUnusedVertices();
}

void WorldMesh::deleteUnusedVertices() {

	for (auto it = vertices2->rbegin(); it != vertices2->rend(); ++it) {

		auto& v = *it;
		auto vp = v->getParents1();

		if ((v->getLevel() > 0) && (!v->isActive())) {
			delete v;

			vertex_parents->Remove(vp);
			vertices2->remove(it.reverse());
		}
	}
}

Vertex* WorldMesh::getOrCreateChildVertex(Vertex* p1, Vertex* p2) {
	auto child = getChildVertex(p1, p2);

	if (!child) {
		child = addVertex((p1->get() + p2->get()) * 0.5,
				std::max(p1->getLevel(), p2->getLevel()) + 1);

		setParentsChild(p1, p2, child);

		child->setLevel(std::max(p1->getLevel(), p2->getLevel()) + 1);
	}

	return child;
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

