/*
 * IntercardinalDirection.cpp
 *
 *  Created on: 19 de nov de 2016
 *      Author: cassiano
 */

#include "IntercardinalDirection.h"

#define CAST(X) static_cast<Direction>(X)

std::vector<IntercardinalDirection> IntercardinalDirection::all{ NW, NE, SW, SE };

IntercardinalDirection IntercardinalDirection::NW = IntercardinalDirection(0,
		0);
IntercardinalDirection IntercardinalDirection::NE = IntercardinalDirection(1,
		1);
IntercardinalDirection IntercardinalDirection::SW = IntercardinalDirection(2,
		3);
IntercardinalDirection IntercardinalDirection::SE = IntercardinalDirection(3,
		2);

std::size_t IntercardinalDirection::getCount() {
	return 4;
}

const std::vector<IntercardinalDirection>& IntercardinalDirection::getAll() {
	return all;
}

IntercardinalDirection::IntercardinalDirection(int mIndex, int cIndex) : Direction(mIndex, cIndex) {

}
