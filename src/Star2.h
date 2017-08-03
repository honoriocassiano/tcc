/*
 * Star2.h
 *
 *  Created on: 3 de ago de 2017
 *      Author: cassiano
 */

#ifndef SRC_STAR2_H_
#define SRC_STAR2_H_

#include "Quadtree/Geometry/AstronomicalObject.h"
#include "Quadtree/Geometry/QuadtreeTerrain/RidgedMultifractalSphericalQuadtreeTerrain.h"
#include "Quadtree/Geometry/Atmosphere.h"
#include "Quadtree/Geometry/CloudLayer.h"
#include "Quadtree/Geometry/PlanetRing.h"

#include "particles/ParticleEffects.h"
#include "particles/ParticleInitializer.h"

namespace pssg {

class Star2: public AstronomicalObject {

private:
	// Class variables
	static unsigned int instanceCount;
	static bool wireframe;
	static bool renderOrbits;

	// TODO Mode this method to parent class (???)
	Vector3<double> CalculateOrbitGravity();

	// Instance variables

	RidgedMultifractalSphericalQuadtreeTerrain *terrain;
	Vector3<double> lightDirection;
	double animationTimeElapsed;
	Matrix3x3<double> orbitOrientation;
	double orbitDistance;
	double orbitAngleStart;
	double orbitAngularVelocity;
	Matrix3x3<double> baseOrientation;
	double rotationAngleStart;
	double rotationAngularVelocity;

	Vector3<double> orbitGravity;

	ParticleInitializer initializer;
	std::vector<ParticleEffects*> effects;

	Vector3d sphereColor;

public:
	// Class methods
	static bool GetWireframe();
	static void SetWireframe(const bool value);
//	static bool GetRenderOrbits();
//	static void SetRenderOrbits(const bool value);

// Constructor
	Star2(const double radius = 1.0);

	// Destructor
	~Star2();

	// Instance methods
	void UpdateFrustum();
	void SetCameraPosition(const Vector3<double> &cameraPosition);
	RidgedMultifractalSphericalQuadtreeTerrain* GetTerrain() const;
//	void RenderOrbit() const;
	virtual void Render() const;
	virtual void Update();
	double GetAnimationTimeElapsed() const;
	void SetAnimationOrbit(const Matrix3x3<double> &orbitOrientation,
			const double orbitDistance, const double orbitAngleStart,
			const double orbitAngularVelocity);
	void SetAnimationRotation(const Matrix3x3<double> &baseOrientation,
			const double rotationAngleStart, double rotationAngularVelocity);
	void Animate(const double timeDelta);
	Vector3<double> GetLightDirection() const;
	void SetLightDirection(const Vector3<double> &lightDirection);
	virtual double GetBoundingRadius() const;
	virtual Vector3<double> GetClosestSurfacePoint(
			const Vector3<double> &pos) const;
	virtual double GetClosestSurfaceDistance(const Vector3<double> &pos) const;
	Vector3<double> GetGravitationalAccelerationLocal(
			const Vector3<double> &pos) const;
	double GetGravitationalAccelerationAtSurface() const;
	virtual string GetString() const;

	void AddParticleEffects(const char *textureFilename,
			unsigned int nParticles,
			const ParticleEffects::ColorInterpolator& colors,
			float particleInitialSize, float minLifeTime, float maxLifeTime);

	ParticleEffects *GetEffects(int index);

	void SetSphereColor(const Vector3d& color);
};

} /* namespace pssg */

#endif /* SRC_STAR2_H_ */
