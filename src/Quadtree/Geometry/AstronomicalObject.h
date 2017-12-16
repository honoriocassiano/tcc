/*******************************************************************************
 * File:           AstronomicalObject.h
 * Author:         Christian Alfons
 * Date created:   2010-07-16
 * Date modified:  2011-01-22
 * Description:    Astronomical object class. Represents astronomical bodies,
 *                 such as planets, stars, etc.
 *******************************************************************************/

#ifndef ASTRONOMICALOBJECT_H_INCLUDED
#define ASTRONOMICALOBJECT_H_INCLUDED

#include "Geometry.h"
#include <string>
#include <vector>

// Units are scaled for convenience
#define GRAVITATIONAL_CONSTANT_G  6.67428e-11
#define LENGTH_SCALE              1.0e6
#define MASS_SCALE                1.0e18

using std::string;
using std::vector;

class AstronomicalObject: public Geometry {

protected:
	// Class variables
	static bool translateWhenRendering;

	// Instance variables
	AstronomicalObject *parent;
	vector<AstronomicalObject*> children;
	string name;
	double mass;
	double semiMajorAxis;

public:
	// Class methods
	static void SetTranslateWhenRendering(const bool value);
	static bool GetTranslateWhenRendering();

	// Constructor
	AstronomicalObject();

	// Destructor
	virtual ~AstronomicalObject();

	// Instance methods
	virtual double GetBoundingRadius() const = 0;
	virtual double GetHoverBoundingRadius() const;
	virtual Vector3<double> GetClosestSurfacePoint(
			const Vector3<double> &pos) const = 0;
	virtual double GetClosestSurfaceDistance(
			const Vector3<double> &pos) const = 0;
	const AstronomicalObject* GetParent() const;
	void SetParent(AstronomicalObject *newParent);
	string GetName() const;
	void SetName(const string &name);
	double GetMass() const;
	void SetMass(const double mass);
	Vector3<double> GetParentPosition() const;
	virtual string GetString() const;
};

#endif
