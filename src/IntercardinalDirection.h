/*
 * IntercardinalDirection.h
 *
 *  Created on: 19 de nov de 2016
 *      Author: cassiano
 */

#ifndef SRC_INTERCARDINALDIRECTION_H_
#define SRC_INTERCARDINALDIRECTION_H_

#include "Direction.h"

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

	static const std::vector<const IntercardinalDirection*>& getAll();

private:
	IntercardinalDirection(int mIndex, int cIndex);

	static std::vector<const IntercardinalDirection*> all;
};

#endif /* SRC_INTERCARDINALDIRECTION_H_ */
