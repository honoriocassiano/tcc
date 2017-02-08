/*
 * IntercardinalDirection.h
 *
 *  Created on: 19 de nov de 2016
 *      Author: cassiano
 */

#ifndef SRC_INTERCARDINALDIRECTION_H_
#define SRC_INTERCARDINALDIRECTION_H_

#include "Direction.h"
#include <string>

class IntercardinalDirection: public Direction {
public:

	static const IntercardinalDirection NW;
	static const IntercardinalDirection NE;
	static const IntercardinalDirection SW;
	static const IntercardinalDirection SE;

	virtual ~IntercardinalDirection() {
	}

	static const IntercardinalDirection* getAtMatrixIndex(int index);
	static const IntercardinalDirection* getAtClockwiseIndex(int index);

	static std::size_t getCount();

	static const std::vector<const IntercardinalDirection*>& all();

private:
	IntercardinalDirection(int mIndex, int cIndex);

	static std::vector<const IntercardinalDirection*> _all;
};

namespace std {
string to_string(const IntercardinalDirection& direction);
}

#endif /* SRC_INTERCARDINALDIRECTION_H_ */
