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

	static IntercardinalDirection NW;
	static IntercardinalDirection NE;
	static IntercardinalDirection SW;
	static IntercardinalDirection SE;

	virtual ~IntercardinalDirection() {
	}

	static std::size_t getCount();

	static const std::vector<IntercardinalDirection>& getAll();

private:
	IntercardinalDirection(int mIndex, int cIndex);

	static std::vector<IntercardinalDirection> all;
};

#endif /* SRC_INTERCARDINALDIRECTION_H_ */
