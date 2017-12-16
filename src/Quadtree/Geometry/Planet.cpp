#include "Planet.h"
#include "../Math/PerlinNoise.h"
#include <sstream>
#include "../../Constants.h"

#define ATMOSPHERE_MESH_SEGMENTS  64
#define ATMOSPHERE_MESH_RINGS     (ATMOSPHERE_MESH_SEGMENTS / 2)

using std::abs;
using std::fixed;
using std::max;
using std::min;
using std::ostringstream;
using std::showpoint;

unsigned int Planet::instanceCount = 0;
bool Planet::wireframe = false;
CylinderAxis* Planet::cylinderAxis = 0;
SimpleCircle* Planet::orbitCircle = 0;
bool Planet::renderOrbits = false;

float to_rad = M_PI / 180.0;

bool Planet::GetWireframe() {
	return wireframe;
}

void Planet::SetWireframe(const bool value) {
	wireframe = value;
}

bool Planet::GetRenderOrbits() {
	return renderOrbits;
}

void Planet::SetRenderOrbits(const bool value) {
	renderOrbits = value;
}

Planet::Planet(const double radius, const float _orbitEccentricity) :
		orbitEccentricity(_orbitEccentricity), terrain(
				new RidgedMultifractalSphericalQuadtreeTerrain(radius)), atmosphere(
				0), cloudLayer(0), planetRing(0), texColormap(0), lightDirection(
				Vector3<double>(0.0, 0.0, 1.0).GetNormalized()), animationTimeElapsed(
				0.0), orbitOrientation(
				Matrix3x3<double>::CreateIdentityMatrix()), orbitDistance(0.0), orbitAngleStart(
				0.0), orbitAngularVelocity(0.0), baseOrientation(
				Matrix3x3<double>::CreateIdentityMatrix()), rotationAngleStart(
				0.0), rotationAngularVelocity(0.0) {
	// If first instance to be created
	if (!instanceCount++) {
//		cylinderAxis = new CylinderAxis(0.076, 3.5, 0.5);
		orbitCircle = new SimpleCircle(1.0, 128);
	}

// Build initial color map
	BuildColormap(vector<Vector4<double> >(), 1);

	// Animate once to properly initialize position and orientation
	Animate(0.0);
}

Planet::~Planet() {
	// If last instance to be deleted
	if (!--instanceCount) {
		delete cylinderAxis;
		delete orbitCircle;
	}

	delete atmosphere;
	delete cloudLayer;
	delete terrain;
	glDeleteTextures(1, &texColormap);
}

// Builds a colormap texture using given colors (alpha component is colormap position)
void Planet::BuildColormap(const vector<Vector4<double> > &colors,
		const unsigned int size) {
	// Calculate colormap data
	GLubyte data[size][size][3];
	for (unsigned int x = 0; x < size; x++) {
		// Calculate height color
		Vector4<double> heightColor(1.0, 1.0, 1.0, 1.0);
		if (colors.size() > 0) {
			const double height = x / ((double) size - 1.0);
			if (height <= colors.front().a)
				// Use bottom color
				heightColor = colors.front();
			else if (height >= colors.back().a)
				// Use top color
				heightColor = colors.back();
			else
				// Interpolate colors
				for (unsigned int i = 1; i < colors.size(); i++)
					if (height > colors[i - 1].a && height < colors[i].a) {
						const double t =
								min(1.0,
										max(0.0,
												(height - colors[i - 1].a)
														/ (colors[i].a
																- colors[i - 1].a)));
						heightColor = colors[i - 1] * (1.0 - t) + colors[i] * t;
						break;
					}
		}
		for (unsigned int y = 0; y < size; y++) {
			//const double slope = y / ((double)size - 1.0);
			data[y][x][0] = (int) (heightColor.b * 255);
			data[y][x][1] = (int) (heightColor.g * 255);
			data[y][x][2] = (int) (heightColor.r * 255);
		}
	}

	// Remove old colormap texture
	if (texColormap)
		glDeleteTextures(1, &texColormap);

	// Create new colormap texture
	glGenTextures(1, &texColormap);
	glBindTexture(GL_TEXTURE_2D, texColormap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, size, size, 0, GL_BGR,
	GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_1D);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Planet::UpdateFrustum() {
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	if (translateWhenRendering)
		glTranslated(position.x, position.y, position.z);
	glMultTransposeMatrixd(Matrix4x4<double>(orientation).data1d);

	terrain->SetFrustum(Frustum());
	glPopMatrix();
}

void Planet::SetCameraPosition(const Vector3<double> &cameraPosition) {
	terrain->SetCameraPosition(TransformToLocal(cameraPosition, true));
}

RidgedMultifractalSphericalQuadtreeTerrain* Planet::GetTerrain() const {
	return terrain;
}

void Planet::SetRing(const double innerRadius, const double outerRadius,
		const Vector4<double> &ringColor) {
	delete planetRing;
	planetRing = new PlanetRing(this, innerRadius, outerRadius, ringColor);
}

void Planet::SetAtmosphere(const double atmosphereRadius,
		const Vector3<float> &atmosphereColor) {
	delete atmosphere;
	if (atmosphereRadius == 0.0)
		atmosphere = 0;
	else
		atmosphere = new Atmosphere(this, atmosphereRadius, atmosphereColor);
}

void Planet::SetClouds(const double cloudLayerRadius, const double cloudCover,
		const double cloudSharpness) {
	delete cloudLayer;
	if (cloudLayerRadius == 0.0)
		cloudLayer = 0;
	else
		cloudLayer = new CloudLayer(this, cloudLayerRadius, cloudCover,
				cloudSharpness);
}

double Planet::GetAnimationTimeElapsed() const {
	return animationTimeElapsed;
}

void Planet::SetAnimationOrbit(const Matrix3x3<double> &orbitOrientation,
		const double orbitDistance, const double orbitAngleStart,
		const double orbitAngularVelocity) {
	// Set animation variables
	this->orbitOrientation = orbitOrientation;
	this->orbitDistance = orbitDistance;
	this->orbitAngleStart = orbitAngleStart;
	this->orbitAngularVelocity = orbitAngularVelocity;
}

void Planet::SetAnimationRotation(const Matrix3x3<double> &baseOrientation,
		const double rotationAngleStart, double rotationAngularVelocity) {
	// Set animation variables
	this->baseOrientation = baseOrientation;
	this->rotationAngleStart = rotationAngleStart;
	this->rotationAngularVelocity = rotationAngularVelocity;
}

double calculateVelocity(double mass, double semiMajorAxis,
		const Vector3<double>& orbiter, const Vector3<double>& planet) {

	auto r_v = planet - orbiter;

	auto r = r_v.GetLength();

	return sqrt(
			mass * pssg::constant::G()
					* (2 / (r * pssg::Scale::getLength())
							- 1 / (semiMajorAxis * pssg::Scale::getLength())));
}

void Planet::Animate(const double timeDelta) {

	// Update planet orientation (rotation about y axis)
	orientation = baseOrientation
			* Matrix3x3<double>::CreateRotationMatrixY(
					rotationAngleStart
							+ animationTimeElapsed * rotationAngularVelocity);

	animationTimeElapsed += timeDelta;

	// Update planet position
	if (parent) {

		if(semiMajorAxis < 0.0 && orbitEccentricity > 0) {
			semiMajorAxis = (position - parent->GetParentPosition()).GetSquaredLength() * orbitEccentricity;
		}

		orbitGravity = CalculateOrbitGravity();

		double velocity = calculateVelocity(this->mass, semiMajorAxis,
				parent->GetPosition(), this->position);

		auto normal = (this->position - parent->GetPosition()).GetNormalized();

		auto matrix = Matrix4x4<double>::CreateRotationMatrixY(90 * to_rad);

		normal = matrix * normal;

		auto normalVelocity = normal * velocity;

		auto direction = (normalVelocity + orbitGravity) * timeDelta;

		position += direction;
	}
}

void Planet::Update() {
// Update terrain
	terrain->Update();
}

void Planet::RenderOrbit() const {
	if (orbitDistance == 0.0 || !orbitCircle)
		return;

	// Set orbit transformation
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	const Vector3<double> parentPosition = GetParentPosition();
	glTranslated(parentPosition.x, parentPosition.y, parentPosition.z);

	const double semiminorAxis = sqrt(
			pow(orbitDistance, 2)
					- pow((orbitDistance * orbitEccentricity), 2));

	glScaled(semiminorAxis, orbitDistance, orbitDistance);
	const Matrix4x4<double> orbitMatrix = Matrix4x4<double>(orbitOrientation)
			* Matrix4x4<double>::CreateRotationMatrixX(0.5 * M_PI);
	glMultTransposeMatrixd(orbitMatrix.data1d);

	// Render
	orbitCircle->Render();

	glPopMatrix();
}

void Planet::Render() const {
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

// Render the faces
	ShaderManager::UseShaderProgram(PLANET);
	static const GLint uniformLightDir = glGetUniformLocation(
			ShaderManager::GetShaderProgramHandle(PLANET), "lightDir");
	glUniform3f(uniformLightDir, (float) GetLightDirection().x,
			(float) GetLightDirection().y, (float) GetLightDirection().z);
	static const GLint uniformColormap = glGetUniformLocation(
			ShaderManager::GetShaderProgramHandle(PLANET), "colormap");
	glUniform1i(uniformColormap, 0);
	static const GLint uniformAtmosphereColor = glGetUniformLocation(
			ShaderManager::GetShaderProgramHandle(PLANET), "atmosphereColor");
	glUniform3fv(uniformAtmosphereColor, 1,
			atmosphere ?
					atmosphere->GetColor().data :
					Vector3<float>(0.0, 0.0, 0.0).data);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texColormap);
	terrain->Render();
	glBindTexture(GL_TEXTURE_2D, 0);

	if (cylinderAxis && renderOrbits) {
		ShaderManager::UseShaderProgram(AXIS);
		const float scale = terrain->GetRadius();
		glPushMatrix();
		glScaled(scale, scale, scale);
		cylinderAxis->Render();
		glPopMatrix();
	}

// Restore render state
	glUseProgram(0);
	glPopAttrib();

// Render atmosphere
	if (atmosphere)
		atmosphere->Render();

// Render clouds
	if (cloudLayer)
		cloudLayer->Render();

// Render ring
	if (planetRing)
		planetRing->Render();

	glPopMatrix();
}

Vector3<double> Planet::GetLightDirection() const {
//return lightDirection;
	return TransformToLocal(-position.GetNormalized(), false);
//return TransformToWorld(lightDirection, false);
}

void Planet::SetLightDirection(const Vector3<double> &lightDirection) {
	this->lightDirection = lightDirection;
}

double Planet::GetBoundingRadius() const {
	double boundingRadius = terrain->GetRadius();
	if (atmosphere)
		boundingRadius = max(boundingRadius, atmosphere->GetRadius());
	return boundingRadius;
}

Vector3<double> Planet::GetClosestSurfacePoint(
		const Vector3<double> &pos) const {
	const Vector3<double> posLocal = TransformToLocal(pos, true);
	const Vector3<double> surfaceDirLocal = posLocal.GetNormalized();
	const Vector3<double> surfacePosLocal = surfaceDirLocal
			* terrain->GetTerrainHeight(surfaceDirLocal);
	return TransformToWorld(surfacePosLocal, true);
}

double Planet::GetClosestSurfaceDistance(const Vector3<double> &pos) const {
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

Vector3<double> Planet::GetGravitationalAccelerationLocal(
		const Vector3<double> &pos) const {
	const double distance = pos.GetLength() * LENGTH_SCALE;
	const Vector3<double> direction = pos.GetNormalized();
	return direction
			* (-GRAVITATIONAL_CONSTANT_G * MASS_SCALE * mass
					/ (distance * distance));
}

double Planet::GetGravitationalAccelerationAtSurface() const {
	const double radius = terrain->GetRadius() * LENGTH_SCALE;
	return GRAVITATIONAL_CONSTANT_G * MASS_SCALE * mass / (radius * radius);
}

string Planet::GetString() const {
	ostringstream planetString;
	planetString.precision(2);
	planetString << (name.length() ? name : "Uncharted Planet")
			<< "\nRadius:           " << showpoint
			<< terrain->GetRadius() * LENGTH_SCALE << " m"
			<< "\nG. Acceleration:  " << showpoint << fixed
			<< GetGravitationalAccelerationAtSurface() << " m/s^2"
			<< "\nHabitable:        " << (atmosphere ? "Yes" : "No");
	return planetString.str();
}

Vector3<double> Planet::CalculateOrbitGravity() {

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
