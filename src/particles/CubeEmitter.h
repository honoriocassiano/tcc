#include "ParticleEmitter.h"
#include "../Quadtree/Math/VectorMath.h"

class CubeEmitter: public ParticleEmitter {
public:
	CubeEmitter();

	virtual void EmitParticle(Particle& particle);
	virtual void DebugRender();

	float MinWidth;
	float MaxWidth;

	float MinHeight;
	float MaxHeight;

	float MinDepth;
	float MaxDepth;

	float MinSpeed;
	float MaxSpeed;

	float MinLifetime;
	float MaxLifetime;

	Vector3d Origin;

};
