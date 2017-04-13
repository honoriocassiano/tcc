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

GLfloat Planet::to_rad = M_PI / 180.0;

float calculateVelocity(float mass, float semiMajorAxis, const Vec3f& orbiter,
		const Vec3f& planet) {

	Vec3f r_v = planet - orbiter;

	float r = r_v.length();

	return sqrtf(
			mass * constant::G()
					* (2 / (r * Scale::getLength())
							- 1 / (semiMajorAxis * Scale::getLength())));
}

Planet::Planet(GLfloat radius) :
		CelestialBody(Vec3f()), pendingUpdate(false) {
	this->radius = radius;

	turbulenceDistortion = 0.2f;
}

Planet::~Planet() {

}

void Planet::update(const Time& dt) {
	float elapsed = dt.getAsSeconds();

	const CelestialBody* orbiter = getOrbiter();

	if (orbiter) {
		calculateOrbitGravity();

		float velocity = calculateVelocity(getMass(), getSemiMajorAxis(),
				orbiter->getCenter(), getCenter());

		Vec3f normal = (center - orbiter->getCenter()).normalized();

		Matrix matrix = Matrix::MakeYRotation(90 * to_rad);

		matrix.Transform(normal);

		Vec3f normalVelocity(normal * velocity);

		Vec3f direction = (normalVelocity + orbitGravity) * dt.getAsSeconds()
				* Scale::getTime();

		setCenter(getCenter() + direction);

	}

}

void Planet::draw() {
	if (pendingUpdate) {
		recalculateNormals();
	}

	glPushMatrix();

	glTranslatef(center.x(), center.y(), center.z());
	glScalef(radius, radius, radius);

	MeshDrawer::draw(mesh);

	glPopMatrix();
}

void Planet::calculateOrbitGravity() {

	if (getOrbiter()) {
//		Vec3f vec = getCenter() - getOrbiter()->getCenter();
		Vec3f vec = getOrbiter()->getCenter() - getCenter();

		float r3 = pow(vec.length() * Scale::getLength(), 2.0);

		orbitGravity = vec.normalized() * constant::G()
				* ((getMass() * getOrbiter()->getMass()) / r3);

	}
}
