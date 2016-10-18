/*
 * QuadCube.cpp
 *
 *  Created on: 9 de set de 2016
 *      Author: aluno
 */

#include "QuadCube.h"

#include "MeshDrawer.h"

QuadCube::QuadCube(const Vec3f& _position) :
		position(_position), faces { nullptr }, reference(
				directions::Face::FRONT) {

	using Face = directions::Face;

	faces[Face::FRONT] = new Quadtree(Vec3f(-0.5, -0.5, -0.5), Vec3f(1, 0, 0),
			Vec3f(0, 1, 0), 0, 20);
	faces[Face::BACK] = new Quadtree(Vec3f(0.5, 0.5, 0.5), Vec3f(-1, 0, 0),
			Vec3f(0, -1, 0), 0, 20);
	faces[Face::LEFT] = new Quadtree(Vec3f(-0.5, -0.5, -0.5), Vec3f(0, 0, 1),
			Vec3f(0, 1, 0), 0, 20);
	faces[Face::RIGHT] = new Quadtree(Vec3f(0.5, 0.5, 0.5), Vec3f(0, 0, -1),
			Vec3f(0, -1, 0), 0, 20);
	faces[Face::BOTTOM] = new Quadtree(Vec3f(-0.5, -0.5, -0.5), Vec3f(1, 0, 0),
			Vec3f(0, 0, 1), 0, 20);
	faces[Face::TOP] = new Quadtree(Vec3f(0.5, 0.5, 0.5), Vec3f(-1, 0, 0),
			Vec3f(0, 0, -1), 0, 20);

	initNeighbours();
}

QuadCube::~QuadCube() {
	for (auto& f : faces) {
		delete f;
	}
}

void QuadCube::draw(bool wireframe, bool generateNoise) {
	for (auto& m : faces) {
		MeshDrawer::draw(m->getMesh(), wireframe, generateNoise);
	}
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
	front->setNeighbours(top, right, bottom, left);

	left->setNeighbours(top, back, front, bottom);

	bottom->setNeighbours(back, right, left, front);

	// INVERTED
	back->setNeighbours(bottom, left, top, right);

	right->setNeighbours(front, bottom, top, back);

	top->setNeighbours(left, front, back, right);
}
