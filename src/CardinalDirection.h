/*
 * CardinalDirection.h
 *
 *  Created on: 19 de nov de 2016
 *      Author: cassiano
 */

#ifndef SRC_CARDINALDIRECTION_H_
#define SRC_CARDINALDIRECTION_H_

#include "Direction.h"
#include <string>

class CardinalDirection: public Direction {
public:

	CardinalDirection(CardinalDirection&&) = delete;
	CardinalDirection(const CardinalDirection&) = delete;
	CardinalDirection& operator=(const CardinalDirection&) = delete;

	static const CardinalDirection N;
	static const CardinalDirection E;
	static const CardinalDirection W;
	static const CardinalDirection S;

	virtual ~CardinalDirection() {
	}

	static std::size_t getCount();

	static const CardinalDirection* getAtMatrixIndex(int index);
	static const CardinalDirection* getAtClockwiseIndex(int index);

	static const std::vector<const CardinalDirection*>& all();

private:
	CardinalDirection(int mIndex, int cIndex) :
			Direction(mIndex, cIndex) {
	}

	static std::vector<const CardinalDirection*> _all;
};

namespace std {
string to_string(const CardinalDirection& direction);
}

#endif /* SRC_CARDINALDIRECTION_H_ */
