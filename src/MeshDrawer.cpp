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

void MeshDrawer::draw(Mesh* mesh) {
	mesh->computeFaceNormals();
	mesh->computeVerticesNormals();

	// scale it so it fits in the window
//		Vec3f center;
//		bbox->getCenter(center);
//		float s = 1 / bbox->maxDim();
	//glScalef(s, s, s);
	//	glTranslatef(-center.x(), -center.y(), -7);///-center.z()*100);

	//	printf("(%f, %f, %f)\n", center.x(), center.y(), center.z());

	// this offset prevents "z-fighting" bewteen the edges and faces
	// the edges will always win.
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1.1, 4.0);

	// draw the triangles
	glColor3f(1, 1, 1);
	Iterator<Triangle*> *iter = mesh->triangles->StartIteration();
	glBegin(GL_TRIANGLES);
	while (Triangle *t = iter->GetNext()) {
		Vec3f a = (*t)[0]->get();
		Vec3f b = (*t)[1]->get();
		Vec3f c = (*t)[2]->get();

		Vec3f n_a = (*t)[0]->getNormal();
		Vec3f n_b = (*t)[1]->getNormal();
		Vec3f n_c = (*t)[2]->getNormal();
		//InsertNormal(a, b, c);

		//Vec3f normal = t->getNormal();
		//glNormal3f(normal.x(), normal.y(), normal.z());

		glNormal3f(n_a.x(), n_a.y(), n_a.z());
		glVertex3f(a.x(), a.y(), a.z());

		glNormal3f(n_b.x(), n_b.y(), n_b.z());
		glVertex3f(b.x(), b.y(), b.z());

		glNormal3f(n_c.x(), n_c.y(), n_c.z());
		glVertex3f(c.x(), c.y(), c.z());
	}
	mesh->triangles->EndIteration(iter);
	glEnd();

	glDisable(GL_POLYGON_OFFSET_FILL);

	/*
	 if (args->wireframe) {
	 glDisable(GL_LIGHTING);

	 // draw all the interior, non-crease edges
	 glLineWidth(1);
	 glColor3f(0, 0, 0);
	 glBegin(GL_LINES);
	 Iterator<Edge*> *iter = edges->StartIteration();
	 while (Edge *e = iter->GetNext()) {
	 if (e->getOpposite() == NULL || e->getCrease() > 0)
	 continue;
	 Vec3f a = (*e)[0]->get();
	 Vec3f b = (*e)[1]->get();
	 glVertex3f(a.x(), a.y(), a.z());
	 glVertex3f(b.x(), b.y(), b.z());
	 }
	 edges->EndIteration(iter);
	 glEnd();

	 // draw all the interior, crease edges
	 glLineWidth(3);
	 glColor3f(1, 1, 0);
	 glBegin(GL_LINES);
	 iter = edges->StartIteration();
	 while (Edge *e = iter->GetNext()) {
	 if (e->getOpposite() == NULL || e->getCrease() == 0)
	 continue;
	 Vec3f a = (*e)[0]->get();
	 Vec3f b = (*e)[1]->get();
	 glVertex3f(a.x(), a.y(), a.z());
	 glVertex3f(b.x(), b.y(), b.z());
	 }
	 edges->EndIteration(iter);
	 glEnd();

	 // draw all the boundary edges
	 glLineWidth(3);
	 glColor3f(1, 0, 0);
	 glBegin(GL_LINES);
	 iter = edges->StartIteration();
	 while (Edge *e = iter->GetNext()) {
	 if (e->getOpposite() != NULL)
	 continue;
	 assert(e->getCrease() == 0);
	 Vec3f a = (*e)[0]->get();
	 Vec3f b = (*e)[1]->get();
	 glVertex3f(a.x(), a.y(), a.z());
	 glVertex3f(b.x(), b.y(), b.z());
	 }
	 edges->EndIteration(iter);
	 glEnd();

	 glEnable(GL_LIGHTING);
	 }
	 */

	HandleGLError();
}
