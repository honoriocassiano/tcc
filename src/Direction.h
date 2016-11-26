/*
 * Direction.h
 *
 *  Created on: 19 de nov de 2016
 *      Author: cassiano
 */

#ifndef SRC_DIRECTION_H_
#define SRC_DIRECTION_H_

#include <cstddef>

#include <vector>

class Direction {
public:

	virtual ~Direction() {
	}

	inline int getClockwiseIndex() const {
		return cIndex;
	}
	inline int getMatrixIndex() const {
		return mIndex;
	}

	friend bool operator<(const Direction& d1, const Direction& d2) {
		return d1.getMatrixIndex() < d1.getMatrixIndex();
	}

protected:
	Direction(int mIndex, int cIndex) : mIndex(mIndex), cIndex(cIndex) {}

private:
	const int mIndex;
	const int cIndex;
};

#endif /* SRC_DIRECTION_H_ */
