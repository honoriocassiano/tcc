#include "structures/vectors.h"
#include "Planet.h"
#include <stdio.h>
#include <vector>

#include "structures/Halfedge/vertex.h"
#include "structures/matrix.h"

#include "structures/Halfedge/triangle.h"

#include "MeshDrawer.h"

#include "Constants.h"
#include "Scale.h"

constexpr double X = .525731112119133606;
constexpr double Z = .850650808352039932;

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

float calculateVelocity(float mass, float semiMajorAxis, const Vec3f& orbiter,
		const Vec3f& planet) {

	Vec3f r_v = planet - orbiter;

	float r = r_v.length();

	return sqrtf(mass * constant::G() * (2 / (r * Scale::getLength()) - 1 / (semiMajorAxis * Scale::getLength()) ));
}

Planet::Planet(GLfloat radius) :
		CelestialBody(Vec3f()), mPendingUpdate(false) {
	this->mRadius = radius;

	mTurbulenceDistortion = 0.2f;

	makePoints();
	makeTriangles();

	for (int i = 0; i < 1; ++i) {
		subdivide();
	}
}

Planet::~Planet() {

}

void Planet::makePoints() {
	for (int i = 0; i < 12; ++i) {

		float x = vdata[i][0];
		float y = vdata[i][1];
		float z = vdata[i][2];

		Vec3f position(x, y, z);

		float turbulence = Perlin::generateTurbulence(octaves, A, B,
				(x + 1) * mRadius, (y + 1) * mRadius, (z + 1) * mRadius);

		position += (position * turbulence * mTurbulenceDistortion);

		//green 008000
//		acqua 7FFFD4

		float positiveTurbulence = (turbulence * 0.5) + 0.5;

//		Color green(0x00 / 255.0f, 0x80 / 255.0f, 0x00 / 255.0f);
//		Color acqua(0xCD / 255.0f, 0x85 / 255.0f, 0x3F / 255.0f);

#warning "Fix"
		Vertex * v = mesh->addVertex(position, 0);
//		v->setColor(Color::interpolate(acqua, green, positiveTurbulence));
	}
}

void Planet::subdivide() {
	mPendingUpdate = true;

	int count = mesh->numTriangles();

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

		p01.normalize();
		p12.normalize();
		p20.normalize();

		float turbulence01 = Perlin::generateTurbulence(octaves, A, B,
				(p01.x() + 1) * mRadius, (p01.y() + 1) * mRadius,
				(p01.z() + 1) * mRadius);
		float turbulence12 = Perlin::generateTurbulence(octaves, A, B,
				(p12.x() + 1) * mRadius, (p12.y() + 1) * mRadius,
				(p12.z() + 1) * mRadius);
		float turbulence20 = Perlin::generateTurbulence(octaves, A, B,
				(p20.x() + 1) * mRadius, (p20.y() + 1) * mRadius,
				(p20.z() + 1) * mRadius);

		p01 += p01 * turbulence01 * mTurbulenceDistortion;
		p12 += p12 * turbulence12 * mTurbulenceDistortion;
		p20 += p20 * turbulence20 * mTurbulenceDistortion;

//		Color green(0x00 / 255.0f, 0x80 / 255.0f, 0x00 / 255.0f);
//		Color acqua(0xCD / 255.0f, 0x85 / 255.0f, 0x3F / 255.0f);

#warning "Fix"
		v01 = mesh->addVertex(p01, 0);
		v12 = mesh->addVertex(p12, 0);
		v20 = mesh->addVertex(p20, 0);

//		v01->setColor(Color::interpolate(acqua, green, (turbulence01 * 0.5) + 0.5));
//		v12->setColor(Color::interpolate(acqua, green, (turbulence12 * 0.5) + 0.5));
//		v20->setColor(Color::interpolate(acqua, green, (turbulence20 * 0.5) + 0.5));

		mesh->removeTriangle(t);

		mesh->addTriangle(v0, v01, v20);
		mesh->addTriangle(v1, v12, v01);
		mesh->addTriangle(v2, v20, v12);
		mesh->addTriangle(v01, v12, v20);
	}
}

void Planet::update(const Time& dt) {
	float elapsed = dt.getAsSeconds();

	const CelestialBody* orbiter = getOrbiter();

	if (orbiter) {
		calculateOrbitGravity();

		float velocity = calculateVelocity(getMass(), getSemiMajorAxis(),
				orbiter->getCenter(), getCenter());

		Vec3f normal = (center - orbiter->getCenter());
		normal.normalize();

		Matrix matrix = Matrix::MakeYRotation(90 * to_rad);

		matrix.Transform(normal);

		Vec3f normalVelocity(normal * velocity);

		Vec3f direction = (normalVelocity + mOrbitGravity) * dt.getAsSeconds()
				* Scale::getTime();

		setCenter(getCenter() + direction);

	}
}

void Planet::draw() {
	if (mPendingUpdate) {
		recalculateNormals();
	}

	glPushMatrix();

	glTranslatef(center.x(), center.y(), center.z());
	glScalef(mRadius, mRadius, mRadius);

	//mMesh->Paint(nullptr);
	MeshDrawer::draw(mesh);

	glPopMatrix();
}

void Planet::makeTriangles() {
	for (int i = 0; i < 20; ++i) {
//		Vertex * v0 = mesh->getVertex(tindices[i][0]);
//		Vertex * v1 = mesh->getVertex(tindices[i][1]);
//		Vertex * v2 = mesh->getVertex(tindices[i][2]);

//		mesh->addTriangle(v0, v2, v1);
	}
}

void Planet::calculateOrbitGravity() {

	if (getOrbiter()) {
//		Vec3f vec = getCenter() - getOrbiter()->getCenter();
		Vec3f vec = getOrbiter()->getCenter() - getCenter();

		float r3 = pow(vec.length() * Scale::getLength(), 2.0);

		vec.normalize();

		mOrbitGravity = vec * constant::G()
				* ((getMass() * getOrbiter()->getMass()) / r3);

		/*
		printf("gravity size: %e, vector: (%e, %e, %e)\n",
				mOrbitGravity.Length(), mOrbitGravity.x(), mOrbitGravity.y(),
				mOrbitGravity.z());
		*/

//		printf("%e, %f, %lf\n", constant::G, Scale::get(),
//				(constant::G * Scale::get()));
//		printf("gravity size: %f, vector: (%f, %f, %f)\n", mOrbitGravity.Length(), mOrbitGravity.x(), mOrbitGravity.y(),
//				mOrbitGravity.z());
	}
}
