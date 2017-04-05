/*
 * World.cpp
 *
 *  Created on: 1 de abr de 2017
 *      Author: cassiano
 */

#include "World.h"

#include <algorithm>
#include <cmath>
#include <GL/gl.h>

#include "Perlin.h"

int World::tris_rendered = 0;


World::World() {
	// TODO Auto-generated constructor stub

}

World::~World() {
	// TODO Auto-generated destructor stub
}

void World::draw_triangle(Vec3f p1, Vec3f p2, Vec3f p3) {
	glBegin (GL_TRIANGLES);
	glVertex3f(p1.x(), p1.y(), p1.z());
	glVertex3f(p2.x(), p2.y(), p2.z());
	glVertex3f(p3.x(), p3.y(), p3.z());
	glEnd();
	tris_rendered++;
}

void World::draw_recursive(Vec3f p1, Vec3f p2, Vec3f p3, Vec3f center,
			double size) {
		//		double ratio_size = size * gui.screen[0].slider["lod.ratio"].val; // default : 1
		//		double minsize = gui.screen[0].slider["detail"].val;    // default : 0.01
		double ratio_size = size * 20.0; // default : 1
		double minsize = 0.01;    // default : 0.01
		Vec3f edge_center[3] = { (p1 + p2) * 0.5f, (p2 + p3) * 0.5f, (p3 + p1) * 0.5f };
		bool edge_test[3];
		double angle[3];

		for (auto i = 0; i < 3; ++i) {


			Vec3f d = center + edge_center[i];

//			Vec3f d = edge_center[i] - center;

//			edge_test[i] = d.len() > ratio_size;

			edge_test[i] = d.Length() > ratio_size;
			d.Normalize();

			double dot = edge_center[i].Dot3(d);

//			double dot = edge_center[i].dot(d);
//			double dot = edge_center[i].dot(d.norm());
//			angle[i] = acos(clamp(dot, -1.0, 1.0));
			angle[i] = std::acos(std::max(-1.0, std::min(dot, 1.0)));
		}

		// culling
//		if (std::max(angle[0], std::max(angle[1], angle[2])) < M_PI / 2 - 0.2)
//			return;

		// draw
		if ((edge_test[0] && edge_test[1] && edge_test[2]) || size < minsize) {

//			{
//				p1 += p1 * (0.005f * Perlin::generate(p1));
//				p2 += p2 * (0.005f * Perlin::generate(p2));
//				p3 += p3 * (0.005f * Perlin::generate(p3));
//			}

			draw_triangle(p1, p2, p3);
			return;
		}

		// Recurse
		Vec3f p[6] = { p1, p2, p3, edge_center[0], edge_center[1],
				edge_center[2] };
		int idx[12] = { 0, 3, 5, 5, 3, 4, 3, 1, 4, 5, 4, 2 };
		bool valid[4] = { 1, 1, 1, 1 };

		if (edge_test[0]) {
			p[3] = p1;
			valid[0] = 0;
		} // skip triangle 0 ?
		if (edge_test[1]) {
			p[4] = p2;
			valid[2] = 0;
		} // skip triangle 2 ?
		if (edge_test[2]) {
			p[5] = p3;
			valid[3] = 0;
		} // skip triangle 3 ?

		for (auto i = 0; i < 4; ++i) {

//			printf("isValid: \n", i);

			if (valid[i]) {
				int i1 = idx[3 * i + 0], i2 = idx[3 * i + 1], i3 =
						idx[3 * i + 2];

				p[i1].Normalize();
				p[i2].Normalize();
				p[i3].Normalize();

				draw_recursive(p[i1], p[i2], p[i3], center,
						size / 2);

//				draw_recursive(p[i1].norm(), p[i2].norm(), p[i3].norm(), center,
//						size / 2);
			}
		}
	}

void World::draw(Vec3f center) {
	// create icosahedron
	double t = (1.0 + sqrt(5.0)) / 2.0;

	std::vector<Vec3f> p( { { -1, t, 0 }, { 1, t, 0 }, { -1, -t, 0 }, { 1,
			-t, 0 }, { 0, -1, t }, { 0, 1, t }, { 0, -1, -t }, { 0, 1, -t },
			{ t, 0, -1 }, { t, 0, 1 }, { -t, 0, -1 }, { -t, 0, 1 }, });
	std::vector<int> idx( { 0, 11, 5, 0, 5, 1, 0, 1, 7, 0, 7, 10, 0, 10, 11,
			1, 5, 9, 5, 11, 4, 11, 10, 2, 10, 7, 6, 10, 7, 6, 7, 1, 8, 3, 9,
			4, 3, 4, 2, 3, 2, 6, 3, 6, 8, 3, 8, 9, 4, 9, 5, 2, 4, 11, 6, 2,
			10, 8, 6, 7, 9, 8, 1 });

	tris_rendered = 0;

	for (auto i = 0; i < idx.size() / 3; ++i) {

		p[idx[i * 3 + 0]].Normalize();
		p[idx[i * 3 + 1]].Normalize();
		p[idx[i * 3 + 2]].Normalize();

		Vec3f p3[3] = { p[idx[i * 3 + 0]],	// triangle point 1
							p[idx[i * 3 + 1]],	// triangle point 2
							p[idx[i * 3 + 2]] };	// triangle point 3

//			Vec3f p3[3] = { p[idx[i * 3 + 0]].norm(),	// triangle point 1
//					p[idx[i * 3 + 1]].norm(),	// triangle point 2
//					p[idx[i * 3 + 2]].norm() };	// triangle point 3
		draw_recursive(p3[0], p3[1], p3[2], center);
	}


	//		gui.screen[0].label["stats"].text = Gui::String("Triangles: ")
	//				+ tris_rendered;
}
