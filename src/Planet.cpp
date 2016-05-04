#include "Planet.h"
#include <stdio.h>
#include <vector>

#include "vertex.h"
#include "vectors.h"

#include "triangle.h"

#include "MeshDrawer.h"

#define DEBUG_POINTS 0
#define DEBUG_INDEX 0
#define DEBUG_GLEW 0

#define X .525731112119133606
#define Z .850650808352039932

const GLfloat Planet::vdata[12][3] = { { -X, 0.0, Z }, { X, 0.0, Z }, { -X, 0.0,
		-Z }, { X, 0.0, -Z }, { 0.0, Z, X }, { 0.0, Z, -X }, { 0.0, -Z, X }, {
		0.0, -Z, -X }, { Z, X, 0.0 }, { -Z, X, 0.0 }, { Z, -X, 0.0 }, { -Z, -X,
		0.0 } };
const GLuint Planet::tindices[20][3] = { { 0, 4, 1 }, { 0, 9, 4 }, { 9, 5, 4 },
		{ 4, 5, 8 }, { 4, 8, 1 }, { 8, 10, 1 }, { 8, 3, 10 }, { 5, 3, 8 }, { 5,
				2, 3 }, { 2, 7, 3 }, { 7, 10, 3 }, { 7, 6, 10 }, { 7, 11, 6 }, {
				11, 0, 6 }, { 0, 1, 6 }, { 6, 1, 10 }, { 9, 0, 11 },
		{ 9, 11, 2 }, { 9, 2, 5 }, { 7, 2, 11 } };

int Planet::octaves = 8;
int Planet::A = 10;
int Planet::B = 2;

GLfloat Planet::to_rad = M_PI / 180.0;

Planet::Planet(GLfloat radius, int horizontal_sections, int vertical_sections) :
		CelestialBody(Vec3f()), mPendingUpdate(false) {
	this->mRadius = radius;

	this->h_sections = horizontal_sections;
	this->v_sections = vertical_sections;

	step_circle_angle = 360.0 / h_sections;
	step_height_angle = 180.0 / v_sections;

	this->vertex_size = h_sections * (v_sections + 1) * 3;
	this->index_size = 6 * h_sections * v_sections;

	this->vertex = new GLfloat[vertex_size];
	this->vertex_index = new GLuint[index_size];

	this->vertex_id = 0;
	this->vertex_index_id = 0;

	mTurbulenceDistortion = 0.1f;

	makePoints();
	makeTriangles();

	for (int i = 0; i < 4; ++i) {
		subdivide();
	}
}

Planet::~Planet() {
	delete[] vertex_index;
	delete[] vertex;
}

void Planet::show() {

	std::cout << "Drawing Sphere\n";

	makePoints();

#if DEBUG_GLEW
	std::cout << glewGetErrorString(glGetError()) << std::endl;
#endif

#if DEBUG_POINTS
	for (int i = 0; i < vertex_size; i += 3) {
		std::cout << "(x, y, z) => (" << vertex[i] << ", " << vertex[i + 1]
		<< ", " << vertex[i + 2] << ")\n";
	}

	std::cout << "Number of Vertex: " << vertex_size / 3 << std::endl;
#endif

#if DEBUG_INDEX

	for (int i = 0; i < index_size; i += 3) {
		std::cout << "Triangle => (" << vertex_index[i] << ", "
		<< vertex_index[i + 1] << ", " << vertex_index[i + 2]
		<< ")\n";
	}

	std::cout << "Number of Index: " << index_size << std::endl;

#endif

	//update(0);
}

//void Planet::update(float time) {
//	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);
//
////	glBindBuffer(GL_ARRAY_BUFFER, vertex_id);
////	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertex_index_id);
//	glEnableClientState(GL_VERTEX_ARRAY);
//	glVertexPointer(3, GL_FLOAT, 3 * sizeof(GLfloat), 0);
//
//	glDrawElements(GL_TRIANGLES, index_size,
//	GL_UNSIGNED_INT, (void *) 0);
//
//	glDisableClientState(GL_VERTEX_ARRAY);
////	glBindBuffer(GL_ARRAY_BUFFER, 0);
////	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//}

void Planet::makePoints() {
	for (int i = 0; i < 12; ++i) {

		float x = vdata[i][0];
		float y = vdata[i][1];
		float z = vdata[i][2];

		Vec3f position(x, y, z);

		float turbulence = Perlin::generateTurbulence(octaves, A, B,
				(x + 1) * mRadius, (y + 1) * mRadius, (z + 1) * mRadius);

		position += (position * turbulence * mTurbulenceDistortion);

		mMesh->addVertex(position);
	}
}

void Planet::subdivide() {
	mPendingUpdate = true;

	int count = mMesh->numTriangles();

	Bag<Triangle*>* triangles = getTriangles();

	auto iterator = triangles->StartIteration();

	std::vector<Triangle*> ts;

	while (Triangle* triangle = iterator->GetNext()) {
		ts.push_back(triangle);
	}
	triangles->EndIteration(iterator);

	Vertex * v01 = nullptr;
	Vertex * v12 = nullptr;
	Vertex * v20 = nullptr;

	for (int i = 0; i < count; ++i) {
		Triangle * t = ts[i];

		Vertex * v0 = (*t)[0];
		Vertex * v1 = (*t)[1];
		Vertex * v2 = (*t)[2];

		Vec3f p01 = v0->get() + v1->get();
		Vec3f p12 = v1->get() + v2->get();
		Vec3f p20 = v2->get() + v0->get();

		p01.Normalize();
		p12.Normalize();
		p20.Normalize();

		float turbulence01 = Perlin::generateTurbulence(octaves, A, B,
				(p01.x() + 1) * mRadius, (p01.y() + 1) * mRadius, (p01.z() + 1) * mRadius);
		float turbulence12 = Perlin::generateTurbulence(octaves, A, B,
				(p12.x() + 1) * mRadius, (p12.y() + 1) * mRadius, (p12.z() + 1) * mRadius);
		float turbulence20 = Perlin::generateTurbulence(octaves, A, B,
				(p20.x() + 1) * mRadius, (p20.y() + 1) * mRadius, (p20.z() + 1) * mRadius);

		p01 += p01 * turbulence01 * mTurbulenceDistortion;
		p12 += p12 * turbulence12 * mTurbulenceDistortion;
		p20 += p20 * turbulence20 * mTurbulenceDistortion;

		v01 = mMesh->addVertex(p01);
		v12 = mMesh->addVertex(p12);
		v20 = mMesh->addVertex(p20);

		mMesh->removeTriangle(t);

		mMesh->addTriangle(v0, v01, v20);
		mMesh->addTriangle(v1, v12, v01);
		mMesh->addTriangle(v2, v20, v12);
		mMesh->addTriangle(v01, v12, v20);
	}
}

void Planet::update(const Time& dt) {
	float elapsed = dt.getAsSeconds();

	float sp = 0.1f;

	const Vec3f speed(sp, sp, sp);

	setCenter( getCenter() + (speed * elapsed) );
}

void Planet::draw() {
	if(mPendingUpdate) {
		recalculateNormals();
	}

	glPushMatrix();

	glTranslatef(mCenter.x(), mCenter.y(), mCenter.z());
	glScalef(mRadius, mRadius, mRadius);

	//mMesh->Paint(nullptr);
	MeshDrawer::draw(mMesh);

	glPopMatrix();
}

void Planet::makeTriangles() {
	for (int i = 0; i < 20; ++i) {
		Vertex * v0 = mMesh->getVertex(tindices[i][0]);
		Vertex * v1 = mMesh->getVertex(tindices[i][1]);
		Vertex * v2 = mMesh->getVertex(tindices[i][2]);

		mMesh->addTriangle(v0, v2, v1);
	}
}
