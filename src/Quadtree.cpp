#include "Quadtree.h"
#include "Object.h"

#include "Debug.h"

#include <algorithm>

using namespace std;
using namespace directions;

void Quadtree::subdivide(Intercardinal point, const std::string& tag,
		bool propagate) {

#define INVERT(S) (static_cast<QuadtreeData::Side>(1 - static_cast<int>( S )))

	using namespace directions;

	if (children[point]) {
		Error("\"Unsplitable\" quadtree!");
		return;
	}

	Edge* edge1 = nullptr;
	Edge* edge2 = nullptr;

	QuadtreeData::Side sideTriangle1 = QuadtreeData::Side::LEFT;
	QuadtreeData::Side sideTriangle2 = QuadtreeData::Side::LEFT;

	switch (point) {
	case Intercardinal::NW: {

		edge1 = mesh->getEdge(vertices[NW], vertices[NE]);
		edge2 = mesh->getEdge(vertices[SW], vertices[NW]);

		if (!edge1 || (edge1 && (edge1->getTriangle() != triangles[N]))) {
			edge1 = mesh->getEdge(vertices[NE], vertices[NW]);
		}

		if (!edge2 || ((edge2 && (edge2->getTriangle() != triangles[W])))) {
			edge2 = mesh->getEdge(vertices[NW], vertices[SW]);
		}

		break;
	}
	case Intercardinal::NE: {

//		auto t1 = triangles[directions::N];
//		auto t2 = triangles[directions::E];

		edge1 = mesh->getEdge(vertices[NW], vertices[NE]);
		edge2 = mesh->getEdge(vertices[SE], vertices[NE]);

		if (!edge1 || (edge1 && (edge1->getTriangle() != triangles[N]))) {
			edge1 = mesh->getEdge(vertices[NE], vertices[NW]);
		}

		if (!edge2 || (edge2 && (edge2->getTriangle() != triangles[E]))) {
			edge2 = mesh->getEdge(vertices[NE], vertices[SE]);
		}

		break;
	}
	case Intercardinal::SW: {

//		auto t1 = triangles[directions::S];
//		auto t2 = triangles[directions::W];

		edge1 = mesh->getEdge(vertices[SW], vertices[SE]);
		edge2 = mesh->getEdge(vertices[SW], vertices[NW]);

		if (!edge1 || (edge1 && (edge1->getTriangle() != triangles[S]))) {
			edge1 = mesh->getEdge(vertices[SE], vertices[SW]);
		}

		if (!edge2 || (edge2 && (edge2->getTriangle() != triangles[W]))) {
			edge2 = mesh->getEdge(vertices[NW], vertices[SW]);
		}

		break;
	}
	case Intercardinal::SE: {

//		auto t1 = triangles[directions::S];
//		auto t2 = triangles[directions::E];

		edge1 = mesh->getEdge(vertices[SW], vertices[SE]);
		edge2 = mesh->getEdge(vertices[SE], vertices[NE]);

		if (!edge1 || (edge1 && (edge1->getTriangle() != triangles[S]))) {
			edge1 = mesh->getEdge(vertices[SE], vertices[SW]);
		}

		if (!edge2 || (edge2 && (edge2->getTriangle() != triangles[E]))) {
			edge2 = mesh->getEdge(vertices[NE], vertices[SE]);
		}

		break;
	}

	}

	{
		auto tempS1 =
				static_cast<QuadtreeData*>(edge1->getTriangle()->getUserData());
		auto tempS2 =
				static_cast<QuadtreeData*>(edge2->getTriangle()->getUserData());

		if (tempS1->data[sideTriangle1] != point) {
			sideTriangle1 = INVERT(sideTriangle1);
		}

		if (tempS2->data[sideTriangle2] != point) {
			sideTriangle2 = INVERT(sideTriangle2);
		}
	}

	Triangle* neighborTriangle1 =
			(edge1->getOpposite()) ?
					edge1->getOpposite()->getTriangle() : nullptr;
	Triangle* neighborTriangle2 =
			(edge2->getOpposite()) ?
					edge2->getOpposite()->getTriangle() : nullptr;

	Quadtree* neighborTree1 =
			edge1->getOpposite() ?
					static_cast<QuadtreeData*>(neighborTriangle1->getUserData())->tree :
					nullptr;

	Quadtree* neighborTree2 =
			edge2->getOpposite() ?
					static_cast<QuadtreeData*>(neighborTriangle2->getUserData())->tree :
					nullptr;

	QuadtreeData::Side neighborSideTriangle1 = INVERT(sideTriangle1);
	QuadtreeData::Side neighborSideTriangle2 = INVERT(sideTriangle2);

	Vertex * v11 = edge1->getVertex();
	Vertex * v12 = edge1->getNext()->getNext()->getVertex();

	Vertex* v21 = edge2->getVertex();
	Vertex* v22 = edge2->getNext()->getNext()->getVertex();

	Vertex * middlePoint1 = mesh->getChildVertex(v11, v12);
	Vertex * middlePoint2 = mesh->getChildVertex(v21, v22);

	if (!middlePoint1) {
		auto middlePos = (v11->get() + v12->get()) * 0.5;

		middlePoint1 = mesh->addVertex(middlePos);

		mesh->setParentsChild(v11, v12, middlePoint1);
	}

	if (!middlePoint2) {
		auto middlePos = (v21->get() + v22->get()) * 0.5;

		middlePoint2 = mesh->addVertex(middlePos);

		mesh->setParentsChild(v21, v22, middlePoint2);
	}

	int positionT1 = findPosition(edge1->getTriangle());
	int positionT2 = findPosition(edge2->getTriangle());

	auto userDataTriangle1 = triangles[positionT1]->getUserData();
	auto userDataTriangle2 = triangles[positionT2]->getUserData();

	mesh->removeTriangle(triangles[positionT1]);
	triangles[positionT1] = nullptr;

	mesh->removeTriangle(triangles[positionT2]);
	triangles[positionT2] = nullptr;

	if (!children[static_cast<QuadtreeData*>(edge1->getTriangle()->getUserData())->data[INVERT(
			sideTriangle1)]]) {

		triangles[positionT1] =
				mesh->addTriangle(
						vertices[static_cast<QuadtreeData*>(userDataTriangle1)->data[INVERT(
								sideTriangle1)]], middlePoint1, middle);

//		triangles[positionT1] = mesh->addTriangle(
//				middlePoint1, vertices[static_cast<QuadtreeData*>(userDataTriangle1)->data[INVERT(sideTriangle1)]],
//						middle);

		triangles[positionT1]->setUserData(userDataTriangle1);
	}

	if (!children[static_cast<QuadtreeData*>(edge2->getTriangle()->getUserData())->data[INVERT(
			sideTriangle2)]]) {

		triangles[positionT2] =
				mesh->addTriangle(
						vertices[static_cast<QuadtreeData*>(userDataTriangle2)->data[INVERT(
								sideTriangle2)]], middlePoint2, middle);

		triangles[positionT2]->setUserData(userDataTriangle2);
	}

	Log("C");

	// Subdivide neighbours
	if (propagate) {

		directions::Intercardinal neighborSide1 =
				static_cast<QuadtreeData*>(neighborTriangle1->getUserData())->data[INVERT(
						sideTriangle1)];

		directions::Intercardinal neighborSide2 =
				static_cast<QuadtreeData*>(neighborTriangle2->getUserData())->data[INVERT(
						sideTriangle2)];

		if (neighborTree1 && !neighborTree1->getChild(neighborSide1)) {
			neighborTree1->subdivide(neighborSide1, "neighbour 1", false);
		}

		if (neighborTree2 && !neighborTree2->getChild(neighborSide2)) {
			neighborTree2->subdivide(neighborSide2, "neighbour 2", false);
		}

	} else {
		// todo check this
	}

	// TODO Check this 'this'

	children[point] = new Quadtree(middlePoint1, vertices[point], middle,
			middlePoint2, level + 1, maxLevel, mesh, this);

//	children[point] = new Quadtree(vertices[point], middlePoint1, middle, middlePoint2,
//			level + 1, maxLevel, mesh, this);

	if (propagate) {

		auto temp1 =
				static_cast<QuadtreeData*>(userDataTriangle1)->data[INVERT(
						sideTriangle1)];
		auto temp2 =
				static_cast<QuadtreeData*>(userDataTriangle2)->data[INVERT(
						sideTriangle2)];

		if (!children[temp1]) {
			subdivide(temp1, "children 1", false);
		}

		if (!children[temp2]) {
			subdivide(temp2, "children 2", false);
		}
	}

	Log("%s A", tag.c_str());

#undef INVERT

	/*
	 if (propagate) {
	 if (n && !n->getChild(Intercardinal::SW)) {
	 n->subdivide(Intercardinal::SW, false);
	 n = n->getChild(Intercardinal::SW);
	 }

	 if (w && !w->getChild(Intercardinal::NE)) {
	 w->subdivide(Intercardinal::NE, false);
	 w = w->getChild(Intercardinal::NE);
	 }
	 } else {
	 n = n ? n->getChild(Intercardinal::SE) : nullptr;
	 w = w ? w->getChild(Intercardinal::NE) : nullptr;
	 }

	 children[Intercardinal::NW] = new Quadtree(vertices[0], topMiddle,
	 leftMiddle, middle, level + 1, maxLevel, mesh, this);

	 if (propagate) {
	 if (!children[Intercardinal::NE]) {
	 subdivide(Intercardinal::NE, false);
	 }

	 if (!children[Intercardinal::SW]) {
	 subdivide(Intercardinal::SW, false);
	 }
	 }

	 children[0]->setNeighbours(n, children[Intercardinal::NE],
	 children[Intercardinal::SW], w);
	 */

	/*
	 switch (point) {
	 case Intercardinal::NW: {

	 Vertex* topMiddle = nullptr;
	 Vertex* leftMiddle = nullptr;

	 // Check if top child vertex exists
	 if (!(topMiddle = mesh->getChildVertex(vertices[Intercardinal::NW],
	 vertices[Intercardinal::NE]))) {

	 topMiddle = mesh->addVertex(
	 (vertices[Intercardinal::NW]->get()
	 + vertices[Intercardinal::NE]->get()) * 0.5);

	 mesh->setParentsChild(vertices[Intercardinal::NW],
	 vertices[Intercardinal::NE], topMiddle);
	 }

	 // Check if left child vertex exists
	 if (!(leftMiddle = mesh->getChildVertex(vertices[Intercardinal::NW],
	 vertices[Intercardinal::SW]))) {

	 leftMiddle = mesh->addVertex(
	 (vertices[Intercardinal::NW]->get()
	 + vertices[Intercardinal::SW]->get()) * 0.5);

	 mesh->setParentsChild(vertices[Intercardinal::NW],
	 vertices[Intercardinal::SW], leftMiddle);
	 }

	 mesh->removeTriangle(triangles[Cardinal::N]);
	 triangles[Cardinal::N] = nullptr;

	 mesh->removeTriangle(triangles[Cardinal::W]);
	 triangles[Cardinal::W] = nullptr;

	 if (!children[Intercardinal::NE]) {
	 triangles[Cardinal::N] = mesh->addTriangle(topMiddle,
	 vertices[Intercardinal::NE], middle);
	 }

	 if (!children[Intercardinal::SW]) {
	 triangles[Cardinal::W] = mesh->addTriangle(
	 vertices[Intercardinal::SW], leftMiddle, middle);
	 }

	 Quadtree * n = neighbours[Cardinal::N];
	 Quadtree * w = neighbours[Cardinal::W];

	 if (propagate) {
	 if (n && !n->getChild(Intercardinal::SW)) {
	 n->subdivide(Intercardinal::SW, false);
	 n = n->getChild(Intercardinal::SW);
	 }

	 if (w && !w->getChild(Intercardinal::NE)) {
	 w->subdivide(Intercardinal::NE, false);
	 w = w->getChild(Intercardinal::NE);
	 }
	 } else {
	 n = n ? n->getChild(Intercardinal::SE) : nullptr;
	 w = w ? w->getChild(Intercardinal::NE) : nullptr;
	 }

	 children[Intercardinal::NW] = new Quadtree(vertices[0], topMiddle,
	 leftMiddle, middle, level + 1, maxLevel, mesh, this);

	 if (propagate) {
	 if (!children[Intercardinal::NE]) {
	 subdivide(Intercardinal::NE, false);
	 }

	 if (!children[Intercardinal::SW]) {
	 subdivide(Intercardinal::SW, false);
	 }
	 }

	 children[0]->setNeighbours(n, children[Intercardinal::NE],
	 children[Intercardinal::SW], w);

	 break;
	 }
	 case Intercardinal::NE: {

	 //********************************
	 Vertex* topMiddle = nullptr;
	 Vertex* rightMiddle = nullptr;

	 // Check if top child vertex exists
	 if (!(topMiddle = mesh->getChildVertex(vertices[Intercardinal::NW],
	 vertices[Intercardinal::NE]))) {

	 topMiddle = mesh->addVertex(
	 (vertices[Intercardinal::NW]->get()
	 + vertices[Intercardinal::NE]->get()) * 0.5);

	 mesh->setParentsChild(vertices[Intercardinal::NW],
	 vertices[Intercardinal::NE], topMiddle);
	 }

	 // Check if right child vertex exists
	 if (!(rightMiddle = mesh->getChildVertex(vertices[Intercardinal::NE],
	 vertices[Intercardinal::SE]))) {
	 rightMiddle = mesh->addVertex(
	 (vertices[Intercardinal::NE]->get()
	 + vertices[Intercardinal::SE]->get()) * 0.5);

	 mesh->setParentsChild(vertices[Intercardinal::NE],
	 vertices[Intercardinal::SE], rightMiddle);
	 }
	 //********************************

	 mesh->removeTriangle(triangles[Cardinal::N]);
	 triangles[Cardinal::N] = nullptr;

	 mesh->removeTriangle(triangles[Cardinal::E]);
	 triangles[Cardinal::E] = nullptr;

	 if (!children[Intercardinal::NW]) {
	 triangles[Cardinal::N] = mesh->addTriangle(
	 vertices[Intercardinal::NW], topMiddle, middle);
	 }

	 if (!children[Intercardinal::SE]) {
	 triangles[Cardinal::E] = mesh->addTriangle(rightMiddle,
	 vertices[Intercardinal::SE], middle);
	 }

	 Quadtree * n = neighbours[Cardinal::N];
	 Quadtree * e = neighbours[Cardinal::E];

	 if (propagate) {
	 if (n && !n->getChild(Intercardinal::SE)) {
	 n->subdivide(Intercardinal::SE, false);

	 n = n->getChild(Intercardinal::SE);
	 }

	 if (e && !e->getChild(Intercardinal::NW)) {
	 e->subdivide(Intercardinal::NW, false);

	 e = e->getChild(Intercardinal::NW);
	 }
	 } else {
	 n = n ? n->getChild(Intercardinal::SE) : nullptr;
	 e = e ? e->getChild(Intercardinal::NW) : nullptr;
	 }

	 children[1] = new Quadtree(topMiddle, vertices[1], middle, rightMiddle,
	 level + 1, maxLevel, mesh, this);

	 if (propagate) {
	 if (!children[Intercardinal::NE]) {
	 subdivide(Intercardinal::NE, false);
	 }

	 if (!children[Intercardinal::SW]) {
	 subdivide(Intercardinal::SW, false);
	 }
	 }

	 children[1]->setNeighbours(n, e, children[Intercardinal::SE],
	 children[Intercardinal::NW]);

	 break;
	 }
	 case Intercardinal::SW: {

	 //********************************
	 Vertex* bottomMiddle = nullptr;
	 Vertex* leftMiddle = nullptr;

	 // Check if top child vertex exists
	 if (!(bottomMiddle = mesh->getChildVertex(vertices[Intercardinal::SW],
	 vertices[Intercardinal::SE]))) {

	 bottomMiddle = mesh->addVertex(
	 (vertices[Intercardinal::SW]->get()
	 + vertices[Intercardinal::SE]->get()) * 0.5);

	 mesh->setParentsChild(vertices[Intercardinal::SW],
	 vertices[Intercardinal::SE], bottomMiddle);
	 }

	 // Check if right child vertex exists
	 if (!(leftMiddle = mesh->getChildVertex(vertices[Intercardinal::NW],
	 vertices[Intercardinal::SW]))) {

	 leftMiddle = mesh->addVertex(
	 (vertices[Intercardinal::NW]->get()
	 + vertices[Intercardinal::SW]->get()) * 0.5);

	 mesh->setParentsChild(vertices[Intercardinal::NW],
	 vertices[Intercardinal::SW], leftMiddle);

	 }
	 //********************************

	 mesh->removeTriangle(triangles[Cardinal::S]);
	 triangles[Cardinal::S] = nullptr;

	 mesh->removeTriangle(triangles[Cardinal::W]);
	 triangles[Cardinal::W] = nullptr;

	 if (!children[Intercardinal::SE]) {
	 triangles[Cardinal::S] = mesh->addTriangle(
	 vertices[Intercardinal::SE], bottomMiddle, middle);
	 }

	 if (!children[Intercardinal::NW]) {
	 triangles[Cardinal::W] = mesh->addTriangle(leftMiddle,
	 vertices[Intercardinal::NW], middle);
	 }

	 Quadtree * s = neighbours[Cardinal::S];
	 Quadtree * w = neighbours[Cardinal::W];

	 if (propagate) {
	 if (s && !s->getChild(Intercardinal::NW)) {
	 s->subdivide(Intercardinal::NW, false);

	 s = s->getChild(Intercardinal::NW);
	 }

	 if (w && !w->getChild(Intercardinal::SE)) {
	 w->subdivide(Intercardinal::SE, false);

	 w = w->getChild(Intercardinal::SE);
	 }
	 } else {
	 s = s ? s->getChild(Intercardinal::NW) : nullptr;
	 w = w ? w->getChild(Intercardinal::SE) : nullptr;
	 }

	 children[2] = new Quadtree(leftMiddle, middle, vertices[2],
	 bottomMiddle, level + 1, maxLevel, mesh, this);

	 if (propagate) {
	 if (!children[Intercardinal::NW]) {
	 subdivide(Intercardinal::NW, false);
	 }

	 if (!children[Intercardinal::SE]) {
	 subdivide(Intercardinal::SE, false);
	 }
	 }

	 children[2]->setNeighbours(children[Intercardinal::NW],
	 children[Intercardinal::SE], s, w);

	 break;
	 }
	 case Intercardinal::SE: {

	 //********************************
	 Vertex* bottomMiddle = nullptr;
	 Vertex* rightMiddle = nullptr;

	 // Check if top child vertex exists
	 if (mesh->getChildVertex(vertices[Intercardinal::SW],
	 vertices[Intercardinal::SE])) {

	 bottomMiddle = mesh->getChildVertex(vertices[Intercardinal::SW],
	 vertices[Intercardinal::SE]);
	 } else {

	 bottomMiddle = mesh->addVertex(
	 (vertices[Intercardinal::SW]->get()
	 + vertices[Intercardinal::SE]->get()) * 0.5);

	 mesh->setParentsChild(vertices[Intercardinal::SW],
	 vertices[Intercardinal::SE], bottomMiddle);
	 }

	 // Check if right child vertex exists
	 if (mesh->getChildVertex(vertices[Intercardinal::NE],
	 vertices[Intercardinal::SE])) {

	 rightMiddle = mesh->getChildVertex(vertices[Intercardinal::NE],
	 vertices[Intercardinal::SE]);
	 } else {

	 rightMiddle = mesh->addVertex(
	 (vertices[Intercardinal::NE]->get()
	 + vertices[Intercardinal::SE]->get()) * 0.5);

	 mesh->setParentsChild(vertices[Intercardinal::NE],
	 vertices[Intercardinal::SE], rightMiddle);
	 }
	 //********************************

	 mesh->removeTriangle(triangles[Cardinal::S]);
	 triangles[Cardinal::S] = nullptr;

	 mesh->removeTriangle(triangles[Cardinal::E]);
	 triangles[Cardinal::E] = nullptr;

	 if (!children[Intercardinal::SW]) {
	 triangles[Cardinal::S] = mesh->addTriangle(bottomMiddle,
	 vertices[Intercardinal::SW], middle);
	 }

	 if (!children[Intercardinal::NE]) {
	 triangles[Cardinal::E] = mesh->addTriangle(
	 vertices[Intercardinal::NE], rightMiddle, middle);
	 }

	 Quadtree * s = neighbours[Cardinal::S];
	 Quadtree * e = neighbours[Cardinal::E];

	 if (propagate) {
	 if (s && !s->getChild(Intercardinal::NE)) {
	 s->subdivide(Intercardinal::NE, false);

	 s = s->getChild(Intercardinal::NE);
	 }

	 if (e && !e->getChild(Intercardinal::SW)) {
	 e->subdivide(Intercardinal::SW, false);

	 e = e->getChild(Intercardinal::SW);
	 }
	 } else {
	 s = s ? s->getChild(Intercardinal::NE) : nullptr;
	 e = e ? e->getChild(Intercardinal::SW) : nullptr;
	 }

	 children[3] = new Quadtree(middle, rightMiddle, bottomMiddle,
	 vertices[3], level + 1, maxLevel, mesh, this);

	 if (propagate) {
	 if (!children[Intercardinal::NE]) {
	 subdivide(Intercardinal::NE, false);
	 }

	 if (!children[Intercardinal::SW]) {
	 subdivide(Intercardinal::SW, false);
	 }
	 }

	 children[3]->setNeighbours(children[Intercardinal::NE], e, s,
	 children[Intercardinal::SW]);

	 break;
	 }

	 }
	 */
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

void Quadtree::setNeighbourAt(directions::Cardinal _position,
		Quadtree* _value) {
	neighbours[_position] = _value;
}

void Quadtree::setNeighbours(Quadtree* n, Quadtree* e, Quadtree* s,
		Quadtree* w) {

	neighbours[0] = n;
	neighbours[1] = e;
	neighbours[2] = s;
	neighbours[3] = w;
}

Quadtree::Quadtree(Vertex* nw, Vertex* ne, Vertex* sw, Vertex* se, int _level,
		int _maxLevel, Mesh * _mesh, Quadtree* _parent) :
		vertices { nw, ne, sw, se }, origin(sw->get()), xAxis(
				se->get() - sw->get()), yAxis(nw->get() - sw->get()), level(
				_level), middle(nullptr), maxLevel(_maxLevel), children {
				nullptr }, triangles { nullptr }, parent(_parent), mesh(_mesh) {

	auto temp1 = (nw->get() + ne->get()) * 0.5;
	auto temp2 = (sw->get() + se->get()) * 0.5;

	auto m = (temp1 + temp2) * 0.5;

	middle = mesh->addVertex(m);

	triangles[0] = mesh->addTriangle(vertices[0], vertices[1], middle);
	triangles[1] = mesh->addTriangle(vertices[1], vertices[3], middle);
	triangles[2] = mesh->addTriangle(vertices[3], vertices[2], middle);
	triangles[3] = mesh->addTriangle(vertices[2], vertices[0], middle);

	triangles[directions::N]->setUserData(
			new QuadtreeData(directions::NE, directions::NW, this));
	triangles[directions::S]->setUserData(
			new QuadtreeData(directions::SW, directions::SE, this));
	triangles[directions::W]->setUserData(
			new QuadtreeData(directions::NW, directions::SW, this));
	triangles[directions::E]->setUserData(
			new QuadtreeData(directions::SE, directions::NE, this));
}

Quadtree::Quadtree(const Vec3f& _origin, const Vec3f& _xAxis,
		const Vec3f& _yAxis, int _level, int _maxLevel) :
		origin(_origin), xAxis(_xAxis), yAxis(_yAxis), level(_level), maxLevel(
				_maxLevel), children { nullptr }, vertices { nullptr }, triangles {
				nullptr }, parent(nullptr), middle(nullptr), mesh(new Mesh()), neighbours {
				nullptr } {

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

	triangles[directions::N]->setUserData(
			new QuadtreeData(directions::NE, directions::NW, this));
	triangles[directions::S]->setUserData(
			new QuadtreeData(directions::SW, directions::SE, this));
	triangles[directions::W]->setUserData(
			new QuadtreeData(directions::NW, directions::SW, this));
	triangles[directions::E]->setUserData(
			new QuadtreeData(directions::SE, directions::NE, this));
}

Quadtree::~Quadtree() {

	for (auto& t : children) {
		if (t)
			delete t;
	}

	for (auto& t : triangles) {
		if (t)
			mesh->removeTriangle(t);
	}

	mesh->removeVertex(middle);

	if (!parent) {
		delete mesh;
	}
}

int Quadtree::findPosition(Triangle* triangle) const {

	for (int i = 0; i < 4; ++i) {
		if (triangles[i] == triangle) {
			return i;
		}
	}

	return -1;
}
