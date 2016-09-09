#include "Quadtree.h"
#include "Object.h"

#include "Debug.h"

using namespace std;
using namespace directions;

void Quadtree::subdivide(Intercardinal point) {

	if (children[point]) {
		Error("Unsplitable quadtree!");
		return;
	}

	switch (point) {
	case Intercardinal::NW: {

		auto topMiddle = mesh->addVertex(
				(vertices[Intercardinal::NW]->get()
						+ vertices[Intercardinal::NE]->get()) * 0.5);

		auto leftMiddle = mesh->addVertex(
				(vertices[Intercardinal::NW]->get()
						+ vertices[Intercardinal::SW]->get()) * 0.5);

		mesh->removeTriangle(triangles[Cardinal::N]);

		mesh->removeTriangle(triangles[Cardinal::W]);

		triangles[Cardinal::N] = mesh->addTriangle(topMiddle,
				vertices[Intercardinal::NE], middle);

		triangles[Cardinal::W] = mesh->addTriangle(vertices[Intercardinal::SW],
				leftMiddle, middle);

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

		mesh->removeTriangle(triangles[Cardinal::N]);

		mesh->removeTriangle(triangles[Cardinal::E]);

		triangles[Cardinal::N] = mesh->addTriangle(vertices[Intercardinal::NW],
				topMiddle, middle);

		triangles[Cardinal::E] = mesh->addTriangle(rightMiddle,
				vertices[Intercardinal::SE], middle);

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

		mesh->removeTriangle(triangles[Cardinal::S]);

		mesh->removeTriangle(triangles[Cardinal::W]);

		triangles[Cardinal::S] = mesh->addTriangle(vertices[Intercardinal::SE],
				bottomMiddle, middle);

		triangles[Cardinal::W] = mesh->addTriangle(leftMiddle,
				vertices[Intercardinal::NW], middle);

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

		mesh->removeTriangle(triangles[Cardinal::S]);

		mesh->removeTriangle(triangles[Cardinal::E]);

		triangles[Cardinal::S] = mesh->addTriangle(bottomMiddle,
				vertices[Intercardinal::SW], middle);

		triangles[Cardinal::E] = mesh->addTriangle(vertices[Intercardinal::NE],
				rightMiddle, middle);

		children[3] = new Quadtree(middle, rightMiddle, bottomMiddle,
				vertices[3], level + 1, maxLevel, mesh, this);

		break;
	}

	}
}

void Quadtree::merge(directions::Intercardinal point) {
	if (!children[point]) {
		Error("Unmergeable quadtree!");
		return;
	}

	switch (point) {

	case Intercardinal::NW: {

		auto topMiddle = children[point]->getVertices()[Intercardinal::NE];
		auto leftMiddle = children[point]->getVertices()[Intercardinal::SW];

		delete children[point];

		children[point] = nullptr;

		mesh->removeTriangle(triangles[Cardinal::N]);
		mesh->removeTriangle(triangles[Cardinal::W]);

		triangles[Cardinal::N] = mesh->addTriangle(vertices[Intercardinal::NW],
				vertices[Intercardinal::NE], middle);
		triangles[Cardinal::W] = mesh->addTriangle(vertices[Intercardinal::SW],
				vertices[Intercardinal::NW], middle);

		mesh->removeVertex(topMiddle);
		mesh->removeVertex(leftMiddle);

		break;
	}

	case Intercardinal::NE: {

		auto topMiddle = children[point]->getVertices()[Intercardinal::NW];
		auto rightMiddle = children[point]->getVertices()[Intercardinal::SE];

		delete children[point];

		children[point] = nullptr;

		mesh->removeTriangle(triangles[Cardinal::N]);
		mesh->removeTriangle(triangles[Cardinal::E]);

		triangles[Cardinal::N] = mesh->addTriangle(vertices[Intercardinal::NW],
				vertices[Intercardinal::NE], middle);
		triangles[Cardinal::E] = mesh->addTriangle(vertices[Intercardinal::NE],
				vertices[Intercardinal::SE], middle);

		mesh->removeVertex(topMiddle);
		mesh->removeVertex(rightMiddle);

		break;
	}

	case Intercardinal::SW: {

		auto bottomMiddle = children[point]->getVertices()[Intercardinal::SE];
		auto leftMiddle = children[point]->getVertices()[Intercardinal::NW];

		delete children[point];

		children[point] = nullptr;

		mesh->removeTriangle(triangles[Cardinal::S]);
		mesh->removeTriangle(triangles[Cardinal::W]);

		triangles[Cardinal::S] = mesh->addTriangle(vertices[Intercardinal::SE],
				vertices[Intercardinal::SW], middle);
		triangles[Cardinal::W] = mesh->addTriangle(vertices[Intercardinal::SW],
				vertices[Intercardinal::NW], middle);

		mesh->removeVertex(bottomMiddle);
		mesh->removeVertex(leftMiddle);

		break;
	}

	case Intercardinal::SE: {

		auto bottomMiddle = children[point]->getVertices()[Intercardinal::SW];
		auto rightMiddle = children[point]->getVertices()[Intercardinal::NE];

		delete children[point];

		children[point] = nullptr;

		mesh->removeTriangle(triangles[Cardinal::S]);
		mesh->removeTriangle(triangles[Cardinal::E]);

		triangles[Cardinal::S] = mesh->addTriangle(vertices[Intercardinal::SE],
				vertices[Intercardinal::SW], middle);
		triangles[Cardinal::E] = mesh->addTriangle(vertices[Intercardinal::NE],
				vertices[Intercardinal::SE], middle);

		mesh->removeVertex(bottomMiddle);
		mesh->removeVertex(rightMiddle);

		break;
	}

	}
}

Quadtree::Quadtree(Vertex* nw, Vertex* ne, Vertex* sw, Vertex* se, int _level,
		int _maxLevel, Mesh * mesh, Quadtree* parent) :
		vertices { nw, ne, sw, se }, origin(sw->get()), xAxis(
				se->get() - sw->get()), yAxis(nw->get() - sw->get()), level(
				_level), middle(nullptr), maxLevel(_maxLevel), children {
				nullptr }, triangles { nullptr }, parent(parent), mesh(mesh) {

	auto m = Vec3f((nw->get().x() + ne->get().x()) * 0.5f,
			(nw->get().y() + sw->get().y()) * 0.5f, 0);

	middle = mesh->addVertex(m);

	triangles[0] = mesh->addTriangle(vertices[0], vertices[1], middle);
	triangles[1] = mesh->addTriangle(vertices[1], vertices[3], middle);
	triangles[2] = mesh->addTriangle(vertices[3], vertices[2], middle);
	triangles[3] = mesh->addTriangle(vertices[2], vertices[0], middle);
}

Quadtree::Quadtree(const Vec3f& _origin, const Vec3f& _xAxis,
		const Vec3f& _yAxis, int _level, int _maxLevel) :
		origin(_origin), xAxis(_xAxis), yAxis(_yAxis), level(_level), maxLevel(
				_maxLevel), children { nullptr }, vertices { nullptr }, triangles {
				nullptr }, parent(nullptr), middle(nullptr), mesh(new Mesh()) {

	if (level == maxLevel) {
		return;
	}

	auto sum = _xAxis + _yAxis;

	middle = mesh->addVertex((sum * 0.5) + _origin);

	vertices[0] = mesh->addVertex(_origin + _yAxis);
	vertices[1] = mesh->addVertex(_origin + sum);
	vertices[2] = mesh->addVertex(_origin);
	vertices[3] = mesh->addVertex(_origin + _xAxis);


	triangles[0] = mesh->addTriangle(vertices[0], vertices[1], middle);
	triangles[1] = mesh->addTriangle(vertices[1], vertices[3], middle);
	triangles[2] = mesh->addTriangle(vertices[3], vertices[2], middle);
	triangles[3] = mesh->addTriangle(vertices[2], vertices[0], middle);

	Log("level: %d", level);
}

Quadtree::~Quadtree() {

	for (auto& t : children) {
		if (t)
			delete t;
	}

	for (auto& t : triangles) {
		mesh->removeTriangle(t);
	}

	mesh->removeVertex(middle);

	if (!parent) {
		delete mesh;
	}
}
