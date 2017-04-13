/*
 * CelestialBody.h
 *
 *  Created on: 14 de abr de 2016
 *      Author: cassiano
 */

#ifndef SRC_CELESTIALBODY_H_
#define SRC_CELESTIALBODY_H_

#include "structures/vectors.h"
#include "structures/Halfedge/mesh.h"

#include "Time.h"

/**
 * Represents a generic celestial body.
 */
class CelestialBody {
public:

	/**
	 * Construct a celestial body with default mass and size.
	 * @param center Body position
	 */
	CelestialBody(const Vec3f& center) :
			center(center), mesh(new Mesh()), mass(0.0f), orbiter(nullptr), semiMajorAxis(
					0) {
	}
	virtual ~CelestialBody() {
		delete mesh;
	}

	/**
	 * Set body position.
	 */
	void setCenter(const Vec3f& position) {
		center = position;
	}

	/**
	 * Get body position.
	 */
	const Vec3f getCenter() const {
		return center;
	}

	/**
	 * Get body mass.
	 */
	float getMass() const {
		return mass;
	}

	/**
	 * Set body mass.
	 */
	void setMass(float mass) {
		this->mass = mass;
	}

	/**
	 * Get half-edge mesh.
	 */
	Mesh * getMesh() {
		return mesh;
	}

	/**
	 * Get orbiter.
	 */
	const CelestialBody* getOrbiter() const {
		return orbiter;
	}

	/**
	 * Set orbiter.
	 */
	void setOrbiter(CelestialBody* orbiter) {
		this->orbiter = orbiter;
		if (orbiter) {
			float distance = (center - orbiter->getCenter()).length();

			semiMajorAxis = (distance / 2) * 1.2;
		} else {
			semiMajorAxis = 0.0f;
		}
	}

	/**
	 * Get half major axis value of orbit.
	 */
	float getSemiMajorAxis() const {
		return semiMajorAxis;
	}

	/**
	 * Update body.
	 * @param dt Time elapsed since last update.
	 */
	virtual void update(const Time& dt) = 0;

	/**
	 * Display body on screen.
	 */
	virtual void draw() = 0;

private:

protected:

	// TODO Check usage of this function
	Bag<Triangle*>* getTriangles() {
		return mesh->triangles;
	}

	/**
	 * Recalculate mesh normals vectors.
	 */
	void recalculateNormals() {
		mesh->updateNormals();
	}

	Vec3f center;						/**< Body position. */
	Mesh * mesh;						/**< Half-edge mesh. */

	CelestialBody * orbiter;

	float mass;							/**< Body mass. */
	float semiMajorAxis;				/**< Semi major axis of orbit. */
};

#endif /* SRC_CELESTIALBODY_H_ */
