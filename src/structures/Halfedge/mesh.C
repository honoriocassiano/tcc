#include <stdio.h>
#include <assert.h>
#include <GL/gl.h>

#include <string.h>

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

	vertices2 = new MultiLevelArray<Vertex*>();
}

Mesh::~Mesh() {
	delete edges;
	edges = NULL;
	delete triangles;
	triangles = NULL;

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
	edges->add(ea);
	edges->add(eb);
	edges->add(ec);

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
	triangles->add(t);

	return t;
}

void Mesh::removeTriangle(Triangle *t) {

	Edge *ea = t->getEdge();
	Edge *eb = ea->getNext();
	Edge *ec = eb->getNext();
	assert(ec->getNext() == ea);

	// remove elements from master lists
	edges->remove(ea);
	edges->remove(eb);
	edges->remove(ec);
	triangles->remove(t);

	// clean up memory
	delete ea;
	delete eb;
	delete ec;
	delete t;
}

Edge* Mesh::getEdge(Vertex *a, Vertex *b) const {
	assert(edges != NULL);
	return edges->get(P_TO_I(a), P_TO_I(b));
}

Vertex* Mesh::getChildVertex(Vertex *p1, Vertex *p2) const {
	VertexParent *vp = vertex_parents->getReorder(P_TO_I(p1), P_TO_I(p2));
	if (vp == NULL)
		return NULL;
	return vp->get();
}

void Mesh::setParentsChild(Vertex *p1, Vertex *p2, Vertex *child) {

	auto vp = new VertexParent(p1, p2, child);

	vertex_parents->add(vp);
	child->setParent(vp);
}

Vertex* Mesh::deleteParentsChildRelation(Vertex *p1, Vertex *p2) {
	VertexParent *vp = vertex_parents->getReorder(P_TO_I(p1), P_TO_I(p2));
	if (vp == NULL)
		return NULL;

	vertex_parents->remove(vp);

	return vp->get();
}

Vec3f computeNormal(const Vec3f &p1, const Vec3f &p2, const Vec3f &p3) {
	Vec3f v12 = p2;
	v12 -= p1;
	Vec3f v23 = p3;
	v23 -= p2;
	Vec3f normal;
	Vec3f::cross3(normal, v12, v23);
	normal.normalize();
	return normal;
}

void Mesh::printTrianglesPointers(int limit) {

	int i = 0;

	for (const auto& t : *triangles) {

		auto edge = t->getEdge();

		auto p0 = edge->getVertex();
		auto p1 = edge->getNext()->getVertex();
		auto p2 = edge->getNext()->getNext()->getVertex();

//		Log(
		printf("%d - v0: %p, v1: %p, v2: %p\n", i, p0, p1, p2);

		++i;
	}
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

//	auto it = triangles->startIteration();
	int i = 0;
//
//	while (Triangle *t = it->getNext()) {
	for (const auto& t : *triangles) {

		auto edge = t->getEdge();

		auto p0 = edge->getVertex()->get();
		auto p1 = edge->getNext()->getVertex()->get();
		auto p2 = edge->getNext()->getNext()->getVertex()->get();

		printf(
				"%d - v0: (%.5f, %.5f, %.5f), v1: (%.5f, %.5f, %.5f), v2: (%.5f, %.5f, %.5f)\n",
				i, p0.x(), p0.y(), p0.z(), p1.x(), p1.y(), p1.z(), p2.x(),
				p2.y(), p2.z());

		++i;
	}

//	triangles->endIteration(it);
}

void Mesh::updateNormals() {
//	auto it = triangles->startIteration();
//
//	while (auto t = it->getNext()) {
//	for(auto& t : *triangles) {
	for (auto it = triangles->begin(); it != triangles->end(); ++it) {

		auto& t = *it;

		auto a = (*t)[0];
		auto b = (*t)[1];
		auto c = (*t)[2];

//		auto normal = ComputeNormal(a->get(), b->get(), c->get());
		auto normal = computeNormal(a->getReal(), b->getReal(), c->getReal());

		auto normalA = a->getNormal() + normal;
		auto normalB = b->getNormal() + normal;
		auto normalC = c->getNormal() + normal;

		normalA.normalize();
		normalB.normalize();
		normalC.normalize();

		a->setNormal(normalA);
		b->setNormal(normalB);
		c->setNormal(normalC);

		t->setNormal(normal);
	}

//	triangles->endIteration(it);
}
// =================================================================
