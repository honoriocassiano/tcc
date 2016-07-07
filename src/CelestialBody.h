/*
 * CelestialBody.h
 *
 *  Created on: 14 de abr de 2016
 *      Author: cassiano
 */

#ifndef SRC_CELESTIALBODY_H_
#define SRC_CELESTIALBODY_H_

#include "vectors.h"
#include "mesh.h"

#include "Time.h"

class CelestialBody {
public:
	CelestialBody(const Vec3f& center) :
			mCenter(center), mMesh(new Mesh()), mMass(0.0f), mOrbiter(nullptr), mSemiMajorAxis(0) {
	}
	virtual ~CelestialBody() {
		delete mMesh;
	}

	void setCenter(const Vec3f& position) {
		mCenter = position;
	}
	const Vec3f getCenter() const {
		return mCenter;
	}

	float getMass() const {
		return mMass;
	}

	void setMass(float mass) {
		mMass = mass;
	}

	Mesh * getMesh() {
		return mMesh;
	}

	const CelestialBody* getOrbiter() const {
		return mOrbiter;
	}

	void setOrbiter(CelestialBody* orbiter) {
		mOrbiter = orbiter;
		if(orbiter) {
			float distance = (mCenter - orbiter->getCenter()).Length();

			mSemiMajorAxis = (distance / 2) * 1.2;
		} else {
			mSemiMajorAxis = 0.0f;
		}
	}

	float getSemiMajorAxis() const {
		return mSemiMajorAxis;
	}

	virtual void update(const Time& dt) = 0;
	virtual void draw() = 0;

private:

protected:
	Bag<Triangle*>* getTriangles() {
		return mMesh->triangles;
	}

	void recalculateNormals() {
		mMesh->computeFaceNormals();
		mMesh->computeVerticesNormals();
	}

	Vec3f mCenter;
	Mesh * mMesh;

	CelestialBody * mOrbiter;

	float mMass;
	float mSemiMajorAxis;
};

#endif /* SRC_CELESTIALBODY_H_ */
