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

#include "triangle.h"
#include "vertex.h"
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

//void MeshDrawer::draw(Mesh* mesh, bool wireframe, bool generateNoise) {
//	mesh->computeFaceNormals();
//	mesh->computeVerticesNormals();
//
//	// scale it so it fits in the window
////		Vec3f center;
////		bbox->getCenter(center);
////		float s = 1 / bbox->maxDim();
//	//glScalef(s, s, s);
//	//	glTranslatef(-center.x(), -center.y(), -7);///-center.z()*100);
//
//	//	printf("(%f, %f, %f)\n", center.x(), center.y(), center.z());
//
//	// this offset prevents "z-fighting" bewteen the edges and faces
//	// the edges will always win.
//	glEnable(GL_POLYGON_OFFSET_FILL);
//	glPolygonOffset(1.1, 4.0);
//
//	if (wireframe) {
//		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);
//	} else {
////		glPolygonMode( GL_FRONT, GL_FILL);
//		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);
//	}
//
//	// draw the triangles
////	glColor3f(1, 1, 1);
//	Iterator<Triangle*> *iter = mesh->triangles->StartIteration();
//	glBegin(GL_TRIANGLES);
//	while (Triangle *t = iter->GetNext()) {
//		Vertex* va = (*t)[0];
//		Vertex* vb = (*t)[1];
//		Vertex* vc = (*t)[2];
//
//		Vec3f a = va->get();
//		Vec3f b = vb->get();
//		Vec3f c = vc->get();
//
//		Vec3f n_a = va->getNormal();
//		Vec3f n_b = vb->getNormal();
//		Vec3f n_c = vc->getNormal();
//
////		printf("(%f, %f, %f)\n", va->getColor().r(), va->getColor().g(), va->getColor().b());
//
//		//***************************************
//		constexpr float maxHeight = 5.0f;
//
//		glColor3f(va->getColor().r(), va->getColor().g(), va->getColor().b());
//		glNormal3f(n_a.x(), n_a.y(), n_a.z());
//
//		if (generateNoise) {
//			glVertex3f(a.x(), a.y(), Perlin::generate(a));
//		} else {
//			glVertex3f(a.x(), a.y(), a.z());
//		}
//
////		glColor3f(n_b.r(), n_b.g(), n_b.b());
//		glColor3f(vb->getColor().r(), vb->getColor().g(), vb->getColor().b());
//		glNormal3f(n_b.x(), n_b.y(), n_b.z());
//
//		if (generateNoise) {
//			glVertex3f(b.x(), b.y(), Perlin::generate(b));
//		} else {
//			glVertex3f(b.x(), b.y(), b.z());
//		}
//
////		glColor3f(n_c.r(), n_c.g(), n_c.b());
//		glColor3f(vc->getColor().r(), vc->getColor().g(), vc->getColor().b());
//		glNormal3f(n_c.x(), n_c.y(), n_c.z());
//
//		if (generateNoise) {
//			glVertex3f(c.x(), c.y(), Perlin::generate(c));
//		} else {
//			glVertex3f(c.x(), c.y(), c.z());
//		}
//
//		//***************************************
//	}
//	mesh->triangles->EndIteration(iter);
//	glEnd();
//
//	glDisable(GL_POLYGON_OFFSET_FILL);
//
//	/*
//	 if (args->wireframe) {
//	 glDisable(GL_LIGHTING);
//
//	 // draw all the interior, non-crease edges
//	 glLineWidth(1);
//	 glColor3f(0, 0, 0);
//	 glBegin(GL_LINES);
//	 Iterator<Edge*> *iter = edges->StartIteration();
//	 while (Edge *e = iter->GetNext()) {
//	 if (e->getOpposite() == NULL || e->getCrease() > 0)
//	 continue;
//	 Vec3f a = (*e)[0]->get();
//	 Vec3f b = (*e)[1]->get();
//	 glVertex3f(a.x(), a.y(), a.z());
//	 glVertex3f(b.x(), b.y(), b.z());
//	 }
//	 edges->EndIteration(iter);
//	 glEnd();
//
//	 // draw all the interior, crease edges
//	 glLineWidth(3);
//	 glColor3f(1, 1, 0);
//	 glBegin(GL_LINES);
//	 iter = edges->StartIteration();
//	 while (Edge *e = iter->GetNext()) {
//	 if (e->getOpposite() == NULL || e->getCrease() == 0)
//	 continue;
//	 Vec3f a = (*e)[0]->get();
//	 Vec3f b = (*e)[1]->get();
//	 glVertex3f(a.x(), a.y(), a.z());
//	 glVertex3f(b.x(), b.y(), b.z());
//	 }
//	 edges->EndIteration(iter);
//	 glEnd();
//
//	 // draw all the boundary edges
//	 glLineWidth(3);
//	 glColor3f(1, 0, 0);
//	 glBegin(GL_LINES);
//	 iter = edges->StartIteration();
//	 while (Edge *e = iter->GetNext()) {
//	 if (e->getOpposite() != NULL)
//	 continue;
//	 assert(e->getCrease() == 0);
//	 Vec3f a = (*e)[0]->get();
//	 Vec3f b = (*e)[1]->get();
//	 glVertex3f(a.x(), a.y(), a.z());
//	 glVertex3f(b.x(), b.y(), b.z());
//	 }
//	 edges->EndIteration(iter);
//	 glEnd();
//
//	 glEnable(GL_LIGHTING);
//	 }
//	 */
//
//	HandleGLError();
//}

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

	mesh->reset();
	mesh->updateNormals();

	// this offset prevents "z-fighting" bewteen the edges and faces
	// the edges will always win.
//	glEnable(GL_POLYGON_OFFSET_FILL);
//	glPolygonOffset(1.1, 4.0);

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

	/*
	 // draw the triangles
	 //	glColor3f(1, 1, 1);
	 if (!options.points) {
	 auto iterEdge = mesh->edges->StartIteration();

	 glBegin(GL_LINES);
	 while (Edge *e = iterEdge->GetNext()) {

	 auto v1 = e->getVertex();
	 auto v2 = e->getNext()->getNext()->getVertex();

	 if (options.noise) {
	 glColor3f(v1->getColor().r(), v1->getColor().g(),
	 v1->getColor().b());
	 glNormal3f(v1->getNormal().x(), v1->getNormal().y(),
	 v1->getNormal().z());
	 glVertex3f(v1->get().x(), v1->get().y(),
	 Perlin::generate(v1->get()));

	 glColor3f(v2->getColor().r(), v2->getColor().g(),
	 v2->getColor().b());
	 glNormal3f(v2->getNormal().x(), v2->getNormal().y(),
	 v2->getNormal().z());
	 glVertex3f(v2->get().x(), v2->get().y(),
	 Perlin::generate(v2->get()));

	 } else {

	 glColor3f(v1->getColor().r(), v1->getColor().g(),
	 v1->getColor().b());
	 glNormal3f(v1->getNormal().x(), v1->getNormal().y(),
	 v1->getNormal().z());
	 glVertex3f(v1->get().x(), v1->get().y(), v1->get().z());

	 glColor3f(v2->getColor().r(), v2->getColor().g(),
	 v2->getColor().b());
	 glNormal3f(v2->getNormal().x(), v2->getNormal().y(),
	 v2->getNormal().z());
	 glVertex3f(v2->get().x(), v2->get().y(), v2->get().z());

	 }
	 }

	 mesh->edges->EndIteration(iterEdge);
	 glEnd();

	 glDisable(GL_POLYGON_OFFSET_FILL);

	 } else {
	 glBegin(GL_POINTS);

	 for (int i = 0; i < mesh->vertices->Count(); ++i) {
	 auto v = (*mesh->vertices)[i];

	 glColor3f(1, 1, 1);
	 glVertex3f(v->x(), v->y(), v->z());
	 }

	 glEnd();
	 }

	 if (options.normals) {
	 Iterator<Triangle*> *iterTriangle = mesh->triangles->StartIteration();

	 iterTriangle = mesh->triangles->StartIteration();

	 glColor3f(0, 0, 1.0);
	 glBegin(GL_LINES);
	 //		glColor3f(1.0, 0, 0);

	 while (Triangle *t = iterTriangle->GetNext()) {
	 MeshDrawer::drawNormal(t);
	 }

	 glEnd();

	 mesh->triangles->EndIteration(iterTriangle);
	 }
	 */

	if (!options.points) {
		Iterator<Triangle*> *iter = mesh->triangles->StartIteration();
		glBegin(GL_TRIANGLES);
		while (Triangle *t = iter->GetNext()) {
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
				glVertex3f(a.x(), a.y(), Perlin::generate(a));
			} else {
//				glVertex3f(a.x(), a.y(), a.z());
				glVertex3f(DX(a.x(), a.y(), a.z()), DY(a.x(), a.y(), a.z()),
						DZ(a.x(), a.y(), a.z()));
			}

			//		glColor3f(n_b.r(), n_b.g(), n_b.b());
			glColor3f(vb->getColor().r(), vb->getColor().g(),
					vb->getColor().b());
			glNormal3f(n_b.x(), n_b.y(), n_b.z());

			if (options.noise) {
				glVertex3f(b.x(), b.y(), Perlin::generate(b));
			} else {
//				glVertex3f(b.x(), b.y(), b.z());
				glVertex3f(DX(b.x(), b.y(), b.z()), DY(b.x(), b.y(), b.z()),
						DZ(b.x(), b.y(), b.z()));
			}

			//		glColor3f(n_c.r(), n_c.g(), n_c.b());
			glColor3f(vc->getColor().r(), vc->getColor().g(),
					vc->getColor().b());
			glNormal3f(n_c.x(), n_c.y(), n_c.z());

			if (options.noise) {
				glVertex3f(c.x(), c.y(), Perlin::generate(c));
			} else {
//				glVertex3f(c.x(), c.y(), c.z());
				glVertex3f(DX(c.x(), c.y(), c.z()), DY(c.x(), c.y(), c.z()),
						DZ(c.x(), c.y(), c.z()));
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
		mesh->triangles->EndIteration(iter);
		glEnd();

//		glDisable(GL_POLYGON_OFFSET_FILL);

		if (options.normals) {
//			iter = mesh->triangles->StartIteration();

			glColor3f(0, 0, 1.0);
			glBegin(GL_LINES);
			//		glColor3f(1.0, 0, 0);

//			while (Triangle *t = iter->GetNext()) {
//				MeshDrawer::drawNormal(t);
//			}

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

			glVertex3f(DX(v->x(), v->y(), v->z()), DY(v->x(), v->y(), v->z()),
					DZ(v->x(), v->y(), v->z()));
		}

		glEnd();
		glEnable(GL_LIGHTING);
	}

//	glDisable(GL_POLYGON_OFFSET_FILL);
	HandleGLError();
}

void MeshDrawer::drawNormal(Mesh* mesh) {

	auto iter = mesh->triangles->StartIteration();

	while (Triangle *triangle = iter->GetNext()) {

		auto p1 = (*triangle)[0]->get();
		auto p2 = (*triangle)[1]->get();
		auto p3 = (*triangle)[2]->get();

		Vec3f normal;

		Vec3f::Cross3(normal, (p2 - p1), (p3 - p1));

		normal.Normalize();
		normal = normal * 0.5;

		auto centroid = getCentroid(triangle);
		auto final = centroid + normal;

//	glColor3f(1.0, 0, 0);
		glVertex3f(centroid.x(), centroid.y(), centroid.z());
//	glColor3f(1.0, 0, 0);
		glVertex3f(final.x(), final.y(), final.z());
	}

	mesh->triangles->EndIteration(iter);
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

	Vec3f::Cross3(normal, (B - A), (C - A));

	return normal;
}

float MeshDrawer::cos(const Vec3f& v1, const Vec3f& v2) {
	return (v1.Dot3(v2)) / (v1.Length() * v2.Length());
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

	auto it = mesh->triangles->StartIteration();

//	glDisable(GL_LIGHTING);
//	glColor3f(1, 1, 0);
	glBegin(GL_LINES);
//	glBegin(GL_TRIANGLES);

	int counter = 0;

	while (Triangle *t = it->GetNext()) {
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

	mesh->triangles->EndIteration(it);

	glEnd();
//	glEnable(GL_LIGHTING);
}

bool MeshDrawer::isClockwise(const Triangle* triangle) {
	auto& A = (*triangle)[0]->get();
	auto& B = (*triangle)[1]->get();
	auto& C = (*triangle)[2]->get();

	Vec3f normal;

	Vec3f::Cross3(normal, (B - A), (C - A));

	Vec3f centroid = getCentroid(triangle);

	return (normal.Dot3(centroid) > 0.0f);
}
