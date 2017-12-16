/*******************************************************************************
 * File:           Planet.h
 * Author:         Christian Alfons
 * Date created:   2010-07-16
 * Date modified:  2011-01-22
 * Description:    Planet class. Uses (RMF) spherical quadtree terrain. Can be
 *                 animated to rotate and go in a circular orbit.
 *******************************************************************************/

#ifndef PLANET_H_INCLUDED
#define PLANET_H_INCLUDED

#include "AstronomicalObject.h"
#include "QuadtreeTerrain/RidgedMultifractalSphericalQuadtreeTerrain.h"
#include "Atmosphere.h"
#include "CloudLayer.h"
#include "PlanetRing.h"
#include "SimpleCylinder.h"
#include "SimpleCircle.h"

class CylinderAxis {
private:
	const SimpleCylinder *topCyl, *midCyl, *botCyl;

public:
	CylinderAxis(const float r, const float h, const float t) :
			topCyl(new SimpleCylinder(r, 0, t, 8, 2, 0.5 * h)), midCyl(
					new SimpleCylinder(r, r, h, 8, 4, -0.5 * h)), botCyl(
					new SimpleCylinder(0, r, t, 8, 2, -0.5 * h - t)) {
	}
	~CylinderAxis() {
		delete topCyl;
		delete midCyl;
		delete botCyl;
	}
	void Render() {
		topCyl->Render();
		midCyl->Render();
		botCyl->Render();
	}
};

class Planet: public AstronomicalObject {

private:
	// Class variables
	static unsigned int instanceCount;
	static bool wireframe;
	static CylinderAxis *cylinderAxis;
	static SimpleCircle *orbitCircle;
	static bool renderOrbits;

	// TODO Mode this method to parent class (???)
	Vector3<double> CalculateOrbitGravity();

	// Instance variables

	RidgedMultifractalSphericalQuadtreeTerrain *terrain;
	Atmosphere *atmosphere;
	CloudLayer *cloudLayer;
	PlanetRing *planetRing;
	GLuint texColormap;
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

	float orbitEccentricity;


public:
	// Class methods
	static bool GetWireframe();
	static void SetWireframe(const bool value);
	static bool GetRenderOrbits();
	static void SetRenderOrbits(const bool value);

	// Constructor
	Planet(const double radius = 1.0, const float orbitEccentricity = 0);

	// Destructor
	~Planet();

	// Instance methods
	void BuildColormap(const vector<Vector4<double> > &colors,
			const unsigned int size = 256);
	void UpdateFrustum();
	void SetCameraPosition(const Vector3<double> &cameraPosition);
	RidgedMultifractalSphericalQuadtreeTerrain* GetTerrain() const;
	void RenderOrbit() const;
	virtual void Render() const;
	virtual void Update();
	void SetRing(const double innerRadius, const double outerRadius,
			const Vector4<double> &ringColor);
	void SetAtmosphere(const double atmosphereRadius,
			const Vector3<float> &atmosphereColor);
	void SetClouds(const double cloudLayerRadius, const double cloudCover = 0.5,
			const double cloudSharpness = 0.01);
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
};

#endif
