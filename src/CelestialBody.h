/*
 * CelestialBody.h
 *
 *  Created on: 14 de abr de 2016
 *      Author: cassiano
 */

#ifndef SRC_CELESTIALBODY_H_
#define SRC_CELESTIALBODY_H_

#include "structures/Halfedge/vertex.h"
#include "structures/vectors.h"
#include "structures/Halfedge/mesh.h"
#include "structures/camera.h"

#include "Time.h"

/**
 * Represents a generic celestial body.
 */
class CelestialBody : public Mesh {
public:

	/**
	 * Construct a celestial body with default mass and size.
	 * @param center Body position
	 */
	CelestialBody(const Vec3f& center, float radius);
	virtual ~CelestialBody() {
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
	inline const Vec3f getCenter() const {
		return center;
	}

	/**
	 * Get body mass.
	 */
	inline float getMass() const {
		return mass;
	}

	/**
	 * Set body mass.
	 */
	void setMass(float mass) {
		this->mass = mass;
	}

	float getRadius() const {
		return radius;
	}

//	/**
//	 * Get half-edge mesh.
//	 */
//	Mesh * getMesh() {
//		return mesh;
//	}

	/**
	 * Get orbiter.
	 */
	inline const CelestialBody* getOrbiter() const {
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

	void reset() override;

	void recursiveUpdate(Vertex* v1, Vertex* v2, Vertex* v3,
			const Camera* camera, double size = 1);

	void updateLOD(const Camera* camera);

	void rotate(float dTheta);

	void deleteUnusedVertices();

	Vertex* getOrCreateChildVertex(Vertex *p1, Vertex *p2);

	Matrix getTransform() override {
		return transform;
	}

protected:

	virtual Vec3f getVertexPositionWithTransform(Vertex* v);

//	// TODO Check usage of this function
//	Bag<Triangle*>* getTriangles() {
//		return mesh->triangles;
//	}
//
//	/**
//	 * Recalculate mesh normals vectors.
//	 */
//	void recalculateNormals() {
//		mesh->updateNormals();
//	}

	Vec3f center; /**< Body position. */
//	Mesh * mesh; /**< Half-edge mesh. */

	CelestialBody * orbiter;

	float mass; /**< Body mass. */
	float semiMajorAxis; /**< Semi major axis of orbit. */

	float radius;

	Vec3f axis;

	Vertex* baseVertices[12];

	std::size_t baseIndices[20][3];

	Matrix transform;
};

#endif /* SRC_CELESTIALBODY_H_ */
