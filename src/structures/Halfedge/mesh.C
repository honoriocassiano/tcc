#include <stdio.h>
#include <assert.h>
#include <GL/gl.h>

#include <string.h>

#include "Debug.h"

#include "mesh.h"
#include "edge.h"
#include "vertex.h"
#include "triangle.h"
#include "vertex_parent.h"

#define INITIAL_VERTEX 10000
#define INITIAL_EDGE 10000
#define INITIAL_TRIANGLE 10000

#define P_TO_I(p) ( (long long int)((void *)(p)) )

// =======================================================================
// CONSTRUCTORS & DESTRUCTORS
// =======================================================================

Mesh::Mesh() {
//	vertices = new Array<Vertex*>(INITIAL_VERTEX);
	edges = new Bag<Edge*>(INITIAL_EDGE, Edge::extract_func);
	triangles = new Bag<Triangle*>(INITIAL_TRIANGLE, Triangle::extract_func);
	vertex_parents = new Bag<VertexParent*>(INITIAL_VERTEX,
			VertexParent::extract_func);
	bbox = NULL;

	vertices2 = new MultiLevelArray<Vertex*>();
}

Mesh::~Mesh() {
//	delete vertices;
//	vertices = NULL;
	delete edges;
	edges = NULL;
	delete triangles;
	triangles = NULL;
	delete bbox;
	bbox = NULL;

	delete vertices2;
	vertices2 = nullptr;
}

// =======================================================================
// MODIFIERS:   ADD & REMOVE
// =======================================================================

Vertex* Mesh::addVertex(const Vec3f &position, std::size_t level) {
	int index = numVertices();
	Vertex *v = new Vertex(index, position);
//	vertices->Add(v);

	vertices2->add(v, level);

//	if (bbox == NULL)
//		bbox = new BoundingBox(position, position);
//	else
//		bbox->Extend(position);
	return v;
}

void Mesh::removeVertex(Vertex* vertex) {
//	vertices->Remove(vertex);

	auto it = vertices2->find(vertex, vertex->getLevel());

	if (it != vertices2->end(vertex->getLevel())) {
		vertices2->remove(it);
	}

	delete vertex;
}

Triangle* Mesh::addTriangle(Vertex *a, Vertex *b, Vertex *c) {

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

	return t;
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
//	return edges->Get(a->getIndex(), b->getIndex());
	return edges->Get(P_TO_I(a), P_TO_I(b));
}

Vertex* Mesh::getChildVertex(Vertex *p1, Vertex *p2) const {
//	VertexParent *vp = vertex_parents->GetReorder(p1->getIndex(),
//			p2->getIndex());
	VertexParent *vp = vertex_parents->GetReorder(P_TO_I(p1), P_TO_I(p2));
	if (vp == NULL)
		return NULL;
	return vp->get();
}

void Mesh::setParentsChild(Vertex *p1, Vertex *p2, Vertex *child) {

	auto vp = new VertexParent(p1, p2, child);

	vertex_parents->Add(vp);
	child->setParents1(vp);
}

void Mesh::setParentsChild(Vertex *p1_1, Vertex *p1_2, Vertex *p2_1,
		Vertex *p2_2, Vertex *child) {

	auto vp1 = new VertexParent(p1_1, p1_2, child);
	auto vp2 = new VertexParent(p2_1, p2_2, child);

	vertex_parents->Add(vp1);
	vertex_parents->Add(vp2);

	child->setParents1(vp1);
	child->setParents2(vp2);
}

Vertex* Mesh::deleteParentsChildRelation(Vertex *p1, Vertex *p2) {
//	VertexParent *vp = vertex_parents->GetReorder(p1->getIndex(),
//			p2->getIndex());
	VertexParent *vp = vertex_parents->GetReorder(P_TO_I(p1), P_TO_I(p2));
	if (vp == NULL)
		return NULL;

	vertex_parents->Remove(vp);

	return vp->get();
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
			addVertex(Vec3f(x, y, z), 0);
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

		if (currentEdge->getOpposite()) {
			currentEdge = currentEdge->getOpposite()->getNext()->getNext();
		} else {
			break;
		}
	} while (currentEdge != e);

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

void Mesh::printTrianglesPointers(int limit) {

	auto it = triangles->StartIteration();
	int i = 0;

	while (Triangle *t = it->GetNext()) {

		auto edge = t->getEdge();

		auto p0 = edge->getVertex();
		auto p1 = edge->getNext()->getVertex();
		auto p2 = edge->getNext()->getNext()->getVertex();

//		Log(
		printf("%d - v0: %p, v1: %p, v2: %p\n", i, p0, p1, p2);

		++i;
	}

	triangles->EndIteration(it);
}

void Mesh::computeVerticesNormals() {
	Iterator<Edge*> *iter = edges->StartIteration();

	Edge *e = nullptr;

	while ((e = iter->GetNext())) {
		std::vector<Triangle*> triangles = getTrianglesByVertex(e);

		Vec3f normal(0, 0, 0);

		for (Triangle* t : triangles) {
			normal += t->getNormal();
		}

		e->getVertex()->setNormal(normal);
	}

	edges->EndIteration(iter);
}

void Mesh::printVertices(int limit) {

//	while (i < vertices->Count() && i < limit) {
//		printf(
//		"[%d] (%f, %f, %f)\n", i, (*vertices)[i]->x(), (*vertices)[i]->y(),
//				(*vertices)[i]->z());
//
//		i++;
//	}
	std::size_t i = 0;
	for (const auto& v : *vertices2) {
		if (i >= limit) {
			break;
		}

		printf("[%lu] (%f, %f, %f)\n", i, v->x(), v->y(), v->z());

		i++;
	}
}

void Mesh::printTriangles(int limit) {

	auto it = triangles->StartIteration();
	int i = 0;

	while (Triangle *t = it->GetNext()) {

		auto edge = t->getEdge();

		auto p0 = edge->getVertex()->get();
		auto p1 = edge->getNext()->getVertex()->get();
		auto p2 = edge->getNext()->getNext()->getVertex()->get();

//		Log(
		printf(
				"%d - v0: (%.5f, %.5f, %.5f), v1: (%.5f, %.5f, %.5f), v2: (%.5f, %.5f, %.5f)\n",
				i, p0.x(), p0.y(), p0.z(), p1.x(), p1.y(), p1.z(), p2.x(),
				p2.y(), p2.z());

		++i;
	}

	triangles->EndIteration(it);
}

void Mesh::updateNormals() {
	auto it = triangles->StartIteration();

	while (auto t = it->GetNext()) {
		auto a = (*t)[0];
		auto b = (*t)[1];
		auto c = (*t)[2];

		auto normal = ComputeNormal(a->get(), b->get(), c->get());

		auto normalA = a->getNormal() + normal;
		auto normalB = b->getNormal() + normal;
		auto normalC = b->getNormal() + normal;

		normalA.Normalize();
		normalB.Normalize();
		normalC.Normalize();

		a->setNormal(normalA);
		b->setNormal(normalB);
		c->setNormal(normalC);

		t->setNormal(normal);
	}

	triangles->EndIteration(it);
}
// =================================================================
