/*
 * CardinalDirection.h
 *
 *  Created on: 19 de nov de 2016
 *      Author: cassiano
 */

#ifndef SRC_CARDINALDIRECTION_H_
#define SRC_CARDINALDIRECTION_H_

#include "Direction.h"

class CardinalDirection: public Direction {
public:

	static const CardinalDirection N;
	static const CardinalDirection E;
	static const CardinalDirection W;
	static const CardinalDirection S;

	virtual ~CardinalDirection() {
	}

	static std::size_t getCount();

	static const CardinalDirection* getAtMatrixIndex(int index);
	static const CardinalDirection* getAtClockwiseIndex(int index);

	static const std::vector<CardinalDirection>& getAll();

private:
	CardinalDirection(int mIndex, int cIndex) :
			Direction(mIndex, cIndex) {
	}

	static std::vector<CardinalDirection> all;
};

#endif /* SRC_CARDINALDIRECTION_H_ */
