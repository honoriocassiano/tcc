/*
 * Star3.h
 *
 *  Created on: 3 de ago de 2017
 *      Author: cassiano
 */

#ifndef SRC_STAR3_H_
#define SRC_STAR3_H_

#include "Quadtree/Geometry/AstronomicalObject.h"
#include "Quadtree/Geometry/QuadtreeTerrain/RidgedMultifractalSphericalQuadtreeTerrain.h"
#include "Quadtree/Geometry/Atmosphere.h"
#include "Quadtree/Geometry/CloudLayer.h"
#include "Quadtree/Geometry/PlanetRing.h"

#include "particles/ParticleEffects2.h"
#include "particles/ParticleInitializer.h"

namespace pssg {

class Star3: public AstronomicalObject {
public:

	Star3(double radius, Vector3d position, Vector3d sphereColor);
	void Render() const;
	void AddParticleEffects(const char *textureFilename,
			unsigned int nParticles,
			const ParticleEffects2::ColorInterpolator& colors,
			float particleInitialSize, float minLifeTime, float maxLifeTime);

	virtual Vector3<double> GetClosestSurfacePoint(const Vector3d &pos) const;
	virtual double GetBoundingRadius() const;

	virtual double GetHoverBoundingRadius() const;
	virtual double GetClosestSurfaceDistance(const Vector3<double> &pos) const;

	void Update(GLfloat deltaTime);
	ParticleEffects2 *GetEffects(int index);

private:
//	float cameraDistance;
	const double radius;
	Vector3d sphereColor;
	ParticleInitializer initializer;
	std::vector<ParticleEffects2*> effects;
};

} /* namespace pssg */

#endif /* SRC_STAR2_H_ */
