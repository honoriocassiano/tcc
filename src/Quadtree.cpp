#include "Quadtree.h"
#include "Object.h"

#include "Debug.h"

using namespace std;

void Quadtree::subdivide(Intercardinal point) {
	switch (point) {
	case Intercardinal::NW: {

		auto topMiddle = mesh->addVertex(
				(vertices[Intercardinal::NW]->get()
						+ vertices[Intercardinal::NE]->get()) * 0.5);

		auto leftMiddle = mesh->addVertex(
				(vertices[Intercardinal::NW]->get()
						+ vertices[Intercardinal::SW]->get()) * 0.5);

		children[0] = new Quadtree(vertices[0], topMiddle, leftMiddle, middle,
				level + 1, maxLevel, mesh, this);

		break;
	}
	case Intercardinal::NE: {

		auto topMiddle = mesh->addVertex(
				(vertices[Intercardinal::NW]->get()
						+ vertices[Intercardinal::NE]->get()) * 0.5);

		auto rightMiddle = mesh->addVertex(
				(vertices[Intercardinal::NE]->get()
						+ vertices[Intercardinal::SE]->get()) * 0.5);

		children[1] = new Quadtree(topMiddle, vertices[1], middle, rightMiddle,
				level + 1, maxLevel, mesh, this);

		break;
	}
	case Intercardinal::SW: {

		auto bottomMiddle = mesh->addVertex(
				(vertices[Intercardinal::SW]->get()
						+ vertices[Intercardinal::SE]->get()) * 0.5);

		auto leftMiddle = mesh->addVertex(
				(vertices[Intercardinal::NW]->get()
						+ vertices[Intercardinal::SW]->get()) * 0.5);

		children[2] = new Quadtree(leftMiddle, middle, vertices[2],
				bottomMiddle, level + 1, maxLevel, mesh, this);

		break;
	}
	case Intercardinal::SE: {

		auto bottomMiddle = mesh->addVertex(
				(vertices[Intercardinal::SW]->get()
						+ vertices[Intercardinal::SE]->get()) * 0.5);

		auto rightMiddle = mesh->addVertex(
				(vertices[Intercardinal::NE]->get()
						+ vertices[Intercardinal::SE]->get()) * 0.5);

		children[3] = new Quadtree(middle, rightMiddle, bottomMiddle, vertices[3], level + 1, maxLevel, mesh, this);

		break;
	}

	}
}

Quadtree::Quadtree(Vertex* nw, Vertex* ne, Vertex* sw, Vertex* se, int _level,
		int _maxLevel, Mesh * mesh, Quadtree* parent) :
		vertices { nw, ne, sw, se }, x(ne->get().x()), y(nw->get().y()), width(
				ne->get().x() - nw->get().x()), height(
				sw->get().y() - se->get().y()), level(_level), middle(nullptr), maxLevel(
				_maxLevel), children { nullptr }, triangles { nullptr }, parent(
				parent), mesh(mesh) {

	auto m = Vec3f((nw->get().x() + ne->get().x()) * 0.5f,
			(nw->get().y() + sw->get().y()) * 0.5f, 0);

	middle = mesh->addVertex(m);
}

Quadtree::Quadtree(float _x, float _y, float _width, float _height, int _level,
		int _maxLevel) :
		x(_x), y(_y), width(_width), height(_height), level(_level), maxLevel(
				_maxLevel), children { nullptr }, vertices { nullptr }, triangles {
				nullptr }, parent(nullptr), middle(nullptr), mesh(new Mesh()) {
	if (level == maxLevel) {
		return;
	}

	const float halfWidth = width * 0.5f;
	const float halfHeight = height * 0.5f;

	middle = mesh->addVertex(Vec3f(x + halfWidth, y + halfHeight, 0));

	vertices[0] = mesh->addVertex(Vec3f(x, y + height, 0));
	vertices[1] = mesh->addVertex(Vec3f(x + width, y + height, 0));
	vertices[2] = mesh->addVertex(Vec3f(x, y, 0));
	vertices[3] = mesh->addVertex(Vec3f(x + width, y, 0));

	triangles[0] = mesh->addTriangle(vertices[0], vertices[1], middle);
	triangles[1] = mesh->addTriangle(vertices[1], vertices[2], middle);
	triangles[2] = mesh->addTriangle(vertices[2], vertices[3], middle);
	triangles[3] = mesh->addTriangle(vertices[3], vertices[0], middle);

	Log("level: %d", level);
}

Quadtree::~Quadtree() {

	for (auto& t : children) {
		if (t)
			delete t;
	}

	if (level != maxLevel) {
		mesh->removeVertex(middle);
	}

	if (!parent) {
		delete mesh;
	}
}
