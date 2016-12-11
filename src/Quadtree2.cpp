/*
 * Quadtree2.cpp
 *
 *  Created on: 19 de nov de 2016
 *      Author: cassiano
 */

#include "Quadtree2.h"

#include <math.h>
#include <algorithm>
#include "CardinalDirection.h"
#include "Perlin.h"

float Quadtree2::C = 1.0;

#define MIDDLE(P1, P2) ( (P1 + P2 ) * 0.5)
#define CAST(X, TYPE) static_cast<TYPE>(X)

Quadtree2::Quadtree2(Vertex* nw, Vertex* ne, Vertex* sw, Vertex* se,
		QuadtreeMesh * _mesh) :
		parent(nullptr), mesh(_mesh), intercardinals(
				IntercardinalDirection::getAll(), nullptr), children(
				IntercardinalDirection::getAll(), nullptr), neighbors(
				CardinalDirection::getAll(), nullptr) {

	// TODO Delete this code when the new constructor will be created
	intercardinals[IntercardinalDirection::NW] = nw;
	intercardinals[IntercardinalDirection::NE] = ne;
	intercardinals[IntercardinalDirection::SW] = sw;
	intercardinals[IntercardinalDirection::SE] = se;

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

		if (!mesh->getChildVertex(d1, d2)) {

			auto tempVertex = mesh->addVertex(MIDDLE(d1->get(), d2->get()));

			mesh->setParentsChild(d1, d2, tempVertex);
		}
	}

// Create middle points between center and intercardinals
	for (int i = 0; i < 4; ++i) {
		auto& d1 = intercardinals[*IntercardinalDirection::getAtClockwiseIndex(
				i)];

		if (!mesh->getChildVertex(center, d1)) {
			auto tempVertex = mesh->addVertex(MIDDLE(center->get(), d1->get()));

			mesh->setParentsChild(d1, center, tempVertex);
		}
	}

	/*
	 // Create triangles
	 for (int i = 0; i < 4; ++i) {
	 auto& d1 = intercardinals[*IntercardinalDirection::getAtClockwiseIndex(
	 i)];
	 auto& d2 = intercardinals[*IntercardinalDirection::getAtClockwiseIndex(
	 (i + 1) % 4)];

	 mesh->addTriangle(center, d1, d2);
	 }
	 */
}

Quadtree2::~Quadtree2() {
	delete center;

	for (auto& e : IntercardinalDirection::getAll()) {
//	for (int i = 0; i < IntercardinalDirection::getAll().size(); ++i) {

//		auto e = *IntercardinalDirection::getAtClockwiseIndex(i);

		delete intercardinals[*e];
	}
}

void Quadtree2::updateActives(const Vec3f& cameraPosition, Vertex* center,
		DirectionArray<IntercardinalDirection, Vertex*>& intercardinals) {

	for (int i = 0; i < 4; ++i) {

		auto direction = *IntercardinalDirection::getAtClockwiseIndex(i);

		auto& e = intercardinals[direction];

		auto middleVertex = mesh->getChildVertex(center, e);

		if (middleVertex->isActive()) {

			auto temp = DirectionArray<IntercardinalDirection, Vertex*>(
					IntercardinalDirection::getAll(), nullptr);

			temp[*IntercardinalDirection::getAtClockwiseIndex(i)] = e;

			temp[*IntercardinalDirection::getAtClockwiseIndex((i + 1) % 4)] =
					mesh->getChildVertex(e,
							intercardinals[*IntercardinalDirection::getAtClockwiseIndex(
									(i + 1) % 4)]);
			temp[*IntercardinalDirection::getAtClockwiseIndex((i + 2) % 4)] =
					center;

			temp[*IntercardinalDirection::getAtClockwiseIndex((i + 3) % 4)] =
					mesh->getChildVertex(e,
							intercardinals[*IntercardinalDirection::getAtClockwiseIndex(
									(i + 3) % 4)]);

			updateActives(cameraPosition, middleVertex, temp);
		}

		auto d =
				((intercardinals[IntercardinalDirection::NE]->get()
						+ intercardinals[IntercardinalDirection::NW]->get())
						* 0.5).Length();

		auto l = (middleVertex->get() - cameraPosition).Length();

		if (l / d < C) {
			// Remove edges
			//			marked[*IntercardinalDirection::getAtClockwiseIndex(i)] = true;
			middleVertex->setActive(true);
		}
	}
}

void Quadtree2::update2(const Vec3f& cameraPosition, const std::string& tag) {

	/*
	 //	DirectionArray<IntercardinalDirection, bool> marked(
	 //			IntercardinalDirection::getAll(), false);


	 for (int i = 0; i < 4; ++i) {
	 auto& e =
	 intercardinals[*IntercardinalDirection::getAtClockwiseIndex(i)];

	 auto middleVertex = mesh->getChildVertex(center, e);

	 auto d =
	 ((intercardinals[IntercardinalDirection::NE]->get()
	 + intercardinals[IntercardinalDirection::NW]->get())
	 * 0.5).Length();
	 auto l = (middleVertex->get() - cameraPosition).Length();

	 if (l / d < C) {
	 // Remove edges
	 //			marked[*IntercardinalDirection::getAtClockwiseIndex(i)] = true;
	 middleVertex->setActive(true);
	 }
	 }
	 */

	updateActives(cameraPosition, center, intercardinals);

	remesh(center, intercardinals,
			DirectionArray<CardinalDirection, Vertex*>(
					CardinalDirection::getAll(), nullptr));
}

float Quadtree2::calculateRoughness(const Vertex* center,
		const DirectionArray<IntercardinalDirection, Vertex*>& intercardinals) {

	float dh[6] { 0.0f };

	for (auto i = 0; i < 4; ++i) {
		auto v1 =
				intercardinals[*IntercardinalDirection::getAtClockwiseIndex(i)];
		auto v2 = intercardinals[*IntercardinalDirection::getAtClockwiseIndex(
				(i + 1) % 4)];

		auto middle = mesh->getOrCreateChildVertex(v1, v2);

		auto elevation = Perlin::generate(middle->get());

		// TODO Change this formula
		auto elevationVector = Vec3f(middle->get().x(), middle->get().y(),
				middle->get().z() + (1.0 * elevation));

		// |(p - r1)x(p - r2)| / |r2 - r1|

		Vec3f numerator;

		Vec3f::Cross3(numerator, (elevationVector - v1->get()),
				(elevationVector - v2->get()));

		dh[i] = numerator.Length() / (v1->get() - v2->get()).Length();
	}

	for (auto i = 0; i < 2; ++i) {
		auto v1 =
				intercardinals[*IntercardinalDirection::getAtClockwiseIndex(i)];
		auto v2 = intercardinals[*IntercardinalDirection::getAtClockwiseIndex(
				(i + 2) % 4)];

		auto elevation = Perlin::generate(center->get());

		// TODO Change this formula
		auto elevationVector = Vec3f(center->get().x(), center->get().y(),
				center->get().z() + (1.0 * elevation));

		Vec3f numerator;

		Vec3f::Cross3(numerator, (elevationVector - v1->get()),
				(elevationVector - v2->get()));

		dh[i + 4] = numerator.Length() / (v1->get() - v2->get()).Length();
	}

	auto d = (intercardinals[IntercardinalDirection::NE]->get()
			- intercardinals[IntercardinalDirection::NW]->get()).Length();

	return (*std::max_element(dh, dh + 6) / d);
}

void Quadtree2::remesh(Vertex* center,
		const DirectionArray<IntercardinalDirection, Vertex*>& intercardinals,
		const DirectionArray<CardinalDirection, Vertex*>& neighbors,
		const std::string& tag) {

	Log("%s", tag.c_str());

	for (auto i = 0; i < IntercardinalDirection::getCount(); ++i) {
		auto middleVertex =
				mesh->getOrCreateChildVertex(center,
						intercardinals[*IntercardinalDirection::getAtClockwiseIndex(
								i)]);
//				mesh->getChildVertex(center,
//						intercardinals[*IntercardinalDirection::getAtClockwiseIndex(
//								i)]);

		Log("%s %p", tag.c_str(), middleVertex);

		if (middleVertex->isActive()) {

			Log("%s", tag.c_str());

			auto direction = *IntercardinalDirection::getAtClockwiseIndex(i);

			auto& e = intercardinals[direction];

			auto tempIntercardinals = DirectionArray<IntercardinalDirection,
					Vertex*>(IntercardinalDirection::getAll(), nullptr);
			auto tempNeighbors = DirectionArray<CardinalDirection, Vertex*>(
					CardinalDirection::getAll(), nullptr);

			Log("%s", tag.c_str());

			// Setting intercardinals
			tempIntercardinals[*IntercardinalDirection::getAtClockwiseIndex(i)] =
					e;

			tempIntercardinals[*IntercardinalDirection::getAtClockwiseIndex(
					(i + 1) % 4)] = mesh->getOrCreateChildVertex(e,
					intercardinals[*IntercardinalDirection::getAtClockwiseIndex(
							(i + 1) % 4)]);
			tempIntercardinals[*IntercardinalDirection::getAtClockwiseIndex(
					(i + 2) % 4)] = center;

			tempIntercardinals[*IntercardinalDirection::getAtClockwiseIndex(
					(i + 3) % 4)] = mesh->getOrCreateChildVertex(e,
					intercardinals[*IntercardinalDirection::getAtClockwiseIndex(
							(i + 3) % 4)]);

			Log("%s", tag.c_str());

			// Setting neighbors
			tempNeighbors[*CardinalDirection::getAtClockwiseIndex(i)] =
					neighbors[*CardinalDirection::getAtClockwiseIndex(i)];

			tempNeighbors[*CardinalDirection::getAtClockwiseIndex((i + 1) % 4)] =
					mesh->getOrCreateChildVertex(center,
							intercardinals[*IntercardinalDirection::getAtClockwiseIndex(
									(i + 1) % 4)]);

			tempNeighbors[*CardinalDirection::getAtClockwiseIndex((i + 2) % 4)] =
					mesh->getOrCreateChildVertex(center,
							intercardinals[*IntercardinalDirection::getAtClockwiseIndex(
									(i + 3) % 4)]);

			tempNeighbors[*CardinalDirection::getAtClockwiseIndex((i + 3) % 4)] =
					neighbors[*CardinalDirection::getAtClockwiseIndex(
							(i + 3) % 4)];

			Log("%s", tag.c_str());

			remesh(middleVertex, tempIntercardinals, tempNeighbors,
					tag + " > " + std::to_string(i));
		}
	}

	Log("%s", tag.c_str());

	for (auto i = 0; i < CardinalDirection::getCount(); ++i) {
		auto direction = *CardinalDirection::getAtClockwiseIndex(i);

		auto v1 =
				intercardinals[*IntercardinalDirection::getAtClockwiseIndex(i)];
		auto v2 = intercardinals[*IntercardinalDirection::getAtClockwiseIndex(
				(i + 1) % 4)];

		// Check if not have neighbor
		if (!neighbors[direction]) {

			auto cardinalMiddle = mesh->getOrCreateChildVertex(v1, v2);

			// TODO If this condition must be an AND or OR
			if (!(mesh->getEdge(center, cardinalMiddle)
					&& mesh->getEdge(cardinalMiddle, center))) {

				mesh->addTriangle(center, v1, cardinalMiddle);
				mesh->addTriangle(center, v2, cardinalMiddle);
			}
		} else if (!neighbors[direction]->isActive()) {
			auto tempEdge = mesh->getEdge(center, v1);
			auto exist = false;

			if (tempEdge && (tempEdge->getNext()->getVertex() == v2)) {
				exist = true;
			} else {
				tempEdge = mesh->getEdge(v1, center);
			}

			if (!exist && tempEdge
					&& (tempEdge->getNext()->getVertex() == v2)) {
				exist = true;
			}

			if (!exist) {
				mesh->addTriangle(center, v1, v2);
			}
		}
	}

	Log("%s", tag.c_str());
}

void Quadtree2::update(const Vec3f& cameraPosition, const std::string& tag) {

	DirectionArray<IntercardinalDirection, bool> marked(
			IntercardinalDirection::getAll(), false);

	Log("%s", tag.c_str());

	for (auto& e : IntercardinalDirection::getAll()) {
		if (children[*e]) {
			children[*e]->update(cameraPosition,
					tag + " > " + std::to_string(e->getClockwiseIndex()));
		}
	}

	Log("%s", tag.c_str());

// Mark who children must be exist
	for (int i = 0; i < 4; ++i) {
		auto& e =
				intercardinals[*IntercardinalDirection::getAtClockwiseIndex(i)];

		auto middlePoint = mesh->getChildVertex(center, e)->get();

		auto d =
				((intercardinals[IntercardinalDirection::NE]->get()
						+ intercardinals[IntercardinalDirection::NW]->get())
						* 0.5).Length();
		auto l = (middlePoint - cameraPosition).Length();

		if (l / d < C) {
			// Remove edges
			marked[*IntercardinalDirection::getAtClockwiseIndex(i)] = true;

		}
	}

	Log("%s", tag.c_str());

// Update the children
	for (int i = 0; i < 4; ++i) {

		auto& enumValue = *IntercardinalDirection::getAtClockwiseIndex(i);

		Log("%s %d", tag.c_str(), i);

		if (marked[*IntercardinalDirection::getAtClockwiseIndex(i)]
				&& !children[enumValue]) {

			// Marked for creation and it's not exist
			// so this have be created
			{
				auto edge = mesh->getEdge(center, intercardinals[enumValue]);
				auto opposite = mesh->getEdge(intercardinals[enumValue],
						center);

				auto edgeDeleted = false;
				auto oppositeDeleted = false;

				{

					Log("%p %p", edge, opposite);

					// Remove the triangles
					if (edge) {
						Log("%s", tag.c_str());
						mesh->removeTriangle(edge->getTriangle());
						edgeDeleted = true;
					}

					if (opposite) {
						Log("%s", tag.c_str());
						mesh->removeTriangle(opposite->getTriangle());
						oppositeDeleted = true;
					}
				}

				Log("%s", tag.c_str());

				{
					// Recreate the triangles
					if (oppositeDeleted) {
						mesh->addTriangle(center,
								intercardinals[*IntercardinalDirection::getAtClockwiseIndex(
										(i + 3) % 4)],
								intercardinals[enumValue]);
					}

					Log("%s", tag.c_str());

					if (edgeDeleted) {
						mesh->addTriangle(center,
								intercardinals[*IntercardinalDirection::getAtClockwiseIndex(
										(i + 1) % 4)],
								intercardinals[enumValue]);
					}
				}

				Log("%s", tag.c_str());
			}

			Log("%s", tag.c_str());

			{
				auto& cd1 = *CardinalDirection::getAtClockwiseIndex(i);
				auto& cd2 = *CardinalDirection::getAtClockwiseIndex(
						(i + 3) % 4);

				auto& icd1 = *IntercardinalDirection::getAtClockwiseIndex(
						(i + 1) % 4);
				auto& icd2 = *IntercardinalDirection::getAtClockwiseIndex(
						(i + 3) % 4);

				DirectionArray<CardinalDirection, bool> localMarked(
						CardinalDirection::getAll(), false);

				// TODO Add more checks here
				if (!neighbors[cd1]) {
					localMarked[cd1] = false;
				}

				if (!neighbors[cd2]) {
					localMarked[cd2] = false;
				}

				localMarked[*CardinalDirection::getAtClockwiseIndex((i + 1) % 4)] =
						marked[icd1];

				localMarked[*CardinalDirection::getAtClockwiseIndex((i + 2) % 4)] =
						marked[icd2];

				// NW, NE, SE, SW
				Vertex* v[4] { nullptr };
				Vertex* localCenter =
						mesh->getChildVertex(center,
								intercardinals[*IntercardinalDirection::getAtClockwiseIndex(
										i)]);

				v[i] =
						intercardinals[*IntercardinalDirection::getAtClockwiseIndex(
								i)];

				v[(i + 2) % 4] = center;

				v[(i + 1) % 4] =
						mesh->getChildVertex(
								intercardinals[*IntercardinalDirection::getAtClockwiseIndex(
										i)],
								intercardinals[*IntercardinalDirection::getAtClockwiseIndex(
										(i + 1) % 4)]);

				v[(i + 3) % 4] =
						mesh->getChildVertex(
								intercardinals[*IntercardinalDirection::getAtClockwiseIndex(
										i)],
								intercardinals[*IntercardinalDirection::getAtClockwiseIndex(
										(i + 3) % 4)]);

				children[enumValue] = new Quadtree2(v[0], v[1], v[3], v[2],
						localCenter, mesh, localMarked, this);
			}

			Log("%s", tag.c_str());

		} else if (!marked[*IntercardinalDirection::getAtClockwiseIndex(i)]
				&& !children[enumValue]) {

			if (!mesh->getEdge(center, intercardinals[enumValue])) {

			}

		} else if (!marked[*IntercardinalDirection::getAtClockwiseIndex(i)]
				&& children[enumValue]) {
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

//Quadtree2::Quadtree2(Vertex* nw, Vertex* ne, Vertex* sw, Vertex* se,
//		Vertex* _center, Mesh* _mesh,
//		const DirectionArray<CardinalDirection, bool>& marked,
//		const DirectionArray<CardinalDirection, bool>& willHaveNeighbor) :
//		parent(nullptr), mesh(_mesh), intercardinals(
//				IntercardinalDirection::getAll(), nullptr), children(
//				IntercardinalDirection::getAll(), nullptr), center(_center) {

Quadtree2::Quadtree2(Vertex* nw, Vertex* ne, Vertex* sw, Vertex* se,
		Vertex* _center, QuadtreeMesh* _mesh,
		const DirectionArray<CardinalDirection, bool>& marked,
		Quadtree2* _parent) :
		parent(_parent), mesh(_mesh), intercardinals(
				IntercardinalDirection::getAll(), nullptr), children(
				IntercardinalDirection::getAll(), nullptr), neighbors(
				CardinalDirection::getAll(), nullptr), center(_center) {

// TODO Delete this code when the new constructor will be created
	intercardinals[IntercardinalDirection::NW] = nw;
	intercardinals[IntercardinalDirection::NE] = ne;
	intercardinals[IntercardinalDirection::SW] = sw;
	intercardinals[IntercardinalDirection::SE] = se;

	for (int i = 0; i < 4; ++i) {
		auto& cd = *CardinalDirection::getAtClockwiseIndex(i);

		auto& current_icd = *IntercardinalDirection::getAtClockwiseIndex(i);
		auto& next_icd = *IntercardinalDirection::getAtClockwiseIndex(
				(i + 1) % 4);

		if (!marked[cd]) {

			mesh->addTriangle(center, intercardinals[current_icd],
					intercardinals[next_icd]);

		} else if (marked[cd]) {

			auto& v1 = intercardinals[current_icd];
			auto& v2 = intercardinals[next_icd];

			auto child = mesh->getChildVertex(v1, v2);

			if (!child) {
				child = mesh->addVertex(MIDDLE(v1->get(), v2->get()));

				mesh->setParentsChild(v1, v2, child);
			}

			mesh->addTriangle(child, v1, center);
			mesh->addTriangle(child, v2, center);

		}
	}

// Create cardinals vertices
	for (int i = 0; i < 4; ++i) {
		auto& d1 = intercardinals[*IntercardinalDirection::getAtClockwiseIndex(
				i)];
		auto& d2 = intercardinals[*IntercardinalDirection::getAtClockwiseIndex(
				(i + 1) % 4)];

		if (!mesh->getChildVertex(d1, d2)) {

			auto tempVertex = mesh->addVertex(MIDDLE(d1->get(), d2->get()));

			mesh->setParentsChild(d1, d2, tempVertex);
		}
	}

// Create middle points between center and intercardinals
	for (int i = 0; i < 4; ++i) {
		auto& d1 = intercardinals[*IntercardinalDirection::getAtClockwiseIndex(
				i)];

		if (!mesh->getChildVertex(center, d1)) {
			auto tempVertex = mesh->addVertex(MIDDLE(center->get(), d1->get()));

			mesh->setParentsChild(d1, center, tempVertex);
		}
	}
}

#undef MIDDLE
#undef CAST

