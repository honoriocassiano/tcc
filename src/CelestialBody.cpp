/*
 * CelestialBody.cpp
 *
 *  Created on: 13 de abr de 2017
 *      Author: cassiano
 */

#include "CelestialBody.h"

#include "Perlin.h"

#define SIZE(A) (sizeof(A) / sizeof(A[0]))
#define EL(V) (Perlin::generateTurbulence(8, 1.35, 2, V->get() + Vec3f(1, 1, 1)) * 0.1f)

CelestialBody::CelestialBody(const Vec3f& _center, float _radius) :
		center(_center), radius(_radius), mass(0.0f), orbiter(nullptr), semiMajorAxis(
				0), baseVertices { nullptr }, transform(Matrix::identity()), axis {
				0, 1, 1 }, baseIndices { { 0, 11, 5 }, { 0, 5, 1 }, { 0, 1, 7 },
				{ 0, 7, 10 }, { 0, 10, 11 }, { 1, 5, 9 }, { 5, 11, 4 }, { 11,
						10, 2 }, { 10, 7, 6 }, { 7, 1, 8 }, { 3, 9, 4 }, { 3, 4,
						2 }, { 3, 2, 6 }, { 3, 6, 8 }, { 3, 8, 9 }, { 4, 9, 5 },
				{ 2, 4, 11 }, { 6, 2, 10 }, { 8, 6, 7 }, { 9, 8, 1 } } {

	float t = (1.0 + sqrt(5.0)) / 2.0;

	Vec3f basePoints[] =
			{ { -1, t, 0 }, { 1, t, 0 }, { -1, -t, 0 }, { 1, -t, 0 },
					{ 0, -1, t }, { 0, 1, t }, { 0, -1, -t }, { 0, 1, -t }, { t,
							0, -1 }, { t, 0, 1 }, { -t, 0, -1 }, { -t, 0, 1 } };

	for (auto& p : basePoints) {
		p.normalize();
	}

	for (auto i = 0; i < SIZE(basePoints); ++i) {

		baseVertices[i] = addVertex(basePoints[i], 0);

		float elevation = EL(baseVertices[i]);

		baseVertices[i]->setElevation(elevation);
		baseVertices[i]->setActive(true);

		setColor(baseVertices[i]);
	}

	for (const auto& idx : baseIndices) {
		addTriangle(baseVertices[idx[0]], baseVertices[idx[1]],
				baseVertices[idx[2]]);
	}

	transform *= Matrix::MakeTranslation(center) * Matrix::MakeScale(radius);
}

void CelestialBody::reset() {
	triangles->deleteAllElements();
	edges->deleteAllElements();

	for (auto& v : *vertices) {
		v->setNormal(Vec3f(0.0f, 0.0f, 0.0f));

		if (v->getLevel() > 0) {
			v->setActive(false);
		}
	}
}

void CelestialBody::recursiveUpdate(Vertex* v1, Vertex* v2, Vertex* v3,
		const Camera* camera, double size) {

	auto center = camera->getPosition();
	auto direction = camera->getDirection();

	double ratio_size = size; // default : 1
	double minsize = 0.01;    // default : 0.01

	Vertex* edge_center[3] = { getOrCreateChildVertex(v1, v2),
			getOrCreateChildVertex(v2, v3), getOrCreateChildVertex(v3, v1) };

	{
		// TODO Check this
		Vec3f tempCenters[3] = { edge_center[0]->get().normalized(),
				edge_center[1]->get().normalized(),
				edge_center[2]->get().normalized() };
//		Vec3f tempCenters[3] = { edge_center[0]->getReal().normalized(),
//				edge_center[1]->getReal().normalized(),
//				edge_center[2]->getReal().normalized() };

		for (auto i = 0; i < 3; ++i) {

			float elevation = EL(edge_center[i]);

			edge_center[i]->set(tempCenters[i]);
			edge_center[i]->setElevation(elevation);
			edge_center[i]->setActive(true);

			setColor(edge_center[i]);
		}
	}

	bool edge_test[3];
	double angle;

	for (auto i = 0; i < 3; ++i) {

		Vec3f d = (getVertexPositionWithTransform(edge_center[i]) - center);

		edge_test[i] = d.length() > ratio_size;
		d.normalize();
	}

	{
		Vec3f d = direction.normalized();

		double dot =
				computeNormal(edge_center[0]->getReal(),
						edge_center[1]->getReal(), edge_center[2]->getReal()).normalized().dot3(
						d);

		angle = std::acos(std::max(-1.0, std::min(dot, 1.0)));
	}

	// 45° degrees
	if (angle < (M_PI / 4)) {
		return;
	}

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

			auto n1 = p[i1]->get().normalized();
			auto n2 = p[i2]->get().normalized();
			auto n3 = p[i3]->get().normalized();

			p[i1]->setNormal(n1);
			p[i2]->setNormal(n2);
			p[i3]->setNormal(n3);

			recursiveUpdate(p[i1], p[i2], p[i3], camera, size / 2);
		}
	}
}

void CelestialBody::updateLOD(const Camera* camera) {

	reset();

	for (const auto& idx : baseIndices) {
		recursiveUpdate(baseVertices[idx[0]], baseVertices[idx[1]],
				baseVertices[idx[2]], camera, radius * 20);
	}

	deleteUnusedVertices();
}

void CelestialBody::deleteUnusedVertices() {

	for (auto it = vertices->rbegin(); it != vertices->rend(); ++it) {

		auto& v = *it;
		auto vp = v->getParents();

		if ((v->getLevel() > 0) && (!v->isActive())) {
			delete v;

			vertex_parents->remove(vp);
			vertices->remove(it.reverse());
		}
	}
}

void CelestialBody::rotate(float dTheta) {
	this->transform *= Matrix::MakeAxisRotation(Vec3f(0, 1, 0), dTheta);
}

Vertex* CelestialBody::getOrCreateChildVertex(Vertex* p1, Vertex* p2) {
	auto child = getChildVertex(p1, p2);

	if (!child) {
		child = addVertex((p1->get() + p2->get()) * 0.5,
				std::max(p1->getLevel(), p2->getLevel()) + 1);

		setParentsChild(p1, p2, child);

		child->setLevel(std::max(p1->getLevel(), p2->getLevel()) + 1);
	}

	return child;
}

Vec3f CelestialBody::getVertexPositionWithTransform(Vertex* v) {

	auto position = v->get() + (v->get() * v->getElevation());

	transform.Transform(position);

	return position;
}

void CelestialBody::setColor(Vertex* vertex) {
	auto el = vertex->getElevation();

	if (el < 0) {
		vertex->setColor(Color(0 / 255.0f, 102 / 255.0f, 0 / 255.0f));
	} else if (el < 0.5) {
		vertex->setColor(Color(102 / 255.0f, 102 / 255.0f, 51 / 255.0f));
	} else {
		vertex->setColor(Color(255 / 255.0f, 255 / 255.0f, 255 / 255.0f));
	}
}

#undef SIZE
#undef EL
