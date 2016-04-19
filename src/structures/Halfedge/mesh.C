#include <stdio.h>
#include <assert.h>
#include <GL/gl.h>

#include "mesh.h"
#include "edge.h"
#include "vertex.h"
#include "triangle.h"
#include "vertex_parent.h"
#include "glCanvas.h"

#define INITIAL_VERTEX 10000
#define INITIAL_EDGE 10000
#define INITIAL_TRIANGLE 10000

// =======================================================================
// CONSTRUCTORS & DESTRUCTORS
// =======================================================================

Mesh::Mesh() {
	vertices = new Array<Vertex*>(INITIAL_VERTEX);
	edges = new Bag<Edge*>(INITIAL_EDGE, Edge::extract_func);
	triangles = new Bag<Triangle*>(INITIAL_TRIANGLE, Triangle::extract_func);
	vertex_parents = new Bag<VertexParent*>(INITIAL_VERTEX,
			VertexParent::extract_func);
	bbox = NULL;
}

Mesh::~Mesh() {
	delete vertices;
	vertices = NULL;
	delete edges;
	edges = NULL;
	delete triangles;
	triangles = NULL;
	delete bbox;
	bbox = NULL;
}

// =======================================================================
// MODIFIERS:   ADD & REMOVE
// =======================================================================

Vertex* Mesh::addVertex(const Vec3f &position) {
	int index = numVertices();
	Vertex *v = new Vertex(index, position);
	vertices->Add(v);
	if (bbox == NULL)
		bbox = new BoundingBox(position, position);
	else
		bbox->Extend(position);
	return v;
}

void Mesh::addTriangle(Vertex *a, Vertex *b, Vertex *c) {

	// create the triangle
	Triangle *t = new Triangle();

	// create the edges
	Edge *ea = new Edge(a, t);
	Edge *eb = new Edge(b, t);
	Edge *ec = new Edge(c, t);

	// point the triangle to one of its edges
	t->setEdge(ea);

	// connect the edges to each other
	ea->setNext(ec);
	eb->setNext(ea);
	ec->setNext(eb);

	// add them to the master list
	edges->Add(ea);
	edges->Add(eb);
	edges->Add(ec);

	// connect up with opposite edges (if they exist)
	Edge *ea_op = getEdge((*ea)[1], (*ea)[0]);
	Edge *eb_op = getEdge((*eb)[1], (*eb)[0]);
	Edge *ec_op = getEdge((*ec)[1], (*ec)[0]);
	if (ea_op != NULL) {
		ea_op->setOpposite(ea);
	}
	if (eb_op != NULL) {
		eb_op->setOpposite(eb);
	}
	if (ec_op != NULL) {
		ec_op->setOpposite(ec);
	}

	// add the triangle to the master list
	triangles->Add(t);
}

void Mesh::removeTriangle(Triangle *t) {

	Edge *ea = t->getEdge();
	Edge *eb = ea->getNext();
	Edge *ec = eb->getNext();
	assert(ec->getNext() == ea);

	// remove elements from master lists
	edges->Remove(ea);
	edges->Remove(eb);
	edges->Remove(ec);
	triangles->Remove(t);

	// clean up memory
	delete ea;
	delete eb;
	delete ec;
	delete t;
}

Edge* Mesh::getEdge(Vertex *a, Vertex *b) const {
	assert(edges != NULL);
	return edges->Get(a->getIndex(), b->getIndex());
}

Vertex* Mesh::getChildVertex(Vertex *p1, Vertex *p2) const {
	VertexParent *vp = vertex_parents->GetReorder(p1->getIndex(),
			p2->getIndex());
	if (vp == NULL)
		return NULL;
	return vp->get();
}

void Mesh::setParentsChild(Vertex *p1, Vertex *p2, Vertex *child) {
	vertex_parents->Add(new VertexParent(p1, p2, child));
}

// =======================================================================
// the load function parses very simple .obj files
// the basic format has been extended to allow the specification 
// of crease weights on the edges.
// =======================================================================

void Mesh::Load(const char *input_file) {

	FILE *objfile = fopen(input_file, "r");
	if (objfile == NULL) {
		printf("ERROR! CANNOT OPEN '%s'\n", input_file);
		return;
	}

	char line[200];
	char token[100];
	char atoken[100];
	char btoken[100];
	char ctoken[100];
	char dtoken[100];
	char etoken[100];
	float x, y, z;
	int a, b, c, d, e;

	int index = 0;
	int vert_count = 0;
	int vert_index = 1;

	while (fgets(line, 200, objfile)) {

		if (line[strlen(line) - 2] == '\\') {
			fgets(token, 100, objfile);
			int tmp = strlen(line) - 2;
			strncpy(&line[tmp], token, 100);
		}
		int token_count = sscanf(line, "%s\n", token);
		if (token_count == -1)
			continue;
		a = b = c = d = e = -1;
		if (!strcmp(token, "usemtl") || !strcmp(token, "g")) {
			vert_index = 1; //vert_count + 1;
			index++;
		} else if (!strcmp(token, "v")) {
			vert_count++;
			sscanf(line, "%s %f %f %f\n", token, &x, &y, &z);
			addVertex(Vec3f(x, y, z));
		} else if (!strcmp(token, "f")) {
			int num = sscanf(line, "%s %s %s %s %s %s\n", token, atoken, btoken,
					ctoken, dtoken, etoken);
			sscanf(atoken, "%d", &a);
			sscanf(btoken, "%d", &b);
			sscanf(ctoken, "%d", &c);
			if (num > 4)
				sscanf(dtoken, "%d", &d);
			if (num > 5)
				sscanf(etoken, "%d", &e);
			a -= vert_index;
			b -= vert_index;
			c -= vert_index;
			if (d >= 0)
				d -= vert_index;
			if (e >= 0)
				e -= vert_index;
			assert(a >= 0 && a < numVertices());
			assert(b >= 0 && b < numVertices());
			assert(c >= 0 && c < numVertices());

			addTriangle(getVertex(a), getVertex(b), getVertex(c));
			if (d > -1) {
				assert(d < numVertices());
				addTriangle(getVertex(a), getVertex(c), getVertex(d));
			}
			if (e > -1) {
				assert(e < numVertices());
				addTriangle(getVertex(a), getVertex(d), getVertex(e));
			}
		} else if (!strcmp(token, "e")) {
			int num = sscanf(line, "%s %s %s %s\n", token, atoken, btoken,
					ctoken);
			assert(num == 4);
			sscanf(atoken, "%d", &a);
			sscanf(btoken, "%d", &b);
			if (!strcmp(ctoken, "inf"))
				x = 1000000; // this is close to infinity...
			else
				sscanf(ctoken, "%f", &x);
			Vertex *va = getVertex(a);
			Vertex *vb = getVertex(b);
			Edge *ab = getEdge(va, vb);
			Edge *ba = getEdge(vb, va);
			assert(ab != NULL);
			assert(ba != NULL);
			ab->setCrease(x);
			ba->setCrease(x);
		} else if (!strcmp(token, "vt")) {
		} else if (!strcmp(token, "vn")) {
		} else if (token[0] == '#') {
		} else {
			printf("LINE: '%s'", line);
		}
	}
}

// =======================================================================
// PAINT
// =======================================================================

Vec3f ComputeNormal(const Vec3f &p1, const Vec3f &p2, const Vec3f &p3) {
	Vec3f v12 = p2;
	v12 -= p1;
	Vec3f v23 = p3;
	v23 -= p2;
	Vec3f normal;
	Vec3f::Cross3(normal, v12, v23);
	normal.Normalize();
	return normal;
}

void InsertNormal(const Vec3f &p1, const Vec3f &p2, const Vec3f &p3) {
	Vec3f normal = ComputeNormal(p1, p2, p3);
	glNormal3f(normal.x(), normal.y(), normal.z());
}

void Mesh::Paint(ArgParser *args) {

	computeFaceNormals();
	computeVerticesNormals();

	// scale it so it fits in the window
	Vec3f center;
	bbox->getCenter(center);
	float s = 1 / bbox->maxDim();
	glScalef(s, s, s);
	glTranslatef(-center.x(), -center.y(), -center.z());

	// this offset prevents "z-fighting" bewteen the edges and faces
	// the edges will always win.
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1.1, 4.0);

	// draw the triangles
	glColor3f(1, 1, 1);
	Iterator<Triangle*> *iter = triangles->StartIteration();
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
	triangles->EndIteration(iter);
	glEnd();

	glDisable(GL_POLYGON_OFFSET_FILL);

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

	HandleGLError();
}

// =================================================================
// SUBDIVISION
// =================================================================

void Mesh::LoopSubdivision() {
	// printf ("Subdivide the mesh!\n");
//	Iterator<Edge*>* iterator = edges->StartIteration();
//
//	Edge* currentEdge = NULL;
//
//	while ((currentEdge = iterator->GetNext())) {
//		//printf("1\n");
//		Vertex* v0 = (*currentEdge)[0];
//		Vertex* v1 = (*currentEdge)[1];
//
//		Vertex* newVertex = addVertex((v0->get() + v1->get()) * 0.5);
//
//		setParentsChild(v0, v1, newVertex);
//
//		Vertex* vertex1 = currentEdge->getVertex();
//		Vertex* vertex2 = currentEdge->getNext()->getVertex();
//		Vertex* vertex3 = currentEdge->getNext()->getNext()->getVertex();
//
//		removeTriangle(currentEdge->getTriangle());
//
//		//printf("2\n");
//
////		addTriangle(newVertex, (*firstEdge)[0], (*firstEdge->getNext())[0]);
////
////		addTriangle(newVertex, (*firstEdge->getNext())[0],
////				(*firstEdge->getNext()->getNext())[0]);
//
//		addTriangle(newVertex, vertex3, vertex2);
//
//		addTriangle(newVertex, vertex2, vertex1);
//
////		addTriangle(newVertex, vertex1, vertex2);
////
////		addTriangle(newVertex, vertex2, vertex3);
//
////		addTriangle(newVertex, firstEdge->getVertex(), firstEdge->getNext()->getVertex());
////
////		addTriangle(newVertex, firstEdge->getNext()->getVertex(),
////				firstEdge->getNext()->getNext()->getVertex());
//
//		//printf("3\n");
//		printf("(%d, %d, %d)\n", vertex1->getIndex(), vertex2->getIndex(), vertex3->getIndex());
//	}
//
//	edges->EndIteration(iterator);
}

// =================================================================
// SIMPLIFICATION
// =================================================================

void Mesh::Simplification(int target_tri_count) {
	printf("Simplify the mesh! %d -> %d\n", numTriangles(), target_tri_count);
}

std::vector<Triangle*> Mesh::getTrianglesByVertex(Edge * e) {
	std::vector<Triangle*> faces;
	Edge * currentEdge = e;

	do {
		faces.push_back(currentEdge->getTriangle());

		if(currentEdge->getOpposite()) {
			currentEdge = currentEdge->getOpposite()->getNext()->getNext();
		} else {
			break;
		}
	} while(currentEdge != e);

	return faces;
}

void Mesh::computeFaceNormals() {
	Iterator<Triangle*> *iter = triangles->StartIteration();

	while (Triangle *t = iter->GetNext()) {
		Vec3f a = (*t)[0]->get();
		Vec3f b = (*t)[1]->get();
		Vec3f c = (*t)[2]->get();
		//InsertNormal(a, b, c);
		t->setNormal(ComputeNormal(a, b, c));
	}
	triangles->EndIteration(iter);
}

void Mesh::computeVerticesNormals() {
	Iterator<Edge*> *iter = edges->StartIteration();

	Edge *e = NULL;

	while( (e = iter->GetNext())) {
		std::vector<Triangle*> triangles = getTrianglesByVertex(e);

		Vec3f normal(0, 0, 0);

		for(Triangle* t : triangles) {
			normal += t->getNormal();
		}

		e->getVertex()->setNormal(normal);
	}

	edges->EndIteration(iter);
}
// =================================================================
