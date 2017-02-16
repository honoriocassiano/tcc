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

template<class DType, class Element>
using DA = DirectionArray<DType, Element>;

using ID = IntercardinalDirection;
using CD = CardinalDirection;

float Quadtree2::C = 20;

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

void Quadtree2::updateActiveIntercardinals(Vertex* center,
		DA<ID, Vertex*>& intercardinals) {

	auto centerIsActive = center->isActive();

	for (auto i = 0; i < 4; ++i) {
		auto direction = *ID::getAtClockwiseIndex(i);

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
		auto direction = *ID::getAtClockwiseIndex(i);

		center->setActive(centerIsActive);

		if (intercardinals[direction]) {
			intercardinals[direction]->setActive(centerIsActive);
		}
	}
}

void Quadtree2::updateActiveCenters(const Vec3f& cameraPosition, Vertex* center,
		DA<ID, Vertex*>& intercardinals, const DA<CD, Vertex*>& neighbors) {

	for (auto i = 0; i < 4; ++i) {

		auto direction = *ID::getAtClockwiseIndex(i);

		auto& e = intercardinals[direction];

//		auto middleVertex = mesh->getChildVertex(center, e);
		auto middleVertex = mesh->getOrCreateChildVertex(center, e);

//		if (middleVertex->isActive()) {
//
//			auto temp = DA<ID, Vertex*>(
//					ID::getAll(), nullptr);
//
//			temp[*ID::getAtClockwiseIndex(i)] = e;
//
//			temp[*ID::getAtClockwiseIndex((i + 1) % 4)] =
//					mesh->getOrCreateChildVertex(e,
//							intercardinals[*ID::getAtClockwiseIndex(
//									(i + 1) % 4)]);
//			temp[*ID::getAtClockwiseIndex((i + 2) % 4)] =
//					center;
//
//			temp[*ID::getAtClockwiseIndex((i + 3) % 4)] =
//					mesh->getOrCreateChildVertex(e,
//							intercardinals[*ID::getAtClockwiseIndex(
//									(i + 3) % 4)]);
//
//			updateActives(cameraPosition, middleVertex, temp,
//					getNeighborhood(center, direction, intercardinals,
//							neighbors));
//		}

		auto d =
				(intercardinals[ID::NE]->get() - intercardinals[ID::NW]->get()).Length();

		auto l = (middleVertex->get() - cameraPosition).Length();

		// TODO Change this value
		if ((l / d) < C) {
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
		auto direction = *ID::getAtClockwiseIndex(i);

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

		auto direction = *ID::getAtClockwiseIndex(i);

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

		auto vp = v->getParents();

		if (vp) {
			if (!(vp->getParent1()->isActive() && vp->getParent2()->isActive())) {
				mesh->deleteChildIfExist(vp->getParent1(), vp->getParent2());
			}
		}
	}
}

void Quadtree2::update2(const Vec3f& cameraPosition, const std::string& tag) {

	// TODO Check this code
	updateActiveCenters(cameraPosition, center, intercardinals, neighbors);
//	updateActives(cameraPosition, center, intercardinals, neighbors);

	updateActiveIntercardinals(center, intercardinals);

	mesh->getTriangles()->DeleteAllElements();
	mesh->getEdges()->DeleteAllElements();

	deleteUnusedVertices();

	Log("COUNT: %d", mesh->getTriangles()->Count());

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

//	mesh->getTriangles()->DeleteAllElements();

	Log("%s", tag.c_str());

	for (auto d : ID::all()) {

		//auto edge = mesh->getEdge(center, intercardinals[*d]);
		auto edge = mesh->getEdge(intercardinals[*d], center);

		if (edge) {
			currentTriangles = mesh->getTrianglesByVertex(edge);
			break;
		}
	}

	Log("%s", tag.c_str());

//	DA<ID, bool> currentSubdivided(ID::all(), false);

	Log("%s", tag.c_str());

//	DA<ID, bool> willBeSubdivided { { ID::NW, mesh->getOrCreateChildVertex(
//			center, intercardinals[ID::NW])->isActive() },
//			{ ID::NE, mesh->getOrCreateChildVertex(center,
//					intercardinals[ID::NE])->isActive() },
//			{ ID::SE, mesh->getOrCreateChildVertex(center,
//					intercardinals[ID::SE])->isActive() },
//			{ ID::SW, mesh->getOrCreateChildVertex(center,
//					intercardinals[ID::SW])->isActive() } };

	Log("%s", tag.c_str());

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

	DA<ID, bool> updated { { ID::NW, !mesh->getOrCreateChildVertex(center,
			intercardinals[ID::NW])->isActive() },
			{ ID::NE, !mesh->getOrCreateChildVertex(center,
					intercardinals[ID::NE])->isActive() }, { ID::SE,
					!mesh->getOrCreateChildVertex(center,
							intercardinals[ID::SE])->isActive() }, { ID::SW,
					!mesh->getOrCreateChildVertex(center,
							intercardinals[ID::SW])->isActive() } };

	Log("[%s] -> NW: %d, NE: %d, SE: %d, SW: %d", tag.c_str(), updated[ID::NW],
			updated[ID::NE], updated[ID::SE], updated[ID::SW]);

//	if (currentTriangles.size() == 0 && center->isActive()) {
	if (center->isActive()) {
		for (auto i = 0; i < 4; ++i) {
			Log("%s [%d]", tag.c_str(), i);

			auto c = *CD::getAtClockwiseIndex(i);

			auto ic = *ID::getAtClockwiseIndex(i);
			auto nextIc = *ID::getAtClockwiseIndex((i + 1) % 4);

			auto middle = mesh->getOrCreateChildVertex(intercardinals[ic],
					intercardinals[nextIc]);

			if (neighbors[c] && !neighbors[c]->isActive()) {
				Log("%s", tag.c_str());

				if (!middleInterCardinals[ic]->isActive()
						&& !middleInterCardinals[nextIc]->isActive()) {
					Log("%s", tag.c_str());

					mesh->addTriangle(center, intercardinals[ic],
							intercardinals[nextIc]);

				} else if (middleInterCardinals[ic]->isActive()) {
					Log("%s", tag.c_str());

//					mesh->addTriangle(center, intercardinals[ic], middle);
					mesh->addTriangle(center, middle, intercardinals[ic]);
				} else {
					Log("%s", tag.c_str());

					mesh->addTriangle(center, middle, intercardinals[nextIc]);
				}
			} else {
				Log("%s", tag.c_str());

//				auto tempMid = mesh->getOrCreateChildVertex(center,
//						intercardinals[ic]);
//
//				if (!tempMid->isActive()) {
				if (!middleInterCardinals[ic]->isActive()) {
//					mesh->addTriangle(center, intercardinals[ic], middle);
					mesh->addTriangle(center, middle, intercardinals[ic]);
				}

				Log("%s", tag.c_str());

//				tempMid = mesh->getOrCreateChildVertex(center,
//						intercardinals[nextIc]);

//				if (!tempMid->isActive()) {
				Log("%p", middleInterCardinals[nextIc]);

				if (!middleInterCardinals[nextIc]->isActive()) {
					mesh->addTriangle(center, middle, intercardinals[nextIc]);
				}
			}
		}
	}

	// Get vertices that are subdivided
	for (auto i = 0; i < 4; ++i) {
		auto direction = *ID::getAtClockwiseIndex(i);

		if (!updated[direction]) {
			auto relativeIntercardinal = getRelativeIntercardinals(direction,
					center, intercardinals);

			remesh(middleInterCardinals[direction], relativeIntercardinal,
					getNeighborhood(center, direction, intercardinals,
							neighbors),
					tag + " > " + std::to_string(direction));
		}

//		auto intCenter = mesh->getOrCreateChildVertex(center,
//				intercardinals[direction]);
//
//		auto relativeInter = getRelativeIntercardinals(direction, center,
//				intercardinals);
//
//		std::vector<Triangle*> localTriangles;

//		for (auto d : ID::getAll()) {
//			auto edge = mesh->getEdge(relativeInter[*d], intCenter);
//
//			if (edge) {
//				localTriangles = mesh->getTrianglesByVertex(edge);
//				break;
//			}
//		}
//
//		if (localTriangles.size() > 0) {
//			currentSubdivided[direction] = true;
//		}
//
//		// Update mesh
//		if (currentSubdivided[direction] && !willBeSubdivided[direction]) {
//			Log("%s", tag.c_str());
//
//			for (auto& t : localTriangles) {
//				mesh->removeTriangle(t);
//			}
//
//			mesh->addTriangle(center, intercardinals[direction],
//					relativeInter[*ID::getAtClockwiseIndex(
//							(i + 1) % 4)]);
//			mesh->addTriangle(center,
//					relativeInter[*ID::getAtClockwiseIndex(
//							(i + 3) % 4)], intercardinals[direction]);
//
//		} else if (!currentSubdivided[direction]
//				&& willBeSubdivided[direction]) {
//			Log("%s", tag.c_str());
//
//			auto e1 = mesh->getEdge(center, intercardinals[direction]);
//			auto e2 = mesh->getEdge(intercardinals[direction], center);
//
//			Log("%s", tag.c_str());
//
//			if (e1 && e1->getTriangle()) {
//				mesh->removeTriangle(e1->getTriangle());
//			}
//
//			Log("%s", tag.c_str());
//
//			if (e2 && e2->getTriangle()) {
//				mesh->removeTriangle(e2->getTriangle());
//			}
//
//			Log("%s", tag.c_str());
//
//			auto middleInter = mesh->getOrCreateChildVertex(center,
//					intercardinals[direction]);
//
//			remesh(middleInter, relativeInter,
//					getNeighborhood(center, direction, intercardinals,
//							neighbors),
//					tag + " > " + std::to_string(direction));
//
//		} else if (currentSubdivided[direction]
//				&& willBeSubdivided[direction]) {
//			Log("%s", tag.c_str());
//
//			for (auto j = 0; j < 4; ++j) {
//				auto ic = *ID::getAtClockwiseIndex(j);
//				auto c = *CD::getAtClockwiseIndex(j);
//
//				auto cVertex =
//						mesh->getChildVertex(relativeInter[ic],
//								relativeInter[*ID::getAtClockwiseIndex(
//										(j + 1) % 4)]);
//
//				auto edge = mesh->getEdge(center, cVertex);
//				auto edgeOp = mesh->getEdge(cVertex, center);
//
//				if ((neighbors[c] && !neighbors[c]->isActive())
//						^ (edge == nullptr)) {
//
//					if (!edge) {
//						mesh->removeTriangle(edge->getTriangle());
//						mesh->removeTriangle(edgeOp->getTriangle());
//
//						mesh->addTriangle(center, relativeInter[ic],
//								relativeInter[*ID::getAtClockwiseIndex(
//										(j + 1) % 4)]);
//					} else {
//
//						auto nextIc =
//								*ID::getAtClockwiseIndex(
//										(j + 1) % 4);
//
//						edge = mesh->getEdge(relativeInter[ic],
//								relativeInter[nextIc]);
//						edgeOp = mesh->getEdge(relativeInter[nextIc],
//								relativeInter[ic]);
//
//						if (edge && edge->getNext()->getVertex() == center) {
//
//							mesh->removeTriangle(edge->getTriangle());
//
//						} else if (edgeOp
//								&& edgeOp->getNext()->getVertex() == center) {
//
//							mesh->removeTriangle(edgeOp->getTriangle());
//						}
//
//						mesh->addTriangle(center, cVertex, relativeInter[ic]);
//
//						mesh->addTriangle(center, relativeInter[nextIc],
//								cVertex);
//					}
//				}
//			}
//
//			for (auto& dirTemp : ID::getAll()) {
//
//				auto middleTemp = mesh->getOrCreateChildVertex(center,
//						intercardinals[*dirTemp]);
//
//				remesh(middleTemp, relativeInter,
//						getNeighborhood(center, *dirTemp, intercardinals,
//								neighbors),
//						tag + " > " + std::to_string(*dirTemp));
//			}
//
//		} // Else, do nothing;

	}

//	Log("%s", tag.c_str());

//	Log("******************************************");
//	Log("%lu", currentTriangles.size());
//	Log("******************************************");

// First subdivision case
//	if (currentTriangles.size() == 0 && center->isActive()) {
//		for (auto i = 0; i < 4; ++i) {
//			Log("%s [%d]", tag.c_str(), i);
//
//			auto c = *CD::getAtClockwiseIndex(i);
//
//			auto ic = *ID::getAtClockwiseIndex(i);
//			auto nextIc = *ID::getAtClockwiseIndex(
//					(i + 1) % 4);
//
//			auto middle = mesh->getOrCreateChildVertex(intercardinals[ic],
//					intercardinals[nextIc]);
//
//			if (neighbors[c] && !neighbors[c]->isActive()) {
//				Log("%s", tag.c_str());
//
//				mesh->addTriangle(center, intercardinals[ic],
//						intercardinals[nextIc]);
//			} else {
//				Log("%s", tag.c_str());
//
//				auto tempMid = mesh->getOrCreateChildVertex(center,
//						intercardinals[ic]);
//
//				if (!tempMid->isActive()) {
//					mesh->addTriangle(center, intercardinals[ic], middle);
//				}
//
//				Log("%s", tag.c_str());
//
//				tempMid = mesh->getOrCreateChildVertex(center,
//						intercardinals[nextIc]);
//
//				if (!tempMid->isActive()) {
//					mesh->addTriangle(center, middle, intercardinals[nextIc]);
//				}
//			}
//		}
//	}

	/*
	 for (auto i = 0; i < 4; ++i) {
	 auto direction = *ID::getAtClockwiseIndex(i);

	 if (currentSubdivided[direction]
	 && !willBeSubdivided[direction]) {



	 } else if (!currentSubdivided[direction]
	 && willBeSubdivided[direction]) {

	 } else if (currentSubdivided[direction]
	 && willBeSubdivided[direction]) {

	 }// Else, do nothing;
	 }
	 */
}

/*
 void Quadtree2::remesh(Vertex* center,
 const DA<ID, Vertex*>& intercardinals,
 const DA<CD, Vertex*>& neighbors,
 const std::string& tag) {

 Log("/****************************************************");
 Log("/************** Entering on function ****************");
 Log("/****************************************************");

 {
 auto edge = mesh->getEdge(intercardinals[ID::NW],
 intercardinals[ID::SE]);

 if (!edge) {
 edge = mesh->getEdge(intercardinals[ID::NE],
 intercardinals[ID::SW]);
 }

 if (edge) {
 auto opp = edge->getOpposite();

 mesh->removeTriangle(edge->getTriangle());

 if (opp) {
 mesh->removeTriangle(opp->getTriangle());
 }
 }
 }

 for (auto i = 0; i < ID::getCount(); ++i) {
 auto middleVertex =
 mesh->getOrCreateChildVertex(center,
 intercardinals[*ID::getAtClockwiseIndex(
 i)]);
 //				mesh->getChildVertex(center,
 //						intercardinals[*ID::getAtClockwiseIndex(
 //								i)]);

 Log("%s %p", tag.c_str(), middleVertex);

 if (middleVertex->isActive()) {

 Log("%s", tag.c_str());

 auto direction = *ID::getAtClockwiseIndex(i);

 auto& e = intercardinals[direction];

 auto tempIntercardinals = DA<ID,
 Vertex*>(ID::getAll(), nullptr);
 auto tempNeighbors = DA<CD, Vertex*>(
 CD::getAll(), nullptr);

 Log("%s", tag.c_str());

 // Setting intercardinals
 tempIntercardinals[*ID::getAtClockwiseIndex(i)] =
 e;

 tempIntercardinals[*ID::getAtClockwiseIndex(
 (i + 1) % 4)] = mesh->getOrCreateChildVertex(e,
 intercardinals[*ID::getAtClockwiseIndex(
 (i + 1) % 4)]);

 tempIntercardinals[*ID::getAtClockwiseIndex(
 (i + 2) % 4)] = center;

 tempIntercardinals[*ID::getAtClockwiseIndex(
 (i + 3) % 4)] = mesh->getOrCreateChildVertex(e,
 intercardinals[*ID::getAtClockwiseIndex(
 (i + 3) % 4)]);

 Log("%s", tag.c_str());

 // Setting neighbors
 tempNeighbors[*CD::getAtClockwiseIndex(i)] =
 neighbors[*CD::getAtClockwiseIndex(i)];

 tempNeighbors[*CD::getAtClockwiseIndex((i + 1) % 4)] =
 mesh->getOrCreateChildVertex(center,
 intercardinals[*ID::getAtClockwiseIndex(
 (i + 1) % 4)]);

 tempNeighbors[*CD::getAtClockwiseIndex((i + 2) % 4)] =
 mesh->getOrCreateChildVertex(center,
 intercardinals[*ID::getAtClockwiseIndex(
 (i + 3) % 4)]);

 tempNeighbors[*CD::getAtClockwiseIndex((i + 3) % 4)] =
 neighbors[*CD::getAtClockwiseIndex(
 (i + 3) % 4)];

 Log("%s", tag.c_str());

 remesh(middleVertex, tempIntercardinals, tempNeighbors,
 tag + " > " + std::to_string(i));
 }
 }

 Log("%s", tag.c_str());

 for (auto i = 0; i < CD::getCount(); ++i) {
 auto direction = *CD::getAtClockwiseIndex(i);

 auto v1 =
 intercardinals[*ID::getAtClockwiseIndex(i)];
 auto v2 = intercardinals[*ID::getAtClockwiseIndex(
 (i + 1) % 4)];

 // Check if not have neighbor
 //		if ( !neighbors[direction] || neighbors[direction]->isActive() ) {
 if ((!neighbors[direction] || neighbors[direction]->isActive())
 && !mesh->getOrCreateChildVertex(center, v1)->isActive()) {
 Log("%s", tag.c_str());

 auto cardinalMiddle = mesh->getOrCreateChildVertex(v1, v2);

 Log("%p %p", mesh->getEdge(center, cardinalMiddle),
 mesh->getEdge(cardinalMiddle, center));
 Log("%s %s", center->get().str().c_str(),
 cardinalMiddle->get().str().c_str());

 // TODO If this condition must be an AND or OR
 //			if (!(mesh->getEdge(center, cardinalMiddle)
 //					&& mesh->getEdge(cardinalMiddle, center))) {
 //
 //				mesh->addTriangle(center, v1, cardinalMiddle);
 //				mesh->addTriangle(center, v2, cardinalMiddle);
 //			}

 auto edge1 = mesh->getEdge(center, cardinalMiddle);
 auto edge2 = mesh->getEdge(cardinalMiddle, center);

 //			Log("ABC: %p %p", edge1, edge2);
 //			Log("ABC: %s %s", center->get().str().c_str(),
 //					cardinalMiddle->get().str().c_str());
 //
 //			if (edge2) {
 //				Log("ABC: %p", edge2->getOpposite());
 //			}

 //			if ((edge1 == nullptr) ^ (edge2 == nullptr)) {
 //
 //				if (edge1) {
 //					mesh->removeTriangle(edge1->getTriangle());
 //					edge1 = nullptr;
 //				}
 //
 //				if (edge2) {
 //					mesh->removeTriangle(edge2->getTriangle());
 //					edge2 = nullptr;
 //				}
 //			}

 //			if (!(mesh->getEdge(center, cardinalMiddle)
 //					&& mesh->getEdge(cardinalMiddle, center))) {

 if (!edge1 && !edge2) {
 mesh->addTriangle(center, v1, cardinalMiddle);
 mesh->addTriangle(center, cardinalMiddle, v2);
 }
 //			}

 } else if (neighbors[direction] && !neighbors[direction]->isActive()) {

 Log("%s", tag.c_str());

 //			auto tempEdge = mesh->getEdge(center, v1);
 auto tempEdge = mesh->getEdge(v1, v2);
 auto exist = false;
 //			auto abc = true;
 auto numberTriangles = 0;

 {
 if (tempEdge && (tempEdge->getNext()->getVertex() == center)) {
 exist = true;
 } else {

 //					abc = abc && (tempEdge != nullptr);
 tempEdge = mesh->getEdge(v2, v1);

 Log("%p %p %p",
 (tempEdge ? tempEdge->getVertex() : nullptr),
 center, v1);

 //					abc = abc && (tempEdge != nullptr);
 }

 Log("%s", tag.c_str());
 if (!exist && tempEdge
 && (tempEdge->getNext()->getVertex() == center)) {
 Log("%s", tag.c_str());
 exist = true;
 }

 Log("%s", tag.c_str());

 if (exist) {
 //				if (!exist && abc) {
 numberTriangles = 1;
 //				mesh->addTriangle(center, v1, v2);
 }
 }
 Log("%s", tag.c_str());

 if (!numberTriangles) {
 tempEdge = mesh->getEdge(center,
 mesh->getOrCreateChildVertex(v1, v2));

 Log("%s", tag.c_str());
 if (tempEdge) {
 exist = true;
 } else {
 tempEdge = mesh->getEdge(
 mesh->getOrCreateChildVertex(v1, v2), center);
 }

 Log("%s", tag.c_str());

 if (!exist && tempEdge) {
 exist = true;
 }

 if (exist) {
 numberTriangles = 2;
 }

 Log("%s", tag.c_str());
 }

 if (numberTriangles == 0) {
 //				Log("%s %d", tag.c_str(), i);
 //				Log("%p %p", mesh->getEdge(v1, v2), mesh->getEdge(v2, v1));
 //				Log("t1: %s\tt2: %s",
 //						mesh->getEdge(v1, v2)->getTriangle()->str().c_str(),
 //						mesh->getEdge(v2, v1)->getTriangle()->str().c_str());
 ////				Log("%p %p", mesh->getEdge(v1, v2)->getVertex(), mesh->getEdge(v2, v1)->getVertex());
 //				Log("%p %p %p", v1, v2, center);
 //				Log("%s", center->get().str().c_str());
 //				Log("%s %s", v1->get().str().c_str(), v2->get().str().c_str());

 mesh->addTriangle(center, v1, v2);

 } else if (numberTriangles == 2) {
 Log("%s %d", tag.c_str(), i);

 auto opp = tempEdge->getOpposite();

 mesh->removeTriangle(tempEdge->getTriangle());
 mesh->removeTriangle(opp->getTriangle());

 mesh->addTriangle(center, v1, v2);
 }

 ////			if (tempEdge && (tempEdge->getNext()->getVertex() == v2)) {
 //			if (tempEdge && (tempEdge->getNext()->getVertex() == center)) {
 //				Log("%s", tag.c_str());
 //				exist = true;
 //			} else {
 //
 ////				abc = abc && (tempEdge != nullptr);
 //
 //				Log("%s", tag.c_str());
 //
 //				Log("%p %p %p", (tempEdge ? tempEdge->getVertex() : nullptr),
 //						center, v1);
 //				Log("%p %p %p",
 //						((tempEdge && tempEdge->getOpposite()) ?
 //								tempEdge->getOpposite()->getVertex() : nullptr),
 //						center, v1);
 //				Log("%s %s %s", center->get().str().c_str(),
 //						v1->get().str().c_str(), v2->get().str().c_str());
 //
 ////				tempEdge = mesh->getEdge(v1, center);
 //				tempEdge = mesh->getEdge(v2, v1);
 //
 //				Log("%p %p %p", (tempEdge ? tempEdge->getVertex() : nullptr),
 //						center, v1);
 //
 ////				abc = abc && (tempEdge != nullptr);
 //			}
 //
 ////			Log("%d %p %d", (!exist), tempEdge,
 ////					true);//(tempEdge->getNext()->getVertex() == v2));
 //
 ////			if (!exist && tempEdge
 ////					&& (tempEdge->getNext()->getVertex() == v2)) {
 //			if (!exist && tempEdge
 //					&& (tempEdge->getNext()->getVertex() == center)) {
 //				Log("%s", tag.c_str());
 //				exist = true;
 //			}
 //
 ////			Log("%s %s %s", center->get().str().c_str(),
 ////					v1->get().str().c_str(), v2->get().str().c_str());
 //
 ////			if (!exist && abc) {
 //			if (!exist) {
 //				mesh->addTriangle(center, v1, v2);
 //			}
 }
 }

 Log("%s", tag.c_str());
 }
 */

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
				auto ic1 = *ID::getAtClockwiseIndex(i);
				auto ic2 = *ID::getAtClockwiseIndex((i + 1) % 4);

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

#undef MIDDLE
#undef CAST

