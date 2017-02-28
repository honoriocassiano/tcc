/*
 * Quadtree2.cpp
 *
 *  Created on: 19 de nov de 2016
 *      Author: cassiano
 */

#include "Quadtree2.h"

#include <math.h>
#include <algorithm>

#include "Perlin.h"
#include "MathUtils.hpp"

template<class DType, class Element>
using DA = DirectionArray<DType, Element>;

using ID = IntercardinalDirection;
using CD = CardinalDirection;

float Quadtree2::C = 20;
float Quadtree2::c = 30;

#define MIDDLE(P1, P2) ( (P1 + P2 ) * 0.5)
#define CAST(X, TYPE) static_cast<TYPE>(X)

Quadtree2::Quadtree2(Vertex* nw, Vertex* ne, Vertex* sw, Vertex* se,
		QuadtreeMesh * _mesh) :
		parent(nullptr), mesh(_mesh), intercardinals(ID::all(), nullptr), children(
				ID::all(), nullptr), neighbors(CD::all(), nullptr) {

	// TODO Delete this code when the new constructor will be created
	intercardinals[ID::NW] = nw;
	intercardinals[ID::NE] = ne;
	intercardinals[ID::SW] = sw;
	intercardinals[ID::SE] = se;

	using namespace directions;

	auto temp1 = (nw->get() + ne->get()) * 0.5;
	auto temp2 = (sw->get() + se->get()) * 0.5;

	auto m = (temp1 + temp2) * 0.5;

	center = mesh->addVertex(m);
	center->setActive(true);

//	mesh->setParentsChild(nw, se, ne, sw, center);

// Create cardinals vertices
	for (int i = 0; i < 4; ++i) {
		auto& d1 = intercardinals[*ID::getAtClockwiseIndex(i)];
		auto& d2 = intercardinals[*ID::getAtClockwiseIndex((i + 1) % 4)];

		mesh->getOrCreateChildVertex(d1, d2);
		/*
		 if (!mesh->getChildVertex(d1, d2)) {

		 auto tempVertex = mesh->addVertex(MIDDLE(d1->get(), d2->get()));

		 mesh->setParentsChild(d1, d2, tempVertex);
		 }
		 */
	}

// Create middle points between center and intercardinals
	for (int i = 0; i < 4; ++i) {
		auto& d1 = intercardinals[*ID::getAtClockwiseIndex(i)];

		mesh->getOrCreateChildVertex(center, d1);
		/*
		 if (!mesh->getChildVertex(center, d1)) {
		 auto tempVertex = mesh->addVertex(MIDDLE(center->get(), d1->get()));

		 mesh->setParentsChild(d1, center, tempVertex);
		 }
		 */
	}

	/*
	 // Create triangles
	 for (int i = 0; i < 4; ++i) {
	 auto& d1 = intercardinals[*ID::getAtClockwiseIndex(
	 i)];
	 auto& d2 = intercardinals[*ID::getAtClockwiseIndex(
	 (i + 1) % 4)];

	 mesh->addTriangle(center, d1, d2);
	 }
	 */
}

Quadtree2::~Quadtree2() {
	delete center;

	for (auto& e : ID::all()) {
//	for (int i = 0; i < ID::getAll().size(); ++i) {

//		auto e = *ID::getAtClockwiseIndex(i);

//		delete intercardinals[*e];
	}
}

void Quadtree2::updateD2() {
	recursiveUpdateD2(center, intercardinals, neighbors);
}

#define K (C / (2 * (C - 1)))

void Quadtree2::recursiveUpdateD2(Vertex* center,
		DA<ID, Vertex*>& intercardinals, DA<CD, Vertex*>& neighbors) {

	float neighborsD2[4] { 0.0f };

	for (int i = 0; i < 4; ++i) {

		auto& direction = *CD::getAtClockwiseIndex(i);

		if (neighbors[direction]) {
			neighborsD2[i] = neighbors[direction]->getD2();
		}
	}

	if ((*std::min_element(neighborsD2, neighborsD2 + 4) / center->getD2()) < K) {

	}
}

#undef K

void Quadtree2::updateActiveIntercardinals(Vertex* center,
		DA<ID, Vertex*>& intercardinals) {

	auto centerIsActive = center->isActive();

	for (auto i = 0; i < 4; ++i) {
		auto& direction = *ID::getAtClockwiseIndex(i);

		if (intercardinals[direction]) {
			auto middleIntercardinal = mesh->getChildVertex(center,
					intercardinals[direction]);

			if (middleIntercardinal) {
				auto relativeIntercardinals =
						getRelativeIntercardinalsWithoutCreate(direction,
								center, intercardinals);

				updateActiveIntercardinals(middleIntercardinal,
						relativeIntercardinals);
			}
		}
	}

	for (auto i = 0; i < 4; ++i) {
		auto& direction = *ID::getAtClockwiseIndex(i);

		center->setActive(centerIsActive);

		if (intercardinals[direction]) {
			intercardinals[direction]->setActive(centerIsActive);
		}
	}
}

void Quadtree2::updateActiveCenters(const Vec3f& cameraPosition, Vertex* center,
		DA<ID, Vertex*>& intercardinals, const DA<CD, Vertex*>& neighbors) {

	for (auto i = 0; i < 4; ++i) {

		auto& direction = *ID::getAtClockwiseIndex(i);

		auto& e = intercardinals[direction];

		auto middleVertex = mesh->getOrCreateChildVertex(center, e);

		auto d =
				(intercardinals[ID::NE]->get() - intercardinals[ID::NW]->get()).Length();

		auto l = (middleVertex->get() - cameraPosition).Length();

//		auto roughness = calcRoughness(center, intercardinals);

// TODO Change this value
//		if ((l / d) < C) {
//		if ((l / (d * C * std::max(c * roughness, 1.0f))) < 1) {
		if ((l / (d * C * std::max(c * center->getD2(), 1.0f))) < 1) {
			middleVertex->setActive(true);
		} else {

//			if (middleVertex->isActive()) {
//
//				auto relativeIntercardinals = getRelativeIntercardinals(
//						direction, center, intercardinals);
//
//				recursiveDeleteVertices(middleVertex, direction,
//						relativeIntercardinals);
//			}

			middleVertex->setActive(false);
		}
		/*
		 auto c = 1.0f;
		 auto d2 = calculateRoughness(center, intercardinals);
		 auto f = l / (d * C * std::max(d2, 1.0f));

		 if (f < 1.0f) {
		 middleVertex->setActive(true);
		 } else {
		 middleVertex->setActive(false);
		 }
		 */
	}

	// Unset active if any vertex in the neighborhood differ by more than 1
	for (auto i = 0; i < 4; ++i) {
		auto& direction = *ID::getAtClockwiseIndex(i);

		auto& e = intercardinals[direction];

		auto middleVertex = mesh->getOrCreateChildVertex(center, e);

		if (middleVertex->isActive()) {
			auto neighborhood = getNeighborhood(middleVertex, direction,
					intercardinals, neighbors);

			bool differByOne = true;

			for (auto j = 0; j < 4; ++j) {
				if (neighborhood[*CD::getAtClockwiseIndex(j)]
						&& abs(
								(int) middleVertex->getLevel()
										- (int) neighborhood[*CD::getAtClockwiseIndex(
												j)]->getLevel()) > 1) {

//				Log("level: %d", abs(
//						(int)middleVertex->getLevel()
//								- (int)neighborhood[*CD::getAtClockwiseIndex(
//										j)]->getLevel()));

					differByOne = false;
					break;
				}
			}

			if (!differByOne) {
				middleVertex->setActive(false);
			}
		}
	}

	for (auto i = 0; i < 4; ++i) {

		auto& direction = *ID::getAtClockwiseIndex(i);

		auto& e = intercardinals[direction];

		auto middleVertex = mesh->getOrCreateChildVertex(center, e);

		if (middleVertex->isActive()) {

			auto temp = DA<ID, Vertex*>(ID::all(), nullptr);

			temp[*ID::getAtClockwiseIndex(i)] = e;

			temp[*ID::getAtClockwiseIndex((i + 1) % 4)] =
					mesh->getOrCreateChildVertex(e,
							intercardinals[*ID::getAtClockwiseIndex((i + 1) % 4)]);
			temp[*ID::getAtClockwiseIndex((i + 2) % 4)] = center;

			temp[*ID::getAtClockwiseIndex((i + 3) % 4)] =
					mesh->getOrCreateChildVertex(e,
							intercardinals[*ID::getAtClockwiseIndex((i + 3) % 4)]);

			updateActiveCenters(cameraPosition, middleVertex, temp,
					getNeighborhood(center, direction, intercardinals,
							neighbors));
		}
	}

//	for (auto i = 0; i < 4; ++i) {
//		auto direction = *ID::getAtClockwiseIndex(i);
//
//		if(intercardinals[direction]) {
//			intercardinals[direction]->setActive(center->isActive());
//		}
//	}
}

void Quadtree2::deleteUnusedVertices() {

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

void Quadtree2::update2(const Vec3f& cameraPosition, const std::string& tag) {

//	recursiveCalcRoughness(center, intercardinals, neighbors);

// TODO Check this code
	updateActiveCenters(cameraPosition, center, intercardinals, neighbors);
//	updateActives(cameraPosition, center, intercardinals, neighbors);

	updateActiveIntercardinals(center, intercardinals);

//	mesh->getTriangles()->DeleteAllElements();
//	mesh->getEdges()->DeleteAllElements();

//	deleteUnusedVertices();

//	Log("COUNT: %d", mesh->getTriangles()->Count());

	remesh(center, intercardinals, neighbors);
}

float Quadtree2::calculateRoughness(const Vertex* center,
		const DA<ID, Vertex*>& intercardinals) {

	float dh[6] { 0.0f };

	for (auto i = 0; i < 4; ++i) {
		auto v1 = intercardinals[*ID::getAtClockwiseIndex(i)];
		auto v2 = intercardinals[*ID::getAtClockwiseIndex((i + 1) % 4)];

		auto middle = mesh->getOrCreateChildVertex(v1, v2);

		auto elevation = Perlin::generate(middle->get());

		// TODO Change this formula
		auto elevationVector = Vec3f(middle->get().x(), middle->get().y(),
				middle->get().z() + (1.0 * elevation));

		Vec3f numerator;

		Vec3f::Cross3(numerator, (elevationVector - v1->get()),
				(elevationVector - v2->get()));

		// |(p - r1)x(p - r2)| / |r2 - r1|
		dh[i] = numerator.Length() / (v1->get() - v2->get()).Length();
	}

	for (auto i = 0; i < 2; ++i) {
		auto v1 = intercardinals[*ID::getAtClockwiseIndex(i)];
		auto v2 = intercardinals[*ID::getAtClockwiseIndex((i + 2) % 4)];

		auto elevation = Perlin::generate(center->get());

		// TODO Change this formula
		auto elevationVector = Vec3f(center->get().x(), center->get().y(),
				center->get().z() + (1.0 * elevation));

		Vec3f numerator;

		Vec3f::Cross3(numerator, (elevationVector - v1->get()),
				(elevationVector - v2->get()));

		dh[i + 4] = numerator.Length() / (v1->get() - v2->get()).Length();
	}

	auto d =
			(intercardinals[ID::NE]->get() - intercardinals[ID::NW]->get()).Length();

	return (*std::max_element(dh, dh + 6) / d);
}

void Quadtree2::remesh(Vertex* center, DA<ID, Vertex*>& intercardinals,
		const DA<CD, Vertex*>& neighbors, const std::string& tag) {

	std::vector<Triangle*> currentTriangles;

	Log("%s", tag.c_str());

	for (auto d : ID::all()) {

		auto edge = mesh->getEdge(intercardinals[*d], center);

		if (edge) {
			currentTriangles = mesh->getTrianglesByVertex(edge);
			break;
		}
	}

//	Log("******************************************");
//	Log("%lu", currentTriangles.size());
//	Log("******************************************");

	DA<ID, Vertex*> middleInterCardinals { { ID::NW,
			mesh->getOrCreateChildVertex(center, intercardinals[ID::NW]) },
			{ ID::NE, mesh->getOrCreateChildVertex(center,
					intercardinals[ID::NE]) },
			{ ID::SE, mesh->getOrCreateChildVertex(center,
					intercardinals[ID::SE]) },
			{ ID::SW, mesh->getOrCreateChildVertex(center,
					intercardinals[ID::SW]) } };

//	DA<ID, bool> updated { { ID::NW, !mesh->getOrCreateChildVertex(center,
//			intercardinals[ID::NW])->isActive() },
//			{ ID::NE, !mesh->getOrCreateChildVertex(center,
//					intercardinals[ID::NE])->isActive() }, { ID::SE,
//					!mesh->getOrCreateChildVertex(center,
//							intercardinals[ID::SE])->isActive() }, { ID::SW,
//					!mesh->getOrCreateChildVertex(center,
//							intercardinals[ID::SW])->isActive() } };
//
//	Log("[%s] -> NW: %d, NE: %d, SE: %d, SW: %d", tag.c_str(), updated[ID::NW],
//			updated[ID::NE], updated[ID::SE], updated[ID::SW]);

//	if (currentTriangles.size() == 0 && center->isActive()) {
	if (center->isActive()) {
		for (auto i = 0; i < 4; ++i) {
			Log("%s [%d]", tag.c_str(), i);

			auto& c = *CD::getAtClockwiseIndex(i);

			auto& ic = *ID::getAtClockwiseIndex(i);
			auto& nextIc = *ID::getAtClockwiseIndex((i + 1) % 4);

			auto middle = mesh->getOrCreateChildVertex(intercardinals[ic],
					intercardinals[nextIc]);

			if (neighbors[c] && !neighbors[c]->isActive()) {
//				Log("%s", tag.c_str());

				if (!middleInterCardinals[ic]->isActive()
						&& !middleInterCardinals[nextIc]->isActive()) {
//					Log("%s", tag.c_str());

					mesh->addTriangle(center, intercardinals[ic],
							intercardinals[nextIc]);

				} else if (middleInterCardinals[ic]->isActive()) {
//					Log("%s", tag.c_str());

//					mesh->addTriangle(center, intercardinals[ic], middle);

//					mesh->addTriangle(center, middle, intercardinals[ic]);
					mesh->addTriangle(center, middle, intercardinals[nextIc]);
				} else {
//					Log("%s", tag.c_str());

					mesh->addTriangle(center, middle, intercardinals[ic]);
//					mesh->addTriangle(center, middle, intercardinals[nextIc]);
				}
			} else {
//				Log("%s", tag.c_str());

				if (!middleInterCardinals[ic]->isActive()) {
					mesh->addTriangle(center, middle, intercardinals[ic]);
				}

				if (!middleInterCardinals[nextIc]->isActive()) {
					mesh->addTriangle(center, middle, intercardinals[nextIc]);
				}
			}
		}
	}

	// Get vertices that are subdivided
	for (auto i = 0; i < 4; ++i) {
		auto& direction = *ID::getAtClockwiseIndex(i);

//		if (!updated[direction]) {
		if (mesh->getOrCreateChildVertex(center, intercardinals[direction])->isActive()) {
			auto relativeIntercardinal = getRelativeIntercardinals(direction,
					center, intercardinals);

			remesh(middleInterCardinals[direction], relativeIntercardinal,
					getNeighborhood(center, direction, intercardinals,
							neighbors),
					tag + " > " + std::to_string(direction));
		}
	}
}

DA<ID, Vertex*> Quadtree2::getRelativeIntercardinalsWithoutCreate(
		const ID& direction, Vertex* center,
		DA<ID, Vertex*>& parentIntercardinals) {

	auto i = direction.getClockwiseIndex();

	const ID* directions[] = { ID::getAtClockwiseIndex(i),
			ID::getAtClockwiseIndex((i + 1) % 4), ID::getAtClockwiseIndex(
					(i + 2) % 4), ID::getAtClockwiseIndex((i + 3) % 4) };

	return {
		{
			*directions[0],
			parentIntercardinals[*directions[0]]
		}, {
			*directions[1],
			mesh->getChildVertex(parentIntercardinals[*directions[0]],
					parentIntercardinals[*directions[1]])
		}, {
			*directions[2], center
		}, {
			*directions[3],
			mesh->getChildVertex(parentIntercardinals[*directions[0]],
					parentIntercardinals[*directions[3]])
		}
	};
}

DA<ID, Vertex*> Quadtree2::getRelativeIntercardinals(const ID& direction,
		Vertex* center, DA<ID, Vertex*>& parentIntercardinals) {

	auto i = direction.getClockwiseIndex();

	const ID* directions[] = { ID::getAtClockwiseIndex(i),
			ID::getAtClockwiseIndex((i + 1) % 4), ID::getAtClockwiseIndex(
					(i + 2) % 4), ID::getAtClockwiseIndex((i + 3) % 4) };

	return {
		{
			*directions[0],
			parentIntercardinals[*directions[0]]
		}, {
			*directions[1],
//			mesh->getChildVertex(parentIntercardinals[*directions[0]],
//								parentIntercardinals[*directions[1]])
			mesh->getOrCreateChildVertex(parentIntercardinals[*directions[0]],
					parentIntercardinals[*directions[1]])
		}, {
			*directions[2], center
		}, {
			*directions[3],
//			mesh->getChildVertex(parentIntercardinals[*directions[0]],
//								parentIntercardinals[*directions[3]])
			mesh->getOrCreateChildVertex(parentIntercardinals[*directions[0]],
					parentIntercardinals[*directions[3]])
		}
	};
}

DA<CD, Vertex*> Quadtree2::getNeighborhood(Vertex* center, const ID& direction,
		const DA<ID, Vertex*>& intercardinals,
		const DA<CD, Vertex*>& neighbors) {

	auto tempNeighbors = DA<CD, Vertex*>(CD::all(), nullptr);

	auto i = direction.getClockwiseIndex();

	tempNeighbors[*CD::getAtClockwiseIndex(i)] =
			neighbors[*CD::getAtClockwiseIndex(i)];

	tempNeighbors[*CD::getAtClockwiseIndex((i + 1) % 4)] =
			mesh->getOrCreateChildVertex(center,
					intercardinals[*ID::getAtClockwiseIndex((i + 1) % 4)]);

	tempNeighbors[*CD::getAtClockwiseIndex((i + 2) % 4)] =
			mesh->getOrCreateChildVertex(center,
					intercardinals[*ID::getAtClockwiseIndex((i + 3) % 4)]);

	tempNeighbors[*CD::getAtClockwiseIndex((i + 3) % 4)] =
			neighbors[*CD::getAtClockwiseIndex((i + 3) % 4)];

	return tempNeighbors;
}

void Quadtree2::update(const Vec3f& cameraPosition, const std::string& tag) {

	DA<ID, bool> marked(ID::all(), false);

	Log("%s", tag.c_str());

	for (auto& e : ID::all()) {
		if (children[*e]) {
			children[*e]->update(cameraPosition,
					tag + " > " + std::to_string(e->getClockwiseIndex()));
		}
	}

	Log("%s", tag.c_str());

// Mark who children must be exist
	for (int i = 0; i < 4; ++i) {
		auto& e = intercardinals[*ID::getAtClockwiseIndex(i)];

		auto middlePoint = mesh->getChildVertex(center, e)->get();

		auto d =
				((intercardinals[ID::NE]->get() + intercardinals[ID::NW]->get())
						* 0.5).Length();
		auto l = (middlePoint - cameraPosition).Length();

		if (l / d < C) {
// Remove edges
			marked[*ID::getAtClockwiseIndex(i)] = true;

		}
	}

	Log("%s", tag.c_str());

// Update the children
	for (int i = 0; i < 4; ++i) {

		auto& enumValue = *ID::getAtClockwiseIndex(i);

		Log("%s %d", tag.c_str(), i);

		if (marked[*ID::getAtClockwiseIndex(i)] && !children[enumValue]) {

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
								intercardinals[*ID::getAtClockwiseIndex(
										(i + 3) % 4)],
								intercardinals[enumValue]);
					}

					Log("%s", tag.c_str());

					if (edgeDeleted) {
						mesh->addTriangle(center,
								intercardinals[*ID::getAtClockwiseIndex(
										(i + 1) % 4)],
								intercardinals[enumValue]);
					}
				}

				Log("%s", tag.c_str());
			}

			Log("%s", tag.c_str());

			{
				auto& cd1 = *CD::getAtClockwiseIndex(i);
				auto& cd2 = *CD::getAtClockwiseIndex((i + 3) % 4);

				auto& icd1 = *ID::getAtClockwiseIndex((i + 1) % 4);
				auto& icd2 = *ID::getAtClockwiseIndex((i + 3) % 4);

				DA<CD, bool> localMarked(CD::all(), false);

				// TODO Add more checks here
				if (!neighbors[cd1]) {
					localMarked[cd1] = false;
				}

				if (!neighbors[cd2]) {
					localMarked[cd2] = false;
				}

				localMarked[*CD::getAtClockwiseIndex((i + 1) % 4)] =
						marked[icd1];

				localMarked[*CD::getAtClockwiseIndex((i + 2) % 4)] =
						marked[icd2];

				// NW, NE, SE, SW
				Vertex* v[4] { nullptr };
				Vertex* localCenter = mesh->getChildVertex(center,
						intercardinals[*ID::getAtClockwiseIndex(i)]);

				v[i] = intercardinals[*ID::getAtClockwiseIndex(i)];

				v[(i + 2) % 4] = center;

				v[(i + 1) % 4] = mesh->getChildVertex(
						intercardinals[*ID::getAtClockwiseIndex(i)],
						intercardinals[*ID::getAtClockwiseIndex((i + 1) % 4)]);

				v[(i + 3) % 4] = mesh->getChildVertex(
						intercardinals[*ID::getAtClockwiseIndex(i)],
						intercardinals[*ID::getAtClockwiseIndex((i + 3) % 4)]);

				children[enumValue] = new Quadtree2(v[0], v[1], v[3], v[2],
						localCenter, mesh, localMarked, this);
			}

			Log("%s", tag.c_str());

		} else if (!marked[*ID::getAtClockwiseIndex(i)]
				&& !children[enumValue]) {

			if (!mesh->getEdge(center, intercardinals[enumValue])) {

			}

		} else if (!marked[*ID::getAtClockwiseIndex(i)]
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

void Quadtree2::recursiveDeleteVertices(Vertex* center,
		const IntercardinalDirection& direction,
		DirectionArray<IntercardinalDirection, Vertex*>& intercardinals,
		std::size_t level) {

	for (int i = 0; i < 4; ++i) {
		auto& direction = *ID::getAtClockwiseIndex(i);
		auto& e = intercardinals[direction];

		if (!e) {
			Log("CONTINUE, PORRA");
			continue;
		}

		auto middleVertex = mesh->getChildVertex(center, e);

		if (middleVertex) {

			auto relativeIntercardinals =
					getRelativeIntercardinalsWithoutCreate(direction, center,
							intercardinals);

			recursiveDeleteVertices(middleVertex, direction,
					relativeIntercardinals, level + 1);

			auto deleted = mesh->deleteChildIfExist(center, e);

			{
				auto& ic1 = *ID::getAtClockwiseIndex(i);
				auto& ic2 = *ID::getAtClockwiseIndex((i + 1) % 4);

				if (intercardinals[ic1] && intercardinals[ic2]) {
					mesh->deleteChildIfExist(intercardinals[ic1],
							intercardinals[ic2]);
				}
			}

			if (level > 0) {
				mesh->removeVertex(center);
			}
		}
	}
}

#define K (C / (2 * (C - 1)))

float Quadtree2::recursiveCalcRoughness(Vertex* center,
		DA<ID, Vertex*>& intercardinals, DA<CD, Vertex*>& neighbors) {

	auto newD2 = std::numeric_limits<float>::infinity();
	auto currentD2 = calcRoughness(center, intercardinals);
	float iD2[4] { 0.0f };

	/*
	 for (int i = 0; i < 4; ++i) {
	 auto& direction = *ID::getAtClockwiseIndex(i);

	 auto id = intercardinals[direction];

	 if (id) {
	 auto relativeCenter = mesh->getChildVertex(center, id);

	 if (relativeCenter) {
	 auto relativeIntercardinals = getRelativeIntercardinals(
	 direction, center, intercardinals);

	 iD2[i] = recursiveCalcRoughness(relativeCenter,
	 relativeIntercardinals);
	 }
	 }
	 }
	 */

	for (int i = 0; i < 4; ++i) {
		auto& direction = *CD::getAtClockwiseIndex(i);

		auto n = neighbors[direction];

		if (n) {

			DA<ID, Vertex*> relativeIntercardinals { { ID::NW,
					n->getParents1()->getParent1() }, { ID::SE,
					n->getParents1()->getParent2() }, { ID::NE,
					n->getParents2()->getParent1() }, { ID::SW,
					n->getParents2()->getParent2() } };

//				auto relativeNeighbors = getNeighborhood(n, )
		}
//			auto id = intercardinals[direction];
//
//			if (id) {
//				auto relativeCenter = mesh->getChildVertex(center, id);
//
//				if (relativeCenter) {
//					auto relativeIntercardinals = getRelativeIntercardinals(
//							direction, center, intercardinals);
//
//					iD2[i] = recursiveCalcRoughness(relativeCenter,
//							relativeIntercardinals);
//				}
//			}
	}

	for (auto id2 : iD2) {
		if ((id2 / currentD2) < K && ((id2 / currentD2) < newD2)) {
			newD2 = currentD2; //(id2 / currentD2);
		}
	}

	if (newD2 < std::numeric_limits<float>::infinity()) {

		auto temp = newD2 * K;

		center->setD2(temp);

		for (auto& d : ID::all()) {
			intercardinals[*d]->setD2(temp);
		}
	}

	return newD2;
}

void Quadtree2::updateRoughness() {
	recursiveUpdateRoughness(center, intercardinals);
}

void Quadtree2::recursiveUpdateRoughness(Vertex* center,
		DA<ID, Vertex*>& intercardinals) {

	for (int i = 0; i < 4; ++i) {
		auto& direction = *ID::getAtClockwiseIndex(i);

		auto id = intercardinals[direction];

		auto relCenter = mesh->getChildVertex(id, center);

		if (relCenter) {
			auto relIntercardinals = getRelativeIntercardinals(direction,
					center, intercardinals);

			recursiveUpdateRoughness(relCenter, relIntercardinals);
		}
	}

	center->setD2(calcRoughness(center, intercardinals));
}

void Quadtree2::updateRoughness2() {
	recursiveUpdateRoughness2(center, intercardinals, nullptr, &neighbors);
}

float Quadtree2::recursiveUpdateRoughness2(Vertex* center,
		DirectionArray<IntercardinalDirection, Vertex*>& intercardinals,
		DirectionArray<CardinalDirection, Vertex*>* parentNeighbors,
		DirectionArray<CardinalDirection, Vertex*>* neighbors) {

	bool hasNext = false;
	float nextRoughness = -1.0f;

	for (int i = 0; i < 4; ++i) {
		auto& id = *ID::getAtClockwiseIndex(i);

		auto relCenter = mesh->getChildVertex(center, intercardinals[id]);

		if (relCenter) {
			auto relIntercardinals = getRelativeIntercardinals(id, center,
					intercardinals);

			auto relNeighbors = getNeighborhood(center, id, intercardinals,
					*neighbors);

//			if(!parentNeighbors) {
			nextRoughness = recursiveUpdateRoughness2(relCenter,
					relIntercardinals, neighbors, &relNeighbors);
//			} else {
//				nextRoughness = recursiveUpdateRoughness2(relCenter, relIntercardinals,
//									relNeighbors);
//			}

//			auto relNeighbors = getNeighborhood(center, id, intercardinals,
//					parentNeighbors);
//
//			nextRoughness = recursiveUpdateRoughness2(relCenter, relIntercardinals,
//					relNeighbors);
		} else {
			hasNext = true;
		}
	}

	auto minD2 = std::numeric_limits<float>::max();

	if (parentNeighbors) {
		for (int i = 0; i < 4; ++i) {
			auto& cd = *CD::getAtClockwiseIndex(i);

			if ((*parentNeighbors)[cd]->getD2() < minD2) {
				minD2 = (*parentNeighbors)[cd]->getD2();
			}

//		if (parentNeighbors[cd]->getD2() < minD2) {
//			minD2 = parentNeighbors[cd]->getD2();
//		}
		}
	}

	float currRoughness = 0.0f;

	if ((minD2 / center->getD2()) < K) {

		if (nextRoughness >= 0.0f) {
			currRoughness = nextRoughness * K;
		} else {
			currRoughness = center->getD2();
		}

		for (int i = 0; i < 4; ++i) {
			auto& id = *ID::getAtClockwiseIndex(i);

			if (intercardinals[id]) {
				intercardinals[id]->setD2(currRoughness);
			}
		}
	}

	return currRoughness;
}

//Quadtree2::Quadtree2(Vertex* nw, Vertex* ne, Vertex* sw, Vertex* se,
//		Vertex* _center, Mesh* _mesh,
//		const DA<CD, bool>& marked,
//		const DA<CD, bool>& willHaveNeighbor) :
//		parent(nullptr), mesh(_mesh), intercardinals(
//				ID::getAll(), nullptr), children(
//				ID::getAll(), nullptr), center(_center) {

Quadtree2::Quadtree2(Vertex* nw, Vertex* ne, Vertex* sw, Vertex* se,
		Vertex* _center, QuadtreeMesh* _mesh, const DA<CD, bool>& marked,
		Quadtree2* _parent) :
		parent(_parent), mesh(_mesh), intercardinals { { ID::NW, nw }, { ID::NE,
				ne }, { ID::SW, sw }, { ID::SE, se } }, children(ID::all(),
				nullptr), neighbors(CD::all(), nullptr), center(_center) {

	for (int i = 0; i < 4; ++i) {
		auto& cd = *CD::getAtClockwiseIndex(i);

		auto& current_icd = *ID::getAtClockwiseIndex(i);
		auto& next_icd = *ID::getAtClockwiseIndex((i + 1) % 4);

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
		auto& d1 = intercardinals[*ID::getAtClockwiseIndex(i)];
		auto& d2 = intercardinals[*ID::getAtClockwiseIndex((i + 1) % 4)];

		if (!mesh->getChildVertex(d1, d2)) {

			auto tempVertex = mesh->addVertex(MIDDLE(d1->get(), d2->get()));

			mesh->setParentsChild(d1, d2, tempVertex);
		}
	}

// Create middle points between center and intercardinals
	for (int i = 0; i < 4; ++i) {
		auto& d1 = intercardinals[*ID::getAtClockwiseIndex(i)];

		if (!mesh->getChildVertex(center, d1)) {
			auto tempVertex = mesh->addVertex(MIDDLE(center->get(), d1->get()));

			mesh->setParentsChild(d1, center, tempVertex);
		}
	}
}

void Quadtree2::setNeighbours(Vertex* n, Vertex* e, Vertex* s, Vertex* w) {
	neighbors[CD::N] = n;
	neighbors[CD::E] = e;
	neighbors[CD::S] = s;
	neighbors[CD::W] = w;
}

float Quadtree2::calcRoughness(Vertex* center,
		DirectionArray<IntercardinalDirection, Vertex*>& intercardinals) {

	float roughness = 0;
	float d =
			(intercardinals[ID::NW]->get() - intercardinals[ID::NE]->get()).Length();

	for (int i = 0; i < 4; ++i) {

		auto id1 = intercardinals[*ID::getAtClockwiseIndex(i)];
		auto id2 = intercardinals[*ID::getAtClockwiseIndex((i + 1) % 4)];

//		auto cd = mesh->getOrCreateChildVertex(id1, id2);
		auto cd = MIDDLE(id1->get(), id2->get());

//		auto r = abs(
//				math::distanceFromPointToLine(id1->get(), id2->get(),
//						cd->get()));

		auto r = abs(math::distanceFromPointToLine(id1->get(), id2->get(), cd));

		if (r > roughness) {
			roughness = r;
		}
	}

	for (int i = 0; i < 2; ++i) {
		auto id1 = intercardinals[*ID::getAtClockwiseIndex(i)];
		auto id2 = intercardinals[*ID::getAtClockwiseIndex((i + 2) % 4)];

//		Log("%p %p %p", id1, id2, center);

		auto r = abs(
				math::distanceFromPointToLine(id1->get(), id2->get(),
						center->get()));

		if (r > roughness) {
			roughness = r;
		}
	}

	return roughness / d;
}

#undef MIDDLE
#undef CAST

