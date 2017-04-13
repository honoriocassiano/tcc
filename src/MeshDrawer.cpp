/*
 * MeshDrawer.cpp
 *
 *  Created on: 4 de mai de 2016
 *      Author: cassiano
 */

#include "MeshDrawer.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>

#include "structures/Halfedge/triangle.h"
#include "structures/Halfedge/vertex.h"
#include "structures/matrix.h"

#include "Perlin.h"

int HandleGLError() {
	GLenum error;
	int i = 0;
	while ((error = glGetError()) != GL_NO_ERROR) {
		printf("GL ERROR(%d):  %s\n", i, gluErrorString(error));
		i++;
	}
	if (i == 0)
		return 1;
	return 0;
}

MeshDrawer::MeshDrawer() {

}

MeshDrawer::~MeshDrawer() {

}

#define DX(x, y, z) (x)
#define DY(x, y, z) (y)
#define DZ(x, y, z) (z)

//#define DX(x, y, z) (2 * x * sqrtf(1.0 - (2*y*y) - (2*z*z) + (8*y*y*z*z/1.5)))
//#define DY(x, y, z) (2 * y * sqrtf(1.0 - (2*z*z) - (2*x*x) + (8*z*z*x*x/1.5)))
//#define DZ(x, y, z) (2 * z * sqrtf(1.0 - (2*x*x) - (2*y*y) + (8*x*x*y*y/1.5)))
//#define DX(x, y, z) (x * sqrtf(1.0 - (y*y/2) - (z*z/2) + (y*y*z*z/3)))
//#define DY(x, y, z) (y * sqrtf(1.0 - (z*z/2) - (x*x/2) + (z*z*x*x/3)))
//#define DZ(x, y, z) (z * sqrtf(1.0 - (x*x/2) - (y*y/2) + (x*x*y*y/3)))

void MeshDrawer::draw(Mesh* mesh, const DrawOptions& options) {
//	mesh->computeFaceNormals();
//	mesh->computeVerticesNormals();

//	mesh->reset();
	mesh->updateNormals();

	// this offset prevents "z-fighting" bewteen the edges and faces
	// the edges will always win.
//	glEnable(GL_POLYGON_OFFSET_FILL);
//	glPolygonOffset(1.1, 4.0);

	glPushMatrix();

	auto transform = mesh->getTransform().glGet();

	glMultMatrixf(transform);

	if (options.wireframe) {
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);

		glDisable(GL_LIGHTING);

		if (options.halfEdge) {
			drawHalfEdge(mesh);
		}
	} else {
		//		glPolygonMode( GL_FRONT, GL_FILL);
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);
	}

	if (options.axis) {
		drawAxis();
	}

	if (!options.points) {
		glBegin(GL_TRIANGLES);
		for (const auto& t : *mesh->triangles) {
			Vertex* va = (*t)[0];
			Vertex* vb = (*t)[1];
			Vertex* vc = (*t)[2];

			Vec3f tNormal = MeshDrawer::getNormal(t);

			// Check triangle orientation
			if (!isClockwise(t)) {
				vb = (*t)[2];
				vc = (*t)[1];

				tNormal *= -1;
			}

			Vec3f a = va->get();
			Vec3f b = vb->get();
			Vec3f c = vc->get();

			Vec3f n_a = va->getNormal();
			Vec3f n_b = vb->getNormal();
			Vec3f n_c = vc->getNormal();

			if (MeshDrawer::cos(n_a, tNormal) < 0.0) {
				n_a *= -1;
				va->setNormal(n_a);
			}
			if (MeshDrawer::cos(n_b, tNormal) < 0.0) {
				n_b *= -1;
				vb->setNormal(n_b);
			}

			if (MeshDrawer::cos(n_c, tNormal) < 0.0) {
				n_c *= -1;
				vc->setNormal(n_c);
			}

			//		printf("(%f, %f, %f)\n", va->getColor().r(), va->getColor().g(), va->getColor().b());

			//***************************************
			glColor3f(va->getColor().r(), va->getColor().g(),
					va->getColor().b());
			glNormal3f(n_a.x(), n_a.y(), n_a.z());

			if (options.noise) {
				glVertex3f(va->getReal().x(), va->getReal().y(),
						va->getReal().z());
			} else {
				glVertex3f(va->get().x(), va->get().y(), va->get().z());
			}

			//		glColor3f(n_b.r(), n_b.g(), n_b.b());
			glColor3f(vb->getColor().r(), vb->getColor().g(),
					vb->getColor().b());
			glNormal3f(n_b.x(), n_b.y(), n_b.z());

			if (options.noise) {
				glVertex3f(vb->getReal().x(), vb->getReal().y(),
						vb->getReal().z());
			} else {
				glVertex3f(vb->get().x(), vb->get().y(), vb->get().z());
			}

			//		glColor3f(n_c.r(), n_c.g(), n_c.b());
			glColor3f(vc->getColor().r(), vc->getColor().g(),
					vc->getColor().b());
			glNormal3f(n_c.x(), n_c.y(), n_c.z());

			if (options.noise) {
				glVertex3f(vc->getReal().x(), vc->getReal().y(),
						vc->getReal().z());
			} else {
				glVertex3f(vc->get().x(), vc->get().y(), vc->get().z());
			}

			//		if (options.normals) {
			//			glPushMatrix();
			//			glColor3f(0, 0, 1);
			//			glBegin(GL_LINES);
			//			drawNormal(t);
			//			glEnd();
			//
			//			glPopMatrix();
			//		}

			//***************************************
		}
		glEnd();

//		glDisable(GL_POLYGON_OFFSET_FILL);

		if (options.normals) {
//			iter = mesh->triangles->StartIteration();

			glBegin(GL_LINES);
			//		glColor3f(1.0, 0, 0);

//			while (Triangle *t = iter->GetNext()) {
//				MeshDrawer::drawNormal(t);
//			}
			drawNormals(mesh);

			glEnd();

//			mesh->triangles->EndIteration(iter);
		}

		glEnable(GL_LIGHTING);

	} else {
		glDisable(GL_LIGHTING);
		glPointSize(1.25);
		glBegin(GL_POINTS);
//		for (int i = 0; i < mesh->vertices->Count(); ++i) {
//			auto v = (*mesh->vertices)[i];
//
////			glVertex3f(v->x(), v->y(), v->z());
//			if (v->isActive()) {
//				glColor3f(1, 0, 0);
//			} else {
//				glColor3f(1, 1, 1);
//			}
//
//			glVertex3f(DX(v->x(), v->y(), v->z()), DY(v->x(), v->y(), v->z()),
//					DZ(v->x(), v->y(), v->z()));
//		}
		for (const auto& v : *mesh->vertices2) {

//			glVertex3f(v->x(), v->y(), v->z());
			if (v->isActive()) {
				glColor3f(1, 0, 0);
			} else {
				glColor3f(1, 1, 1);
			}

			if (options.noise) {
				glVertex3f(v->getReal().x(), v->getReal().y(),
						v->getReal().z());
			} else {
				glVertex3f(v->x(), v->y(), v->z());
			}

		}

		glEnd();
		glEnable(GL_LIGHTING);
	}

	glPopMatrix();

	delete[] transform;

//	glDisable(GL_POLYGON_OFFSET_FILL);
	HandleGLError();
}

void MeshDrawer::drawNormals(Mesh* mesh) {

	glColor3f(0, 0, 1.0);

	for(const auto& triangle : *mesh->triangles) {

		auto p1 = (*triangle)[0]->get();
		auto p2 = (*triangle)[1]->get();
		auto p3 = (*triangle)[2]->get();

		Vec3f normal;

		Vec3f::cross3(normal, (p2 - p1), (p3 - p1));

		normal.normalize();
		normal = normal * 0.5;

		auto centroid = getCentroid(triangle);
		auto final = centroid + normal;

//	glColor3f(1.0, 0, 0);
		glVertex3f(centroid.x(), centroid.y(), centroid.z());
//	glColor3f(1.0, 0, 0);
		glVertex3f(final.x(), final.y(), final.z());
	}

	glColor3f(0, 1.0, 0);

	for (auto& v : *mesh->vertices2) {

		auto final = v->get() + v->getNormal();

		glVertex3f(v->get().x(), v->get().y(), v->get().z());

		glVertex3f(final.x(), final.y(), final.z());

	}
}

Vec3f MeshDrawer::getCentroid(const Triangle* triangle) {
	return ((*triangle)[0]->get() + (*triangle)[1]->get()
			+ (*triangle)[2]->get()) * (1.0 / 3);
}

Vec3f MeshDrawer::getNormal(const Triangle* triangle) {
	auto& A = (*triangle)[0]->get();
	auto& B = (*triangle)[1]->get();
	auto& C = (*triangle)[2]->get();

	Vec3f normal;

	Vec3f::cross3(normal, (B - A), (C - A));

	return normal;
}

float MeshDrawer::cos(const Vec3f& v1, const Vec3f& v2) {
	return (v1.dot3(v2)) / (v1.length() * v2.length());
}

void MeshDrawer::drawAxis() {

	glDisable(GL_LIGHTING);

	constexpr float center[3] { 0, 0, 0 };
	constexpr float x[3] { 1, 0, 0 };
	constexpr float y[3] { 0, 1, 0 };
	constexpr float z[3] { 0, 0, 1 };

	glPushMatrix();

	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();

	glBegin(GL_LINES);

	// Draw x axis
	glColor3fv(x);
	glVertex3fv(center);
	glVertex3fv(x);

	// Draw x axis
	glColor3fv(y);
	glVertex3fv(center);
	glVertex3fv(y);

	// Draw x axis
	glColor3fv(z);
	glVertex3fv(center);
	glVertex3fv(z);

	glEnd();

	glPopMatrix();

	glEnable(GL_LIGHTING);
}

void MeshDrawer::drawHalfEdge(Mesh* mesh) {

//	glDisable(GL_LIGHTING);
//	glColor3f(1, 1, 0);
	glBegin(GL_LINES);
//	glBegin(GL_TRIANGLES);

	int counter = 0;

	for(const auto& t : *mesh->triangles) {
//		if (counter++ > 0) {
//			break;
//		}

		auto centroid = getCentroid(t);

		float distance = 0.2f;
		float arrowSize = 0.2f;

		auto dv0 = (centroid - (*t)[0]->get()) * distance;
		auto dv1 = (centroid - (*t)[1]->get()) * distance;
		auto dv2 = (centroid - (*t)[2]->get()) * distance;

		auto v0 = (*t)[0]->get() + dv0;
		auto v1 = (*t)[1]->get() + dv1;
		auto v2 = (*t)[2]->get() + dv2;

		auto n01 = (*t)[0]->getNormal();
		auto n12 = (*t)[1]->getNormal();
		auto n20 = (*t)[2]->getNormal();

		auto n = t->getNormal();

		auto dirV0 = (v1 - v0) * arrowSize;
		auto dirV1 = (v2 - v1) * arrowSize;
		auto dirV2 = (v0 - v2) * arrowSize;

		auto m = Matrix::MakeAxisRotation(n, (160 * M_PI) / 180);

		m.Transform(dirV0);
		m.Transform(dirV1);
		m.Transform(dirV2);

		glColor3f(1, 1, 0);
//		glVertex3f(v0[0], v0[1], v0[2]);
		glVertex3f(DX(v0[0], v0[1], v0[2]), DY(v0[0], v0[1], v0[2]),
				DZ(v0[0], v0[1], v0[2]));

		glColor3f(1, 0, 0);
		glVertex3f(DX(v1[0], v1[1], v1[2]), DY(v1[0], v1[1], v1[2]),
				DZ(v1[0], v1[1], v1[2]));
//		glVertex3f(v1[0], v1[1], v1[2]);

		// ********************************************
		// ********************************************

		glVertex3f(DX(v1[0], v1[1], v1[2]), DY(v1[0], v1[1], v1[2]),
				DZ(v1[0], v1[1], v1[2]));

		//		glColor3f(1, 0, 0);
		glVertex3f(DX(v1[0] + dirV0[0], v1[1] + dirV0[1], v1[2] + dirV0[2]),
				DY(v1[0] + dirV0[0], v1[1] + dirV0[1], v1[2] + dirV0[2]),
				DZ(v1[0] + dirV0[0], v1[1] + dirV0[1], v1[2] + dirV0[2]));
//		glVertex3f(v1[0], v1[1], v1[2]);
//
////		glColor3f(1, 0, 0);
//		glVertex3f(v1[0] + dirV0[0], v1[1] + dirV0[1], v1[2] + dirV0[2]);

		// ********************************************
		// ********************************************

		glColor3f(1, 1, 0);

//		glVertex3f(v1[0], v1[1], v1[2]);
		glVertex3f(DX(v1[0], v1[1], v1[2]), DY(v1[0], v1[1], v1[2]),
				DZ(v1[0], v1[1], v1[2]));

		glColor3f(1, 0, 0);

		glVertex3f(DX(v2[0], v2[1], v2[2]), DY(v2[0], v2[1], v2[2]),
				DZ(v2[0], v2[1], v2[2]));
//		glVertex3f(v2[0], v2[1], v2[2]);

		// ********************************************
		// ********************************************

//		glVertex3f(v2[0], v2[1], v2[2]);
		glVertex3f(DX(v2[0], v2[1], v2[2]), DY(v2[0], v2[1], v2[2]),
				DZ(v2[0], v2[1], v2[2]));

//		glColor3f(1, 0, 0);
//		glVertex3f(v2[0] + dirV1[0], v2[1] + dirV1[1], v2[2] + dirV1[2]);
		glVertex3f(DX(v2[0] + dirV1[0], v2[1] + dirV1[1], v2[2] + dirV1[2]),
				DY(v2[0] + dirV1[0], v2[1] + dirV1[1], v2[2] + dirV1[2]),
				DZ(v2[0] + dirV1[0], v2[1] + dirV1[1], v2[2] + dirV1[2]));

		// ********************************************
		// ********************************************
		glColor3f(1, 1, 0);

//		glVertex3f(v2[0], v2[1], v2[2]);
		glVertex3f(DX(v2[0], v2[1], v2[2]), DY(v2[0], v2[1], v2[2]),
				DZ(v2[0], v2[1], v2[2]));

		glColor3f(1, 0, 0);

//		glVertex3f(v0[0], v0[1], v0[2]);
		glVertex3f(DX(v0[0], v0[1], v0[2]), DY(v0[0], v0[1], v0[2]),
				DZ(v0[0], v0[1], v0[2]));

		// ********************************************
		// ********************************************

		glVertex3f(DX(v0[0], v0[1], v0[2]), DY(v0[0], v0[1], v0[2]),
				DZ(v0[0], v0[1], v0[2]));
		glVertex3f(DX(v0[0] + dirV2[0], v0[1] + dirV2[1], v0[2] + dirV2[2]),
				DY(v0[0] + dirV2[0], v0[1] + dirV2[1], v0[2] + dirV2[2]),
				DZ(v0[0] + dirV2[0], v0[1] + dirV2[1], v0[2] + dirV2[2]));
//		glVertex3f(v0[0], v0[1], v0[2]);
//		glVertex3f(v0[0] + dirV2[0], v0[1] + dirV2[1], v0[2] + dirV2[2]);

		// ********************************************
		// ********************************************
	}

	glEnd();
//	glEnable(GL_LIGHTING);
}

bool MeshDrawer::isClockwise(const Triangle* triangle) {
	auto& A = (*triangle)[0]->get();
	auto& B = (*triangle)[1]->get();
	auto& C = (*triangle)[2]->get();

	Vec3f normal;

	Vec3f::cross3(normal, (B - A), (C - A));

	Vec3f centroid = getCentroid(triangle);

	return (normal.dot3(centroid) > 0.0f);
}
