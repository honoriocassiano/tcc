/*
 * Star2.cpp
 *
 *  Created on: 3 de ago de 2017
 *      Author: cassiano
 */

#include "Star2.h"

#include "Quadtree/Math/PerlinNoise.h"
#include <sstream>
#include "Constants.h"

namespace pssg {

#define ATMOSPHERE_MESH_SEGMENTS  64
#define ATMOSPHERE_MESH_RINGS     (ATMOSPHERE_MESH_SEGMENTS / 2)

using std::abs;
using std::fixed;
using std::max;
using std::min;
using std::ostringstream;
using std::showpoint;

unsigned int Star2::instanceCount = 0;
bool Star2::wireframe = false;
//bool Star2::renderOrbits = false;

float to_rad = M_PI / 180.0;

bool Star2::GetWireframe() {
	return wireframe;
}

void Star2::SetWireframe(const bool value) {
	wireframe = value;
}

Star2::Star2(const double radius) :
		initializer(radius, Vector3d()),
		terrain(new RidgedMultifractalSphericalQuadtreeTerrain(radius)), animationTimeElapsed(
				0.0), orbitOrientation(
				Matrix3x3<double>::CreateIdentityMatrix()), orbitDistance(0.0), orbitAngleStart(
				0.0), orbitAngularVelocity(0.0), baseOrientation(
				Matrix3x3<double>::CreateIdentityMatrix()), rotationAngleStart(
				0.0), rotationAngularVelocity(0.0) {

	// Animate once to properly initialize position and orientation
//	Animate(0.0);
}

Star2::~Star2() {
	delete terrain;
}

void Star2::UpdateFrustum() {
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	if (translateWhenRendering)
		glTranslated(position.x, position.y, position.z);
	glMultTransposeMatrixd(Matrix4x4<double>(orientation).data1d);

	terrain->SetFrustum(Frustum());
	glPopMatrix();
}

void Star2::SetCameraPosition(const Vector3<double> &cameraPosition) {
	terrain->SetCameraPosition(TransformToLocal(cameraPosition, true));
}

RidgedMultifractalSphericalQuadtreeTerrain* Star2::GetTerrain() const {
	return terrain;
}

double Star2::GetAnimationTimeElapsed() const {
	return animationTimeElapsed;
}

void Star2::SetAnimationOrbit(const Matrix3x3<double> &orbitOrientation,
		const double orbitDistance, const double orbitAngleStart,
		const double orbitAngularVelocity) {
	// Set animation variables
	this->orbitOrientation = orbitOrientation;
	this->orbitDistance = orbitDistance;
	this->orbitAngleStart = orbitAngleStart;
	this->orbitAngularVelocity = orbitAngularVelocity;
}

void Star2::SetAnimationRotation(const Matrix3x3<double> &baseOrientation,
		const double rotationAngleStart, double rotationAngularVelocity) {
	// Set animation variables
	this->baseOrientation = baseOrientation;
	this->rotationAngleStart = rotationAngleStart;
	this->rotationAngularVelocity = rotationAngularVelocity;
}

double calculateVelocity(double mass, double semiMajorAxis,
		const Vector3<double>& orbiter, const Vector3<double>& Star2) {

	auto r_v = Star2 - orbiter;

	auto r = r_v.GetLength();

	return sqrt(
			mass * pssg::constant::G()
					* (2 / (r * pssg::Scale::getLength())
							- 1 / (semiMajorAxis * pssg::Scale::getLength())));
}

void Star2::Update() {
// Update terrain
	terrain->Update();
}

void Star2::Animate(const double timeDelta) {

	// Update planet orientation (rotation about y axis)
	for (auto e : effects) {
		e->update(timeDelta);
	}
}

void Star2::Render() const {
// Enable lighting and backface culling
	glPushAttrib(GL_ENABLE_BIT | GL_POLYGON_BIT);
	glDisable(GL_LIGHTING);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	if (wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	if (translateWhenRendering)
		glTranslated(position.x, position.y, position.z);
	glMultTransposeMatrixd(Matrix4x4<double>(orientation).data1d);

	terrain->Render();

	for (auto e : effects) {
		e->render();
	}

	glPopMatrix();
}

Vector3<double> Star2::GetLightDirection() const {
//return lightDirection;
	return TransformToLocal(-position.GetNormalized(), false);
//return TransformToWorld(lightDirection, false);
}

void Star2::SetLightDirection(const Vector3<double> &lightDirection) {
	this->lightDirection = lightDirection;
}

double Star2::GetBoundingRadius() const {
	double boundingRadius = terrain->GetRadius();

	return boundingRadius;
}

Vector3<double> Star2::GetClosestSurfacePoint(
		const Vector3<double> &pos) const {
	const Vector3<double> posLocal = TransformToLocal(pos, true);
	const Vector3<double> surfaceDirLocal = posLocal.GetNormalized();
	const Vector3<double> surfacePosLocal = surfaceDirLocal
			* terrain->GetTerrainHeight(surfaceDirLocal);
	return TransformToWorld(surfacePosLocal, true);
}

double Star2::GetClosestSurfaceDistance(const Vector3<double> &pos) const {
	const Vector3<double> posLocal = TransformToLocal(pos, true);
	const Vector3<double> surfaceDirLocal = posLocal.GetNormalized();
	const Vector3<double> surfacePosLocal = surfaceDirLocal
			* terrain->GetTerrainHeight(surfaceDirLocal);
	const bool inside = surfacePosLocal.GetSquaredLength()
			> posLocal.GetSquaredLength();
	const double distanceToSurface = (TransformToWorld(surfacePosLocal, true)
			- pos).GetLength();
	return inside ? -distanceToSurface : distanceToSurface;
}

Vector3<double> Star2::GetGravitationalAccelerationLocal(
		const Vector3<double> &pos) const {
	const double distance = pos.GetLength() * LENGTH_SCALE;
	const Vector3<double> direction = pos.GetNormalized();
	return direction
			* (-GRAVITATIONAL_CONSTANT_G * MASS_SCALE * mass
					/ (distance * distance));
}

double Star2::GetGravitationalAccelerationAtSurface() const {
	const double radius = terrain->GetRadius() * LENGTH_SCALE;
	return GRAVITATIONAL_CONSTANT_G * MASS_SCALE * mass / (radius * radius);
}

string Star2::GetString() const {
	ostringstream Star2String;
	Star2String.precision(2);
	Star2String << (name.length() ? name : "Uncharted Star2")
			<< "\nRadius:           " << showpoint
			<< terrain->GetRadius() * LENGTH_SCALE << " m"
			<< "\nG. Acceleration:  " << showpoint << fixed
			<< GetGravitationalAccelerationAtSurface() << " m/s^2";
	return Star2String.str();
}

Vector3<double> Star2::CalculateOrbitGravity() {

	Vector3<double> og;

	if (parent) {
		//		Vec3f vec = getCenter() - getOrbiter()->getCenter();
		const auto vec = GetParentPosition() - position;

		float r3 = pow(vec.GetLength() * pssg::Scale::getLength(), 2.0);

		og = vec.GetNormalized() * pssg::constant::G()
				* ((GetMass() * parent->GetMass()) / r3);

//		orbitGravity = vec.normalized() * constant::G()
//				* ((getMass() * getOrbiter()->getMass()) / r3);

	}

	return og;
}

void Star2::AddParticleEffects(const char* textureFilename,
		unsigned int nParticles,
		const ParticleEffects::ColorInterpolator& colors,
		float particleInitialSize, float minLifeTime, float maxLifeTime) {

	effects.push_back(
			new ParticleEffects(nParticles, minLifeTime, maxLifeTime));
	effects.back()->setParticleInitializer(&initializer);
	effects.back()->loadTexture(textureFilename);
	effects.back()->setColorInterplator(colors);
	effects.back()->setSphereColor(sphereColor);
	effects.back()->setParticleSizeRange(particleInitialSize, 0);
}

ParticleEffects* Star2::GetEffects(int index) {
	return effects[index];
}

void Star2::SetSphereColor(const Vector3d& color) {
	sphereColor = color;
}

} /* namespace pssg */
