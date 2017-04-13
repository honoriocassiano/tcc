/*
 * vectors2.h
 *
 *  Created on: 12 de abr de 2017
 *      Author: cassiano
 */

#ifndef SRC_STRUCTURES_VECTORS_H_
#define SRC_STRUCTURES_VECTORS_H_

#include <iostream>
//using namespace std;

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <cstring>
#include <algorithm>

#include <ostream>
#include <string>
#include <sstream>

//namespace vec2 {
//
// originally implemented by Justin Legakis
//

class Matrix;

// ====================================================================
// ====================================================================

enum class Metric
	: unsigned char {EUCLIDIAN, MAXIMUM
};

class Vec2f {

public:

	// CONSTRUCTORS & DESTRUCTOR
	Vec2f() {
		data[0] = data[1] = 0;
	}
	Vec2f(const Vec2f &V) {
		data[0] = V.data[0];
		data[1] = V.data[1];
	}
	Vec2f(float d0, float d1) {
		data[0] = d0;
		data[1] = d1;
	}
	Vec2f(const Vec2f &V1, const Vec2f &V2) {
		data[0] = V1.data[0] - V2.data[0];
		data[1] = V1.data[1] - V2.data[1];
	}
	~Vec2f() {
	}

	// ACCESSORS
//	void Get(float &d0, float &d1) const {
//		d0 = data[0];
//		d1 = data[1];
//	}
	float operator[](int i) const {
		assert(i >= 0 && i < 2);
		return data[i];
	}
	float x() const {
		return data[0];
	}
	float y() const {
		return data[1];
	}

	void normalize(Metric metric = Metric::EUCLIDIAN) {
		float l = length(metric);
		if (l > 0) {
			data[0] /= l;
			data[1] /= l;
		}
	}

	Vec2f normalized(Metric metric = Metric::EUCLIDIAN) const {
		Vec2f temp = *this;

		temp.normalize(metric);

		return temp;
	}

	float length(Metric metric = Metric::EUCLIDIAN) const {
		float l;
		switch (metric) {
		case Metric::EUCLIDIAN:
			l = sqrtf(data[0] * data[0] + data[1] * data[1]);

			break;
		case Metric::MAXIMUM:
			l = fabs(data[0]) > fabs(data[1]) ? fabs(data[0]) : fabs(data[1]);

			break;
		}

		return l;
	}

	// MODIFIERS
	void set(float d0, float d1) {
		data[0] = d0;
		data[1] = d1;
	}
//	void scale(float d0, float d1) {
//		data[0] *= d0;
//		data[1] *= d1;
//	}
//	void divide(float d0, float d1) {
//		data[0] /= d0;
//		data[1] /= d1;
//	}

// OVERLOADED OPERATORS
	Vec2f& operator=(const Vec2f &V) {
		data[0] = V.data[0];
		data[1] = V.data[1];
		return *this;
	}

	Vec2f operator-() {
		Vec2f temp;

		temp.data[0] = -data[0];
		temp.data[1] = -data[1];

		return temp;
	}

	int operator==(const Vec2f &V) const {
		return ((data[0] == V.data[0]) && (data[1] == V.data[1]));
	}

	int operator!=(const Vec2f &V) {
		return ((data[0] != V.data[0]) || (data[1] != V.data[1]));
	}

//	friend Vec2f& operator+=(const Vec2f &V) {
//		data[0] += V.data[0];
//		data[1] += V.data[1];
//		return *this;
//	}
//
//	friend Vec2f& operator-=(const Vec2f &V) {
//		data[0] -= V.data[0];
//		data[1] -= V.data[1];
//		return *this;
//	}

	Vec2f& operator+=(const Vec2f &V) {
		data[0] += V.data[0];
		data[1] += V.data[1];
		return *this;
	}

	Vec2f& operator-=(const Vec2f &V) {
		data[0] -= V.data[0];
		data[1] -= V.data[1];
		return *this;
	}
	Vec2f& operator*=(float f) {
		data[0] *= f;
		data[1] *= f;
		return *this;
	}

	Vec2f& operator/=(float f) {
		data[0] /= f;
		data[1] /= f;
		return *this;
	}

	// OPERATIONS
	float dot2(const Vec2f &V) const {
		return data[0] * V.data[0] + data[1] * V.data[1];
	}

	// STATIC OPERATIONS
	static void add(Vec2f &a, const Vec2f &b, const Vec2f &c) {
		a.data[0] = b.data[0] + c.data[0];
		a.data[1] = b.data[1] + c.data[1];
	}
	static void sub(Vec2f &a, const Vec2f &b, const Vec2f &c) {
		a.data[0] = b.data[0] - c.data[0];
		a.data[1] = b.data[1] - c.data[1];
	}
	static void copyScale(Vec2f &a, const Vec2f &b, float c) {
		a.data[0] = b.data[0] * c;
		a.data[1] = b.data[1] * c;
	}
	static void addScale(Vec2f &a, const Vec2f &b, const Vec2f &c, float d) {
		a.data[0] = b.data[0] + c.data[0] * d;
		a.data[1] = b.data[1] + c.data[1] * d;
	}
	static void average(Vec2f &a, const Vec2f &b, const Vec2f &c) {
		a.data[0] = (b.data[0] + c.data[0]) * 0.5f;
		a.data[1] = (b.data[1] + c.data[1]) * 0.5f;
	}
	static void weightedSum(Vec2f &a, const Vec2f &b, float c, const Vec2f &d,
			float e) {
		a.data[0] = b.data[0] * c + d.data[0] * e;
		a.data[1] = b.data[1] * c + d.data[1] * e;
	}

	// INPUT / OUTPUT
	std::string str() const {

		std::stringstream ss;

		ss << "(" << data[0] << " " << data[1] << ")";

		return ss.str();
	}

	friend std::ostream& operator<<(std::ostream& os, const Vec2f& v) {
		return os << "(" << v.data[0] << " " << v.data[1] << ")";
	}

private:

	// REPRESENTATION
	float data[2];

};

// ====================================================================
// ====================================================================

class Vec3f {

public:

	// CONSTRUCTORS & DESTRUCTOR
	Vec3f() {
		data[0] = data[1] = data[2] = 0;
	}
	Vec3f(const Vec3f &V) {
		data[0] = V.data[0];
		data[1] = V.data[1];
		data[2] = V.data[2];
	}
	Vec3f(float d0, float d1, float d2) {
		data[0] = d0;
		data[1] = d1;
		data[2] = d2;
	}
	Vec3f(const Vec3f &V1, const Vec3f &V2) {
		data[0] = V1.data[0] - V2.data[0];
		data[1] = V1.data[1] - V2.data[1];
		data[2] = V1.data[2] - V2.data[2];
	}
	~Vec3f() {
	}

	// ACCESSORS
//	void get(float &d0, float &d1, float &d2) const {
//		d0 = data[0];
//		d1 = data[1];
//		d2 = data[2];
//	}
	float operator[](int i) const {
		assert(i >= 0 && i < 3);
		return data[i];
	}
	float x() const {
		return data[0];
	}
	float y() const {
		return data[1];
	}
	float z() const {
		return data[2];
	}
	float r() const {
		return data[0];
	}
	float g() const {
		return data[1];
	}
	float b() const {
		return data[2];
	}

	void normalize(Metric metric = Metric::EUCLIDIAN) {
		float l = length(metric);
		if (l > 0) {
			data[0] /= l;
			data[1] /= l;
			data[2] /= l;
		}
	}

	Vec3f normalized(Metric metric = Metric::EUCLIDIAN) const {
		Vec3f temp = *this;

		temp.normalize(metric);

		return temp;
	}

	float length(Metric metric = Metric::EUCLIDIAN) const {
		float l = 0;

		switch (metric) {
		case Metric::EUCLIDIAN:

			l = sqrtf(data[0] * data[0] + data[1] * data[1] + data[2] * data[2]);
			break;
		case Metric::MAXIMUM:

			for (auto i = 0; i < 3; ++i) {
				l = fabs(data[i]) > l ? fabs(data[i]) : l;
			}

			break;
		}
		return l;
	}

	// MODIFIERS
	void set(float d0, float d1, float d2) {
		data[0] = d0;
		data[1] = d1;
		data[2] = d2;
	}
//	void scale(float d0, float d1, float d2) {
//		data[0] *= d0;
//		data[1] *= d1;
//		data[2] *= d2;
//	}
//	void divide(float d0, float d1, float d2) {
//		data[0] /= d0;
//		data[1] /= d1;
//		data[2] /= d2;
//	}
	void clamp(float low = 0, float high = 1) {
		if (data[0] < low)
			data[0] = low;
		if (data[0] > high)
			data[0] = high;
		if (data[1] < low)
			data[1] = low;
		if (data[1] > high)
			data[1] = high;
		if (data[2] < low)
			data[2] = low;
		if (data[2] > high)
			data[2] = high;
	}

	// OVERLOADED OPERATORS
	Vec3f& operator=(const Vec3f &V) {
		data[0] = V.data[0];
		data[1] = V.data[1];
		data[2] = V.data[2];
		return *this;
	}

	Vec3f operator-() {
		Vec3f temp;

		temp.data[0] = -data[0];
		temp.data[1] = -data[1];
		temp.data[2] = -data[2];

		return temp;
	}

	int operator==(const Vec3f &V) {
		return ((data[0] == V.data[0]) && (data[1] == V.data[1])
				&& (data[2] == V.data[2]));
	}

	int operator!=(const Vec3f &V) {
		return ((data[0] != V.data[0]) || (data[1] != V.data[1])
				|| (data[2] != V.data[2]));
	}

	Vec3f& operator+=(const Vec3f &V) {
		data[0] += V.data[0];
		data[1] += V.data[1];
		data[2] += V.data[2];
		return *this;
	}

	Vec3f& operator-=(const Vec3f &V) {
		data[0] -= V.data[0];
		data[1] -= V.data[1];
		data[2] -= V.data[2];
		return *this;
	}
	Vec3f& operator*=(int i) {
		data[0] = float(data[0] * i);
		data[1] = float(data[1] * i);
		data[2] = float(data[2] * i);
		return *this;
	}

	Vec3f& operator*=(float f) {
		data[0] *= f;
		data[1] *= f;
		data[2] *= f;
		return *this;
	}

	Vec3f& operator/=(int i) {
		data[0] = float(data[0] / i);
		data[1] = float(data[1] / i);
		data[2] = float(data[2] / i);
		return *this;
	}

	Vec3f& operator/=(float f) {
		data[0] /= f;
		data[1] /= f;
		data[2] /= f;
		return *this;
	}

	friend Vec3f operator+(const Vec3f &v1, const Vec3f &v2) {
		Vec3f v3;
		add(v3, v1, v2);
		return v3;
	}

	friend Vec3f operator-(const Vec3f &v1, const Vec3f &v2) {
		Vec3f v3;
		sub(v3, v1, v2);
		return v3;
	}

	friend Vec3f operator*(const Vec3f &v1, float f) {
		Vec3f v2;
		copyScale(v2, v1, f);
		return v2;
	}
	friend Vec3f operator*(float f, const Vec3f &v1) {
		Vec3f v2;
		copyScale(v2, v1, f);
		return v2;
	}

	friend Vec3f operator*(const Vec3f &v1, const Vec3f &v2) {
		Vec3f v3;
		mult(v3, v1, v2);
		return v3;
	}

	// OPERATIONS
	float dot3(const Vec3f &V) const {
		return data[0] * V.data[0] + data[1] * V.data[1] + data[2] * V.data[2];
	}

	// STATIC OPERATIONS
	static void add(Vec3f &a, const Vec3f &b, const Vec3f &c) {
		a.data[0] = b.data[0] + c.data[0];
		a.data[1] = b.data[1] + c.data[1];
		a.data[2] = b.data[2] + c.data[2];
	}
	static void sub(Vec3f &a, const Vec3f &b, const Vec3f &c) {
		a.data[0] = b.data[0] - c.data[0];
		a.data[1] = b.data[1] - c.data[1];
		a.data[2] = b.data[2] - c.data[2];
	}
	static void mult(Vec3f &a, const Vec3f &b, const Vec3f &c) {
		a.data[0] = b.data[0] * c.data[0];
		a.data[1] = b.data[1] * c.data[1];
		a.data[2] = b.data[2] * c.data[2];
	}
	static void copyScale(Vec3f &a, const Vec3f &b, float c) {
		a.data[0] = b.data[0] * c;
		a.data[1] = b.data[1] * c;
		a.data[2] = b.data[2] * c;
	}
	static void addScale(Vec3f &a, const Vec3f &b, const Vec3f &c, float d) {
		a.data[0] = b.data[0] + c.data[0] * d;
		a.data[1] = b.data[1] + c.data[1] * d;
		a.data[2] = b.data[2] + c.data[2] * d;
	}
	static void average(Vec3f &a, const Vec3f &b, const Vec3f &c) {
		a.data[0] = (b.data[0] + c.data[0]) * 0.5f;
		a.data[1] = (b.data[1] + c.data[1]) * 0.5f;
		a.data[2] = (b.data[2] + c.data[2]) * 0.5f;
	}
	static void weightedSum(Vec3f &a, const Vec3f &b, float c, const Vec3f &d,
			float e) {
		a.data[0] = b.data[0] * c + d.data[0] * e;
		a.data[1] = b.data[1] * c + d.data[1] * e;
		a.data[2] = b.data[2] * c + d.data[2] * e;
	}
	static void cross3(Vec3f &c, const Vec3f &v1, const Vec3f &v2) {
		float x = v1.data[1] * v2.data[2] - v1.data[2] * v2.data[1];
		float y = v1.data[2] * v2.data[0] - v1.data[0] * v2.data[2];
		float z = v1.data[0] * v2.data[1] - v1.data[1] * v2.data[0];
		c.data[0] = x;
		c.data[1] = y;
		c.data[2] = z;
	}

	static void min(Vec3f &a, const Vec3f &b, const Vec3f &c) {
		a.data[0] = (b.data[0] < c.data[0]) ? b.data[0] : c.data[0];
		a.data[1] = (b.data[1] < c.data[1]) ? b.data[1] : c.data[1];
		a.data[2] = (b.data[2] < c.data[2]) ? b.data[2] : c.data[2];
	}
	static void max(Vec3f &a, const Vec3f &b, const Vec3f &c) {
		a.data[0] = (b.data[0] > c.data[0]) ? b.data[0] : c.data[0];
		a.data[1] = (b.data[1] > c.data[1]) ? b.data[1] : c.data[1];
		a.data[2] = (b.data[2] > c.data[2]) ? b.data[2] : c.data[2];
	}

	const std::string str() const {
		std::stringstream ss;

		ss << "(" << data[0] << ", " << data[1] << ", " << data[2] << ")";

		return ss.str();
	}

	friend std::ostream& operator<<(std::ostream& os, const Vec3f& v) {
		return os << "(" << v.data[0] << ", " << v.data[1] << ", " << v.data[2] << ")";
	}

private:

	friend class Matrix;

	// REPRESENTATION
	float data[3];

};

// ====================================================================
// ====================================================================

class Vec4f {

public:

	// CONSTRUCTORS & DESTRUCTOR
	Vec4f() {
		data[0] = data[1] = data[2] = data[3] = 0;
	}
	Vec4f(const Vec4f &V) {
		data[0] = V.data[0];
		data[1] = V.data[1];
		data[2] = V.data[2];
		data[3] = V.data[3];
	}
	Vec4f(float d0, float d1, float d2, float d3) {
		data[0] = d0;
		data[1] = d1;
		data[2] = d2;
		data[3] = d3;
	}
	Vec4f(const Vec3f &V, float w) {
		data[0] = V.x();
		data[1] = V.y();
		data[2] = V.z();
		data[3] = w;
	}
	Vec4f(const Vec4f &V1, const Vec4f &V2) {
		data[0] = V1.data[0] - V2.data[0];
		data[1] = V1.data[1] - V2.data[1];
		data[2] = V1.data[2] - V2.data[2];
		data[3] = V1.data[3] - V2.data[3];
	}
	~Vec4f() {
	}

	// ACCESSORS
//	void get(float &d0, float &d1, float &d2, float &d3) const {
//		d0 = data[0];
//		d1 = data[1];
//		d2 = data[2];
//		d3 = data[3];
//	}
	float operator[](int i) const {
		assert(i >= 0 && i < 4);
		return data[i];
	}
	float x() const {
		return data[0];
	}
	float y() const {
		return data[1];
	}
	float z() const {
		return data[2];
	}
	float w() const {
		return data[3];
	}
	float r() const {
		return data[0];
	}
	float g() const {
		return data[1];
	}
	float b() const {
		return data[2];
	}
	float a() const {
		return data[3];
	}

	void normalize(Metric metric = Metric::EUCLIDIAN) {
		float l = length(metric);
		if (l > 0) {
			data[0] /= l;
			data[1] /= l;
			data[2] /= l;
			data[3] /= l;
		}
	}

	Vec4f normalized(Metric metric = Metric::EUCLIDIAN) const {
		Vec4f temp = *this;

		temp.normalize(metric);

		return temp;
	}

	float length(Metric metric = Metric::EUCLIDIAN) const {
		float l = 0;

		switch (metric) {
		case Metric::EUCLIDIAN:

			l = sqrt(
					data[0] * data[0] + data[1] * data[1] + data[2] * data[2]
							+ data[3] * data[3]);

			break;
		case Metric::MAXIMUM:

			for (auto i = 0; i < 4; ++i) {
				l = fabs(data[i]) > l ? fabs(data[i]) : l;
			}

			break;
		}

		return l;
	}

	// MODIFIERS
	void set(float d0, float d1, float d2, float d3) {
		data[0] = d0;
		data[1] = d1;
		data[2] = d2;
		data[3] = d3;
	}
//	void scale(float d0, float d1, float d2, float d3) {
//		data[0] *= d0;
//		data[1] *= d1;
//		data[2] *= d2;
//		data[3] *= d3;
//	}
//	void divide(float d0, float d1, float d2, float d3) {
//		data[0] /= d0;
//		data[1] /= d1;
//		data[2] /= d2;
//		data[3] /= d3;
//	}
//	void divideByW() {
//		if (data[3] != 0) {
//			data[0] /= data[3];
//			data[1] /= data[3];
//			data[2] /= data[3];
//		} else {
//			data[0] = data[1] = data[2] = 0;
//		}
//		data[3] = 1;
//	}

// OVERLOADED OPERATORS
	Vec4f& operator=(const Vec4f &V) {
		data[0] = V.data[0];
		data[1] = V.data[1];
		data[2] = V.data[2];
		data[3] = V.data[3];
		return *this;
	}

	Vec4f operator-() {
		Vec4f temp;

		temp.data[0] = -data[0];
		temp.data[1] = -data[1];
		temp.data[2] = -data[2];
		temp.data[3] = -data[3];

		return temp;
	}

	int operator==(const Vec4f &V) const {
		return ((data[0] == V.data[0]) && (data[1] == V.data[1])
				&& (data[2] == V.data[2]) && (data[3] == V.data[3]));
	}
	int operator!=(const Vec4f &V) const {
		return ((data[0] != V.data[0]) || (data[1] != V.data[1])
				|| (data[2] != V.data[2]) || (data[3] != V.data[3]));
	}
	Vec4f& operator+=(const Vec4f &V) {
		data[0] += V.data[0];
		data[1] += V.data[1];
		data[2] += V.data[2];
		data[3] += V.data[3];
		return *this;
	}
	Vec4f& operator-=(const Vec4f &V) {
		data[0] -= V.data[0];
		data[1] -= V.data[1];
		data[2] -= V.data[2];
		data[3] -= V.data[3];
		return *this;
	}
	Vec4f& operator*=(float f) {
		data[0] *= f;
		data[1] *= f;
		data[2] *= f;
		data[3] *= f;
		return *this;
	}
	Vec4f& operator/=(float f) {
		data[0] /= f;
		data[1] /= f;
		data[2] /= f;
		data[3] /= f;
		return *this;
	}

	// OPERATIONS
	float dot2(const Vec4f &V) const {
		return data[0] * V.data[0] + data[1] * V.data[1];
	}
	float dot3(const Vec4f &V) const {
		return data[0] * V.data[0] + data[1] * V.data[1] + data[2] * V.data[2];
	}
	float dot4(const Vec4f &V) const {
		return data[0] * V.data[0] + data[1] * V.data[1] + data[2] * V.data[2]
				+ data[3] * V.data[3];
	}

	// STATIC OPERATIONS
	static void add(Vec4f &a, const Vec4f &b, const Vec4f &c) {
		a.data[0] = b.data[0] + c.data[0];
		a.data[1] = b.data[1] + c.data[1];
		a.data[2] = b.data[2] + c.data[2];
		a.data[3] = b.data[3] + c.data[3];
	}
	static void sub(Vec4f &a, const Vec4f &b, const Vec4f &c) {
		a.data[0] = b.data[0] - c.data[0];
		a.data[1] = b.data[1] - c.data[1];
		a.data[2] = b.data[2] - c.data[2];
		a.data[3] = b.data[3] - c.data[3];
	}
	static void copyScale(Vec4f &a, const Vec4f &b, float c) {
		a.data[0] = b.data[0] * c;
		a.data[1] = b.data[1] * c;
		a.data[2] = b.data[2] * c;
		a.data[3] = b.data[3] * c;
	}
	static void addScale(Vec4f &a, const Vec4f &b, const Vec4f &c, float d) {
		a.data[0] = b.data[0] + c.data[0] * d;
		a.data[1] = b.data[1] + c.data[1] * d;
		a.data[2] = b.data[2] + c.data[2] * d;
		a.data[3] = b.data[3] + c.data[3] * d;
	}
	static void average(Vec4f &a, const Vec4f &b, const Vec4f &c) {
		a.data[0] = (b.data[0] + c.data[0]) * 0.5f;
		a.data[1] = (b.data[1] + c.data[1]) * 0.5f;
		a.data[2] = (b.data[2] + c.data[2]) * 0.5f;
		a.data[3] = (b.data[3] + c.data[3]) * 0.5f;
	}
	static void weightedSum(Vec4f &a, const Vec4f &b, float c, const Vec4f &d,
			float e) {
		a.data[0] = b.data[0] * c + d.data[0] * e;
		a.data[1] = b.data[1] * c + d.data[1] * e;
		a.data[2] = b.data[2] * c + d.data[2] * e;
		a.data[3] = b.data[3] * c + d.data[3] * e;
	}
	static void cross3(Vec4f &c, const Vec4f &v1, const Vec4f &v2) {
		float x = v1.data[1] * v2.data[2] - v1.data[2] * v2.data[1];
		float y = v1.data[2] * v2.data[0] - v1.data[0] * v2.data[2];
		float z = v1.data[0] * v2.data[1] - v1.data[1] * v2.data[0];
		c.data[0] = x;
		c.data[1] = y;
		c.data[2] = z;
	}

	// INPUT / OUTPUT
	std::string str() const {
		std::stringstream ss;

		ss << "(" << data[0] << ", " << data[1] << ", " << data[2] << ", "
				<< data[3] << ")";

		return ss.str();
	}

	friend std::ostream& operator<<(std::ostream& os, const Vec4f& v) {
		return os << "(" << v.data[0] << ", " << v.data[1] << ", " << v.data[2] << ", "
				<< v.data[3] << ")";
	}

private:

	friend class Matrix;

	// REPRESENTATION
	float data[4];

};

#endif /* SRC_STRUCTURES_VECTORS_H_ */
