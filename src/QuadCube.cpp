/*
 * QuadCube.cpp
 *
 *  Created on: 9 de set de 2016
 *      Author: aluno
 */

#include "QuadCube.h"

#include "MeshDrawer.h"

QuadCube::QuadCube(const Vec3f& _position) :
		mesh(new QuadtreeMesh()), position(_position), faces { nullptr }, reference(
				directions::Face::FRONT) {

	using Face = directions::Face;

	// (FRONT/BACK) (LEFT/RIGHT) (TOP,BOTTOM)
	auto flt = mesh->addVertex(Vec3f(-0.5, 0.5, -0.5));
	auto frt = mesh->addVertex(Vec3f(0.5, 0.5, -0.5));
	auto flb = mesh->addVertex(Vec3f(-0.5, -0.5, -0.5));
	auto frb = mesh->addVertex(Vec3f(0.5, -0.5, -0.5));

	auto blt = mesh->addVertex(Vec3f(-0.5, 0.5, 0.5));
	auto brt = mesh->addVertex(Vec3f(0.5, 0.5, 0.5));
	auto blb = mesh->addVertex(Vec3f(-0.5, -0.5, 0.5));
	auto brb = mesh->addVertex(Vec3f(0.5, -0.5, 0.5));

	faces[Face::FRONT] = new Quadtree2(flt, frt, flb, frb, mesh);
	faces[Face::BACK] = new Quadtree2(blt, brt, blb, brb, mesh);

	faces[Face::LEFT] = new Quadtree2(blt, flt, blb, flb, mesh);
	faces[Face::RIGHT] = new Quadtree2(brt, frt, brb, frb, mesh);

	faces[Face::TOP] = new Quadtree2(blt, brt, flt, frt, mesh);
	faces[Face::BOTTOM] = new Quadtree2(blb, brb, flb, frb, mesh);

	initNeighbours();
}

QuadCube::~QuadCube() {
	for (auto& f : faces) {
		delete f;
	}

	delete mesh;
}

void QuadCube::draw(const DrawOptions& options) {
	MeshDrawer::draw(mesh, options);
}

void QuadCube::deleteUnusedVertices() {

	for (int i = mesh->getVertices()->Count() - 1; i >= 0; --i) {
		auto v = (*mesh->getVertices())[i];

		auto vp = v->getParents1();

		if (vp) {
			if (!(vp->getParent1()->isActive() && vp->getParent2()->isActive())) {
				mesh->deleteChildIfExist(vp->getParent1(), vp->getParent2());
			}
		}
	}
}

void QuadCube::update(const Vec3f& cameraPosition) {


	mesh->getTriangles()->DeleteAllElements();
	mesh->getEdges()->DeleteAllElements();


//	for (auto& f : faces) {
//		f->updateRoughness();
//	}

	for (auto& f : faces) {
		f->update2(cameraPosition);
	}

	deleteUnusedVertices();

}

void QuadCube::initNeighbours() {
	using Face = directions::Face;

	auto front = faces[Face::FRONT];
	auto left = faces[Face::LEFT];
	auto bottom = faces[Face::BOTTOM];

	auto back = faces[Face::BACK];
	auto right = faces[Face::RIGHT];
	auto top = faces[Face::TOP];

	// FRONT
	front->setNeighbours(top->getCenter(), right->getCenter(),
			bottom->getCenter(), left->getCenter());

	left->setNeighbours(top->getCenter(), back->getCenter(), front->getCenter(),
			bottom->getCenter());

	bottom->setNeighbours(back->getCenter(), right->getCenter(),
			left->getCenter(), front->getCenter());

	// INVERTED
	back->setNeighbours(bottom->getCenter(), left->getCenter(),
			top->getCenter(), right->getCenter());

	right->setNeighbours(front->getCenter(), bottom->getCenter(),
			top->getCenter(), back->getCenter());

	top->setNeighbours(left->getCenter(), front->getCenter(), back->getCenter(),
			right->getCenter());
}
