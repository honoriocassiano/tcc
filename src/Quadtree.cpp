#include "Quadtree.h"
#include "Object.h"

#include "Debug.h"

#include <math.h>
#include <algorithm>

using namespace std;
using namespace directions;

void Quadtree::subdivide(Intercardinal point, const std::string& tag,
		bool propagate) {

#define INVERT(S) (static_cast<QuadtreeData::Side>(1 - static_cast<int>( S )))

	using namespace directions;

	Edge* edge1 = nullptr;
	Edge* edge2 = nullptr;

	auto sideTriangle1 = QuadtreeData::LEFT;
	auto sideTriangle2 = QuadtreeData::LEFT;

	if (children[point]) {
		Error("\"Unsplitable\" quadtree!");
		return;
	}

	auto adjacents = getAdjacents(point);

	{
		auto v1 = getRelativeVertex(point, adjacents[0]);
		auto v2 = getRelativeVertex(point, adjacents[1]);

//		if (propagate) {
		if (!v1) {
			getRelativeVertex(point, adjacents[0], true);

			v1 = vertices[adjacents[0]];
		}

		if (!v2) {
			getRelativeVertex(point, adjacents[1], true);

			v2 = vertices[adjacents[1]];
		}

		edge1 = mesh->getEdge(vertices[point], v1);
		edge2 = mesh->getEdge(vertices[point], v2);

		if (!propagate) {
			if ((!edge1 || (edge1 && (findPosition(edge1->getTriangle()) == -1)))
					&& (!mesh->getEdge(v1, vertices[point])
							|| (mesh->getEdge(v1, vertices[point])
									&& (findPosition(
											mesh->getEdge(v1, vertices[point])->getTriangle())
											== -1)))) {

				v1 = vertices[adjacents[0]];
				edge1 = mesh->getEdge(vertices[point], v1);
			}

			if ((!edge2 || (edge2 && (findPosition(edge2->getTriangle()) == -1)))
					&& (!mesh->getEdge(v2, vertices[point])
							|| (mesh->getEdge(v2, vertices[point])
									&& (findPosition(
											mesh->getEdge(v2, vertices[point])->getTriangle())
											== -1)))) {

				v2 = vertices[adjacents[1]];
				edge2 = mesh->getEdge(vertices[point], v2);
			}
		}

		Log("%s %d: %p %p", tag.c_str(), point, edge1, edge2);
//		Log("%s %d: %d", tag.c_str(), point, edge1->getTriangle());

		if (!edge1 || (edge1 && (findPosition(edge1->getTriangle()) == -1))) {
//			Log("SDF %d", findPosition(edge1->getTriangle()));

			edge1 = mesh->getEdge(v1, vertices[point]);

//			Log("SDF %d", findPosition(edge1->getTriangle()));
		}

		Log("%s %d: %p %p", tag.c_str(), point, edge1, edge2);
		if (!edge2 || (edge2 && (findPosition(edge2->getTriangle()) == -1))) {
			edge2 = mesh->getEdge(v2, vertices[point]);
		}

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

	Log("%s: AAA %d %d", tag.c_str(), positionT1, positionT2);
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

//	Log("C");

	children[point] = new Quadtree(middlePoint1, vertices[point], middle,
			middlePoint2, level + 1, maxLevel, mesh, this);

	// Subdivide neighbours
	if (propagate) {

		directions::Intercardinal neighborSide1 =
				static_cast<QuadtreeData*>(neighborTriangle1->getUserData())->data[INVERT(
						sideTriangle1)];

		directions::Intercardinal neighborSide2 =
				static_cast<QuadtreeData*>(neighborTriangle2->getUserData())->data[INVERT(
						sideTriangle2)];

		if (neighborTree1 && !neighborTree1->getChild(neighborSide1)) {
			neighborTree1->subdivide(neighborSide1, "neighbour1", false);
		}

		if (neighborTree2 && !neighborTree2->getChild(neighborSide2)) {
			neighborTree2->subdivide(neighborSide2, "neighbour2", false);
		}

	} else {
		// todo check this
	}

	// TODO Check this 'this'

//	children[point] = new Quadtree(middlePoint1, vertices[point], middle,
//			middlePoint2, level + 1, maxLevel, mesh, this);

//	children[point] = new Quadtree(vertices[point], middlePoint1, middle, middlePoint2,
//			level + 1, maxLevel, mesh, this);

	if (propagate) {

		auto temp1 = static_cast<QuadtreeData*>(userDataTriangle1)->data[INVERT(
				sideTriangle1)];
		auto temp2 = static_cast<QuadtreeData*>(userDataTriangle2)->data[INVERT(
				sideTriangle2)];

		Log("BABABA: %p", children[temp1]);

//		if (!children[temp1]) {
//			subdivide(temp1, "children 1", false);
//		}
//
//		if (!children[temp2]) {
//			subdivide(temp2, "children 2", false);
//		}
	}

//	if (children[point]) {
//		Error("\"Unsplitable\" quadtree!");
//		return;
//	}
//
//	Log("%s : %d", tag.c_str(), point);
//
//	Edge* edge1 = nullptr;
//	Edge* edge2 = nullptr;
//
//	QuadtreeData::Side sideTriangle1 = QuadtreeData::Side::LEFT;
//	QuadtreeData::Side sideTriangle2 = QuadtreeData::Side::LEFT;
//
//	switch (point) {
//	case Intercardinal::NW: {
//
//		Vertex* tempNE = nullptr;
////				mesh->getChildVertex(vertices[NW], vertices[NE]) ?
////						mesh->getChildVertex(vertices[NW], vertices[NE]) :
////						vertices[NE];
//
//		Vertex* tempSW = nullptr;
////				mesh->getChildVertex(vertices[SW], vertices[NW]) ?
////						mesh->getChildVertex(vertices[SW], vertices[NW]) :
////						vertices[SW];
//
//		if (!propagate) {
//			tempNE = mesh->getChildVertex(vertices[NW], vertices[NE]);
//			tempSW = mesh->getChildVertex(vertices[SW], vertices[NW]);
//
//			//**************************************************
//			if (!tempNE) {
//				auto middlePos = (vertices[NW]->get() + vertices[NE]->get())
//						* 0.5;
//
//				tempNE = mesh->addVertex(middlePos);
//
//				mesh->setParentsChild(vertices[NW], vertices[NE], tempNE);
//
//				tempNE = vertices[NE];
//			}
//
//			if (!tempSW) {
//				auto middlePos = (vertices[SW]->get() + vertices[NW]->get())
//						* 0.5;
//
//				tempSW = mesh->addVertex(middlePos);
//
//				mesh->setParentsChild(vertices[SW], vertices[NW], tempSW);
//
//				tempSW = vertices[SW];
//			}
//			//**************************************************
//		} else {
//			tempNE = vertices[NE];
//			tempSW = vertices[SW];
//		}
//
//		edge1 = mesh->getEdge(vertices[NW], tempNE);
//		edge2 = mesh->getEdge(tempSW, vertices[NW]);
//
//		Edge* temp1 = nullptr;
//		Edge* temp2 = nullptr;
//
//		if (!edge1 || (edge1 && (edge1->getTriangle() != triangles[N]))) {
//			temp1 = mesh->getEdge(tempNE, vertices[NW]);
//
//			edge1 = temp1;
//		}
//
//		if (!edge2 || ((edge2 && (edge2->getTriangle() != triangles[W])))) {
//			temp2 = mesh->getEdge(vertices[NW], tempSW);
//
//			edge2 = temp2;
//		}
//
//		break;
//	}
//	case Intercardinal::NE: {
//
//		Vertex* tempNW = nullptr;
////				mesh->getChildVertex(vertices[NW], vertices[NE]) ?
////						mesh->getChildVertex(vertices[NW], vertices[NE]) :
////						vertices[NW];
//
//		Vertex* tempSE = nullptr;
////				mesh->getChildVertex(vertices[SE], vertices[NE]) ?
////						mesh->getChildVertex(vertices[SE], vertices[NE]) :
////						vertices[SE];
//
//		if (!propagate) {
//			tempNW = mesh->getChildVertex(vertices[NW], vertices[NE]);
//			tempSE = mesh->getChildVertex(vertices[SE], vertices[NE]);
//
//			//**************************************************
//			if (!tempNW) {
//				auto middlePos = (vertices[NW]->get() + vertices[NE]->get())
//						* 0.5;
//
//				tempNW = mesh->addVertex(middlePos);
//
//				mesh->setParentsChild(vertices[NW], vertices[NE], tempNW);
//
//				tempNW = vertices[NW];
//			}
//
//			if (!tempSE) {
//				auto middlePos = (vertices[NE]->get() + vertices[SE]->get())
//						* 0.5;
//
//				tempSE = mesh->addVertex(middlePos);
//
//				mesh->setParentsChild(vertices[NW], vertices[SE], tempSE);
//
//				tempSE = vertices[SE];
//			}
//			//**************************************************
//		} else {
//			tempNW = vertices[NW];
//			tempSE = vertices[SE];
//		}
//
////		edge1 = mesh->getEdge(vertices[NW], vertices[NE]);
////		edge2 = mesh->getEdge(vertices[SE], vertices[NE]);
//		edge1 = mesh->getEdge(tempNW, vertices[NE]);
//		edge2 = mesh->getEdge(tempSE, vertices[NE]);
//
//		Log("%p %p", edge1, edge2);
//		Edge* temp1 = nullptr;
//		Edge* temp2 = nullptr;
//
//		if (!edge1 || (edge1 && (edge1->getTriangle() != triangles[N]))) {
//			temp1 = mesh->getEdge(vertices[NE], tempNW);
//
//			edge1 = temp1;
//		}
//
//		if (!edge2 || (edge2 && (edge2->getTriangle() != triangles[E]))) {
//			temp2 = mesh->getEdge(vertices[NE], tempSE);
//
//			edge2 = temp2;
//		}
//		Log("%p %p", edge1, edge2);
//
////		if (!edge1 || (edge1 && (edge1->getTriangle() != triangles[N]))) {
////			temp1 = mesh->getEdge(vertices[NE], vertices[NW]);
////		}
////
////		if (!edge2 || (edge2 && (edge2->getTriangle() != triangles[E]))) {
////			temp2 = mesh->getEdge(vertices[NE], vertices[SE]);
////		}
//
//		break;
//	}
//	case Intercardinal::SW: {
//
//		Vertex* tempNW = nullptr;
////				mesh->getChildVertex(vertices[NW], vertices[SW]) ?
////						mesh->getChildVertex(vertices[NW], vertices[SW]) :
////						vertices[NW];
//
//		Vertex* tempSE = nullptr;
////				mesh->getChildVertex(vertices[SW], vertices[SE]) ?
////						mesh->getChildVertex(vertices[SW], vertices[SE]) :
////						vertices[SE];
//
//		if (!propagate) {
//			tempNW = mesh->getChildVertex(vertices[NW], vertices[SW]);
//			tempSE = mesh->getChildVertex(vertices[SW], vertices[SE]);
//
//			//**************************************************
//			if (!tempNW) {
//				auto middlePos = (vertices[NW]->get() + vertices[SW]->get())
//						* 0.5;
//
//				tempNW = mesh->addVertex(middlePos);
//
//				mesh->setParentsChild(vertices[NW], vertices[SW], tempNW);
//
//				tempNW = vertices[NW];
//			}
//
//			if (!tempSE) {
//				auto middlePos = (vertices[SW]->get() + vertices[SE]->get())
//						* 0.5;
//
//				Log("AAAAAAA");
//
//				//tempSE = mesh->addVertex(middlePos);
//
////				mesh->setParentsChild(vertices[SW], vertices[SE], tempSE);
//				mesh->setParentsChild(vertices[SW], vertices[SE], mesh->addVertex(middlePos));
//
//				tempSE = vertices[SE];
//			}
//			//**************************************************
//
//		} else {
//			tempNW = vertices[NW];
//			tempSE = vertices[SE];
//		}
//
//		Log("%p %p", vertices[SE], tempSE);
//		Log("%p %p", vertices[NW], tempNW);
//
//		edge1 = mesh->getEdge(vertices[SW], tempSE);
//		edge2 = mesh->getEdge(vertices[SW], tempNW);
//
////		Log("%p %p", edge1, edge2);
//
////		Log("%p, %p", vertices[SE], tempSE);
//
//		Log("%p %p, %p %p", mesh->getEdge(vertices[SW], tempSE),
//				mesh->getEdge(tempSE, vertices[SW]),
//				mesh->getEdge(vertices[SW], vertices[SE]),
//				mesh->getEdge(vertices[SE], vertices[SW]));
////		Log("%p %p", mesh->getEdge(vertices[SW], vertices[SE]),
////				mesh->getEdge(vertices[SE], vertices[SW]));
//
//		Edge* temp1 = nullptr;
//		Edge* temp2 = nullptr;
//
//		if (!edge1 || (edge1 && (edge1->getTriangle() != triangles[S]))) {
//			temp1 = mesh->getEdge(tempSE, vertices[SW]);
//
//			edge1 = temp1;
//		}
//
//		if (!edge2 || (edge2 && (edge2->getTriangle() != triangles[W]))) {
//			temp2 = mesh->getEdge(tempNW, vertices[SW]);
//
//			edge2 = temp2;
//		}
//
////		Log("%p %p", edge1, edge2);
//
//		break;
//	}
//	case Intercardinal::SE: {
//
//		Vertex* tempNE = nullptr;
////		mesh->getChildVertex(vertices[NE], vertices[SE]) ?
////				mesh->getChildVertex(vertices[NE], vertices[SE]) : vertices[NE];
//
//		Vertex* tempSW = nullptr;
////				mesh->getChildVertex(vertices[SW], vertices[SE]) ?
////						mesh->getChildVertex(vertices[SW], vertices[SE]) :
////						vertices[SW];
//
//		if (!propagate) {
//			tempNE = mesh->getChildVertex(vertices[NE], vertices[SE]);
//			tempSW = mesh->getChildVertex(vertices[SW], vertices[SE]);
//
//			//**************************************************
//			if (!tempNE) {
//				auto middlePos = (vertices[NE]->get() + vertices[SE]->get())
//						* 0.5;
//
//				tempNE = mesh->addVertex(middlePos);
//
//				mesh->setParentsChild(vertices[NE], vertices[SE], tempNE);
//			}
//
//			if (!tempSW) {
//				auto middlePos = (vertices[SW]->get() + vertices[SE]->get())
//						* 0.5;
//
//				tempSW = mesh->addVertex(middlePos);
//
//				mesh->setParentsChild(vertices[SW], vertices[SE], tempSW);
//			}
//			//**************************************************
//
//		} else {
//			tempNE = vertices[NE];
//			tempSW = vertices[SW];
//		}
//
//		edge1 = mesh->getEdge(tempSW, vertices[SE]);
//		edge2 = mesh->getEdge(vertices[SE], tempNE);
//
//		Edge* temp1 = nullptr;
//		Edge* temp2 = nullptr;
//
//		if (!edge1 || (edge1 && (edge1->getTriangle() != triangles[S]))) {
//			temp1 = mesh->getEdge(vertices[SE], tempSW);
//
//			edge1 = temp1;
//		}
//
//		if (!edge2 || (edge2 && (edge2->getTriangle() != triangles[E]))) {
//			temp2 = mesh->getEdge(tempNE, vertices[SE]);
//
//			edge2 = temp2;
//		}
//
//		break;
//	}
//
//	}
//
//	{
//		Log("%d %p", point, edge1);
//		auto tempS1 =
//				static_cast<QuadtreeData*>(edge1->getTriangle()->getUserData());
//		auto tempS2 =
//				static_cast<QuadtreeData*>(edge2->getTriangle()->getUserData());
//
//		if (tempS1->data[sideTriangle1] != point) {
//			sideTriangle1 = INVERT(sideTriangle1);
//		}
//
//		if (tempS2->data[sideTriangle2] != point) {
//			sideTriangle2 = INVERT(sideTriangle2);
//		}
//	}
//
//	Triangle* neighborTriangle1 =
//			(edge1->getOpposite()) ?
//					edge1->getOpposite()->getTriangle() : nullptr;
//	Triangle* neighborTriangle2 =
//			(edge2->getOpposite()) ?
//					edge2->getOpposite()->getTriangle() : nullptr;
//
//	Quadtree* neighborTree1 =
//			edge1->getOpposite() ?
//					static_cast<QuadtreeData*>(neighborTriangle1->getUserData())->tree :
//					nullptr;
//
//	Quadtree* neighborTree2 =
//			edge2->getOpposite() ?
//					static_cast<QuadtreeData*>(neighborTriangle2->getUserData())->tree :
//					nullptr;
//
//	QuadtreeData::Side neighborSideTriangle1 = INVERT(sideTriangle1);
//	QuadtreeData::Side neighborSideTriangle2 = INVERT(sideTriangle2);
//
//	Vertex * v11 = edge1->getVertex();
//	Vertex * v12 = edge1->getNext()->getNext()->getVertex();
//
//	Vertex* v21 = edge2->getVertex();
//	Vertex* v22 = edge2->getNext()->getNext()->getVertex();
//
//	Vertex * middlePoint1 = mesh->getChildVertex(v11, v12);
//	Vertex * middlePoint2 = mesh->getChildVertex(v21, v22);
//
//	if (!middlePoint1) {
//		auto middlePos = (v11->get() + v12->get()) * 0.5;
//
//		middlePoint1 = mesh->addVertex(middlePos);
//
//		mesh->setParentsChild(v11, v12, middlePoint1);
//	}
//
//	if (!middlePoint2) {
//		auto middlePos = (v21->get() + v22->get()) * 0.5;
//
//		middlePoint2 = mesh->addVertex(middlePos);
//
//		mesh->setParentsChild(v21, v22, middlePoint2);
//	}
//
//	int positionT1 = findPosition(edge1->getTriangle());
//	int positionT2 = findPosition(edge2->getTriangle());
//
//	auto userDataTriangle1 = triangles[positionT1]->getUserData();
//	auto userDataTriangle2 = triangles[positionT2]->getUserData();
//
//	mesh->removeTriangle(triangles[positionT1]);
//	triangles[positionT1] = nullptr;
//
//	mesh->removeTriangle(triangles[positionT2]);
//	triangles[positionT2] = nullptr;
//
//	if (!children[static_cast<QuadtreeData*>(edge1->getTriangle()->getUserData())->data[INVERT(
//			sideTriangle1)]]) {
//
//		triangles[positionT1] =
//				mesh->addTriangle(
//						vertices[static_cast<QuadtreeData*>(userDataTriangle1)->data[INVERT(
//								sideTriangle1)]], middlePoint1, middle);
//
////		triangles[positionT1] = mesh->addTriangle(
////				middlePoint1, vertices[static_cast<QuadtreeData*>(userDataTriangle1)->data[INVERT(sideTriangle1)]],
////						middle);
//
//		triangles[positionT1]->setUserData(userDataTriangle1);
//	}
//
//	if (!children[static_cast<QuadtreeData*>(edge2->getTriangle()->getUserData())->data[INVERT(
//			sideTriangle2)]]) {
//
//		triangles[positionT2] =
//				mesh->addTriangle(
//						vertices[static_cast<QuadtreeData*>(userDataTriangle2)->data[INVERT(
//								sideTriangle2)]], middlePoint2, middle);
//
//		triangles[positionT2]->setUserData(userDataTriangle2);
//	}
//
//	Log("C");
//
//	// Subdivide neighbours
//	if (propagate) {
//
//		directions::Intercardinal neighborSide1 =
//				static_cast<QuadtreeData*>(neighborTriangle1->getUserData())->data[INVERT(
//						sideTriangle1)];
//
//		directions::Intercardinal neighborSide2 =
//				static_cast<QuadtreeData*>(neighborTriangle2->getUserData())->data[INVERT(
//						sideTriangle2)];
//
//		if (neighborTree1 && !neighborTree1->getChild(neighborSide1)) {
//			neighborTree1->subdivide(neighborSide1, "neighbour 1", false);
//		}
//
//		if (neighborTree2 && !neighborTree2->getChild(neighborSide2)) {
//			neighborTree2->subdivide(neighborSide2, "neighbour 2", false);
//		}
//
//	} else {
//		// todo check this
//	}
//
//	// TODO Check this 'this'
//
//	children[point] = new Quadtree(middlePoint1, vertices[point], middle,
//			middlePoint2, level + 1, maxLevel, mesh, this);
//
////	children[point] = new Quadtree(vertices[point], middlePoint1, middle, middlePoint2,
////			level + 1, maxLevel, mesh, this);
//
//	if (propagate) {
//
//		auto temp1 = static_cast<QuadtreeData*>(userDataTriangle1)->data[INVERT(
//				sideTriangle1)];
//		auto temp2 = static_cast<QuadtreeData*>(userDataTriangle2)->data[INVERT(
//				sideTriangle2)];
//
//		Log("BABABA: %p", children[temp1]);
//
//		if (!children[temp1]) {
//			subdivide(temp1, "children 1", false);
//		}
//
//		if (!children[temp2]) {
//			subdivide(temp2, "children 2", false);
//		}
//	}
//
//	Log("%s A", tag.c_str());

#undef INVERT
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

Vertex* Quadtree::getChildVertex(directions::Intercardinal d1,
		directions::Intercardinal d2) {

	return mesh->getChildVertex(vertices[d1], vertices[d2]);
}

Vertex* Quadtree::getRelativeVertex(directions::Intercardinal outer,
		directions::Intercardinal inner, bool createIfNotExists) {

#define CAST(X, TYPE) static_cast<TYPE>(X)

	if (outer == inner) {
		return vertices[outer];
	}

	if (abs(3 - CAST(outer, int)) == CAST(inner, int)) {
		return middle;
	}

	if (abs(CAST(inner, int)- CAST(outer, int)) == 1 ) {
		auto tempDir = (CAST(inner, int) + CAST(outer, int)) / 2;

		auto vertex = mesh->getChildVertex(vertices[tempDir], vertices[tempDir+1]);

		if(createIfNotExists && !vertex) {
			vertex = mesh->addVertex( (vertices[tempDir]->get() + vertices[tempDir+1]->get()) * 0.5 );

			mesh->setParentsChild(vertices[tempDir], vertices[tempDir+1], vertex);
		}

		return vertex;
	}

	if (abs(CAST(inner, int)- CAST(outer, int)) == 2 ) {
		auto tempDir = (CAST(inner, int) + CAST(outer, int)) / 2;

		auto vertex = mesh->getChildVertex(vertices[tempDir-1], vertices[tempDir+1]);

		if(createIfNotExists && !vertex) {
			vertex = mesh->addVertex( (vertices[tempDir-1]->get() + vertices[tempDir+1]->get()) * 0.5 );

			mesh->setParentsChild(vertices[tempDir-1], vertices[tempDir+1], vertex);
		}

		return vertex;
	}

	return nullptr;
}

std::array<directions::Intercardinal, 2> Quadtree::getAdjacents(
		directions::Intercardinal point) {

	int p1;
	int p2;

	if ( CAST(point, int)%2 == 0 ) {
		// NW and SW
		p1 = point + 1;

	} else {
		// NE and SE
		p1 = point - 1;
	}

	p2 = (point + 2) % 4;

	return std::array<directions::Intercardinal, 2> { CAST(p1,
			directions::Intercardinal), CAST(p2, directions::Intercardinal) };
}

#undef CAST
