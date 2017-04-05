/*
 * WorldMesh.cpp
 *
 *  Created on: Apr 5, 2017
 *      Author: mint
 */

#include "WorldMesh.h"

#define SIZE(A) (sizeof(A) / sizeof(A[0]))

WorldMesh::WorldMesh(float _radius) :
		radius(_radius) {

	double t = (1.0 + sqrt(5.0)) / 2.0;

	Vec3f basePoints[] =
			{ { -1, t, 0 }, { 1, t, 0 }, { -1, -t, 0 }, { 1, -t, 0 },
					{ 0, -1, t }, { 0, 1, t }, { 0, -1, -t }, { 0, 1, -t }, { t,
							0, -1 }, { t, 0, 1 }, { -t, 0, -1 }, { -t, 0, 1 } };

	std::size_t baseIndices[] = { 0, 11, 5, 0, 5, 1, 0, 1, 7, 0, 7, 10, 0, 10,
			11, 1, 5, 9, 5, 11, 4, 11, 10, 2, 10, 7, 6, 7, 1, 8, 3, 9,
			4, 3, 4, 2, 3, 2, 6, 3, 6, 8, 3, 8, 9, 4, 9, 5, 2, 4, 11, 6, 2, 10,
			8, 6, 7, 9, 8, 1 };

//	std::size_t baseIndices[] = { { 0, 11, 5 }, { 0, 5, 1 }, { 0, 1, 7 }, { 0,
//			7, 10 }, { 0, 10, 11 }, { 1, 5, 9 }, { 5, 11, 4 }, { 11, 10, 2 }, {
//			10, 7, 6 }, { 10, 7, 6 }, { 7, 1, 8 }, { 3, 9, 4 }, { 3, 4, 2 }, {
//			3, 2, 6 }, { 3, 6, 8 }, { 3, 8, 9 }, { 4, 9, 5 }, { 2, 4, 11 }, { 6,
//			2, 10 }, { 8, 6, 7 }, { 9, 8, 1 } };

	Vertex* baseVertices[12] { nullptr };

	for (auto i = 0; i < SIZE(basePoints); ++i) {
		baseVertices[i] = addVertex(basePoints[i], 0);
	}

	for (auto i = 0; i < SIZE(baseIndices); i += 3) {
		addTriangle(baseVertices[baseIndices[i]],
				baseVertices[baseIndices[i + 1]],
				baseVertices[baseIndices[i + 2]]);
	}
}

WorldMesh::~WorldMesh() {

}

#undef SIZE

