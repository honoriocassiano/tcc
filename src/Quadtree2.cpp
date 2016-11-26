/*
 * Quadtree2.cpp
 *
 *  Created on: 19 de nov de 2016
 *      Author: cassiano
 */

#include "Quadtree2.h"

#include <math.h>

float Quadtree2::C = 0.5;

#define MIDDLE(P1, P2) ( (P1 + P2 ) * 0.5)
#define CAST(X, TYPE) static_cast<TYPE>(X)

Quadtree2::Quadtree2(Vertex* nw, Vertex* ne, Vertex* sw, Vertex* se,
		Mesh * _mesh) :
		parent(nullptr), mesh(_mesh), intercardinals(
				IntercardinalDirection::getAll(), nullptr), children(
				IntercardinalDirection::getAll(), nullptr) {
//		parent(nullptr), mesh(_mesh), intercardinals { nw, ne, sw, se }, children {
//				nullptr } {

	using namespace directions;

	auto temp1 = (nw->get() + ne->get()) * 0.5;
	auto temp2 = (sw->get() + se->get()) * 0.5;

	auto m = (temp1 + temp2) * 0.5;

	center = mesh->addVertex(m);
	center->setActive(true);

	// Create cardinals vertices
	for (int i = 0; i < 4; ++i) {
		auto& d1 = intercardinals[*IntercardinalDirection::getAtClockwiseIndex(
				i)];
		auto& d2 = intercardinals[*IntercardinalDirection::getAtClockwiseIndex(
				(i + 1) % 4)];
//		auto& d1 = intercardinals[i];
//		auto& d2 = intercardinals[(i + 1) % 4];

		auto tempVertex = mesh->addVertex(MIDDLE(d1->get(), d2->get()));

		mesh->setParentsChild(d1, d2, tempVertex);
	}

	// Create middle points between center and intercardinals
	for (int i = 0; i < 4; ++i) {
		//auto& d1 = intercardinals[i];
		auto& d1 = intercardinals[*IntercardinalDirection::getAtClockwiseIndex(
				i)];

		auto tempVertex = mesh->addVertex(MIDDLE(center->get(), d1->get()));

		mesh->setParentsChild(d1, center, tempVertex);
	}
}

Quadtree2::~Quadtree2() {
	delete center;

	for (auto& e : IntercardinalDirection::getAll()) {
		delete intercardinals[e];
	}
}

void Quadtree2::update(const Vec3f& cameraPosition) {

	bool marked[4] { false };

	for (int i = 0; i < 4; ++i) {
//		auto& e = intercardinals[i];
		auto& e = intercardinals[*IntercardinalDirection::getAtClockwiseIndex(4)];

		//auto middlePoint = (center->get() + e->get()) * 0.5;
		auto middlePoint = mesh->getChildVertex(center, e)->get();

		auto d =
				((intercardinals[IntercardinalDirection::NE]->get() + intercardinals[IntercardinalDirection::NW]->get()) * 0.5).Length();
		auto l = (middlePoint - cameraPosition).Length();

		if (l / d < C) {
			// Remove edges
			marked[i] = true;
		}
	}

	for (int i = 0; i < 4; ++i) {

		auto& enumValue = *IntercardinalDirection::getAtClockwiseIndex(i);

		if (marked[i] && !children[enumValue]) {
			// Marked for creation and it's not exist
			// so this have be created
			{
				auto edge = mesh->getEdge(center, intercardinals[enumValue]);
				auto opposite = mesh->getEdge(intercardinals[enumValue], center);

				mesh->removeTriangle(edge->getTriangle());
				mesh->removeTriangle(opposite->getTriangle());
			}

			{

			}

		} else if (!marked[i] && !children[enumValue]) {

			if (!mesh->getEdge(center, intercardinals[enumValue])) {

			}

		} else if (!marked[i] && children[enumValue]) {
			// Marked for creation and it's exist
			// so this have be deleted

			delete children[enumValue];

			children[enumValue] = nullptr;
		}

	}
}

//void Quadtree2::setNeighbors(Quadtree* n, Quadtree* e, Quadtree* s,
//		Quadtree* w) {
//}

void Quadtree2::render() {
}

#undef MIDDLE
#undef CAST

