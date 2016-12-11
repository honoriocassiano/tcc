#ifndef _VERTEX_H
#define _VERTEX_H

#include <stdio.h>
#include <assert.h>

#include "../vectors.h"
#include "../Color.h"

class Vertex;

// ==========================================================

class Vertex {

public:

	// ========================
	// CONSTRUCTOR & DESTRUCTOR
	Vertex(int i, const Vec3f &pos) :
			position(pos), color(1.0f, 1.0f, 1.0f), active(false), level(0) {
		index = i;
	}
	virtual ~Vertex() {
	}

	// =========
	// ACCESSORS
	int getIndex() const {
		return index;
	}
	double x() const {
		return position.x();
	}
	double y() const {
		return position.y();
	}
	double z() const {
		return position.z();
	}
	const Vec3f& get() const {
		return position;
	}

	const Vec3f& getNormal() const {
		return normal;
	}
	const Color& getColor() const {
		return color;
	}

	bool isActive() const {
		return active;
	}
	std::size_t getLevel() const {
		return level;
	}

	// =========
	// MODIFIERS
	void set(const Vec3f& v) {
		position = v;
	}
	void set(double x, double y, double z) {
		position.Set(x, y, z);
	}

	void setNormal(const Vec3f& v) {
		normal = v;
	}

	void setNormal(double x, double y, double z) {
		normal.Set(x, y, z);
	}

	void setColor(const Color& c) {
		color = c;
	}

	void setLevel(std::size_t l) {
		this->level = l;
	}

	void setActive(bool active) {
		this->active = active;
	}

private:

	// don't use these constructors
	Vertex() {
		assert(0);
	}
	Vertex& operator=(const Vertex&) {
		assert(0);
	}
	Vertex(const Vertex&) {
		assert(0);
	}

	// ==============
	// REPRESENTATION
	Vec3f position;
	Vec3f normal;
	Color color;
	bool active;
	std::size_t level;

	// this is the index from the original .obj file.
	// technically not part of the half-edge data structure
	int index;

	// NOTE: the vertices don't know anything about adjacency.  In some
	// versions of this data structure they have a pointer to one of
	// their incoming edges.  However, this data is very complicated to
	// maintain during mesh manipulation.

};

// ==========================================================

#endif

