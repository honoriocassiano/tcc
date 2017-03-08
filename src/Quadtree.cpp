/*
 * Quadtree2.cpp
 *
 *  Created on: 19 de nov de 2016
 *      Author: cassiano
 */

#include <math.h>
#include "Quadtree.h"
#include <algorithm>

#include "Perlin.h"
#include "MathUtils.hpp"

template<class DType, class Element>
using DA = DirectionArray<DType, Element>;

using ID = IntercardinalDirection;
using CD = CardinalDirection;

float Quadtree::C = 15;
float Quadtree::c = 20;

#define MIDDLE(P1, P2) ( (P1 + P2 ) * 0.5)
#define CAST(X, TYPE) static_cast<TYPE>(X)

Quadtree::Quadtree(Vertex* nw, Vertex* ne, Vertex* sw, Vertex* se,
		QuadtreeMesh * _mesh) :
		parent(nullptr), mesh(_mesh), intercardinals(ID::all(), nullptr), children(
				ID::all(), nullptr), neighbors(CD::all(), nullptr) {

	// TODO Delete this code when the new constructor will be created
	intercardinals[ID::NW] = nw;
	intercardinals[ID::NE] = ne;
	intercardinals[ID::SW] = sw;
	intercardinals[ID::SE] = se;

	auto temp1 = (nw->get() + ne->get()) * 0.5;
	auto temp2 = (sw->get() + se->get()) * 0.5;

	auto m = (temp1 + temp2) * 0.5;

	center = mesh->addVertex(m);
	center->setActive(true);

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

Quadtree::~Quadtree() {
	delete center;

	for (auto& e : ID::all()) {
//	for (int i = 0; i < ID::getAll().size(); ++i) {

//		auto e = *ID::getAtClockwiseIndex(i);

//		delete intercardinals[*e];
	}
}

#define K (C / (2 * (C - 1)))

void Quadtree::updateActiveIntercardinals(Vertex* center,
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

void Quadtree::updateActiveCenters(const Vec3f& cameraPosition, Vertex* center,
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
	/*
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
	 */

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

void Quadtree::deleteUnusedVertices() {

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

void Quadtree::update(const Vec3f& cameraPosition, const std::string& tag) {

// TODO Check this code
	updateActiveCenters(cameraPosition, center, intercardinals, neighbors);

	updateActiveIntercardinals(center, intercardinals);

	remesh(center, intercardinals, neighbors);
}

void Quadtree::remesh(Vertex* center, DA<ID, Vertex*>& intercardinals,
		const DA<CD, Vertex*>& neighbors, const std::string& tag) {

	DA<ID, Vertex*> middleInterCardinals { { ID::NW,
			mesh->getOrCreateChildVertex(center, intercardinals[ID::NW]) },
			{ ID::NE, mesh->getOrCreateChildVertex(center,
					intercardinals[ID::NE]) },
			{ ID::SE, mesh->getOrCreateChildVertex(center,
					intercardinals[ID::SE]) },
			{ ID::SW, mesh->getOrCreateChildVertex(center,
					intercardinals[ID::SW]) } };

//	if (currentTriangles.size() == 0 && center->isActive()) {
	if (center->isActive()) {
		for (auto i = 0; i < 4; ++i) {

			auto& c = *CD::getAtClockwiseIndex(i);

			auto& ic = *ID::getAtClockwiseIndex(i);
			auto& nextIc = *ID::getAtClockwiseIndex((i + 1) % 4);

			auto middle = mesh->getOrCreateChildVertex(intercardinals[ic],
					intercardinals[nextIc]);

			if (neighbors[c] && !neighbors[c]->isActive()) {

				if (!middleInterCardinals[ic]->isActive()
						&& !middleInterCardinals[nextIc]->isActive()) {

					mesh->addTriangle(center, intercardinals[ic],
							intercardinals[nextIc]);

				} else if (middleInterCardinals[ic]->isActive()
						^ middleInterCardinals[nextIc]->isActive()) {

					mesh->addTriangle(center, middle,
							intercardinals[ic]->isActive() ?
									intercardinals[nextIc] :
									intercardinals[ic]);
				}
			} else {

				if (!middleInterCardinals[ic]->isActive()
						|| !middleInterCardinals[nextIc]->isActive()) {

					if (!middleInterCardinals[ic]->isActive()) {
						mesh->addTriangle(center, middle, intercardinals[ic]);
					}

					if (!middleInterCardinals[nextIc]->isActive()) {
						mesh->addTriangle(center, middle,
								intercardinals[nextIc]);
					}
				}
			}
		}
	}

	// Get vertices that are subdivided
	for (auto i = 0; i < 4; ++i) {
		auto& direction = *ID::getAtClockwiseIndex(i);

//		if (!updated[direction]) {
		if (middleInterCardinals[direction]
				&& middleInterCardinals[direction]->isActive()) {

			auto relativeIntercardinal = getRelativeIntercardinals(direction,
					center, intercardinals);

			remesh(middleInterCardinals[direction], relativeIntercardinal,
					getNeighborhood(center, direction, intercardinals,
							neighbors),
					tag + " > " + std::to_string(direction));
		}
	}
}

DA<ID, Vertex*> Quadtree::getRelativeIntercardinalsWithoutCreate(
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

DA<ID, Vertex*> Quadtree::getRelativeIntercardinals(const ID& direction,
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

DA<CD, Vertex*> Quadtree::getNeighborhood(Vertex* center, const ID& direction,
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

void Quadtree::recursiveDeleteVertices(Vertex* center,
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

void Quadtree::updateRoughnessTopDown() {
	recursiveUpdateRoughness(center, intercardinals);
}

void Quadtree::recursiveUpdateRoughness(Vertex* center,
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

	center->setD2(calculateRoughness(center, intercardinals));
}

void Quadtree::updateRoughnessBottomUp() {
	recursiveUpdateRoughness2(center, intercardinals, nullptr, &neighbors);
}

float Quadtree::recursiveUpdateRoughness2(Vertex* center,
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

void Quadtree::setNeighbours(Vertex* n, Vertex* e, Vertex* s, Vertex* w) {
	neighbors[CD::N] = n;
	neighbors[CD::E] = e;
	neighbors[CD::S] = s;
	neighbors[CD::W] = w;
}

float Quadtree::calculateRoughness(Vertex* center,
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

#undef K
#undef MIDDLE
#undef CAST

