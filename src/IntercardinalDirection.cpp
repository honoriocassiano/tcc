/*
 * IntercardinalDirection.cpp
 *
 *  Created on: 19 de nov de 2016
 *      Author: cassiano
 */

#include "IntercardinalDirection.h"

#define CAST(X) static_cast<Direction>(X)

std::vector<const IntercardinalDirection*> IntercardinalDirection::all = { &NW, &NE, &SW,
		&SE };

const IntercardinalDirection IntercardinalDirection::NW =
		IntercardinalDirection(0, 0);
const IntercardinalDirection IntercardinalDirection::NE =
		IntercardinalDirection(1, 1);
const IntercardinalDirection IntercardinalDirection::SW =
		IntercardinalDirection(2, 3);
const IntercardinalDirection IntercardinalDirection::SE =
		IntercardinalDirection(3, 2);

std::size_t IntercardinalDirection::getCount() {
	return 4;
}

const std::vector<const IntercardinalDirection*>& IntercardinalDirection::getAll() {
	return all;
}

const IntercardinalDirection* IntercardinalDirection::getAtMatrixIndex(
		int index) {
	switch (index) {
	case 0:
		return &NW;
	case 1:
		return &NE;

	case 2:
		return &SW;

	case 3:
		return &SE;

	default:
		return nullptr;
	}
}

const IntercardinalDirection* IntercardinalDirection::getAtClockwiseIndex(
		int index) {
	switch (index) {
	case 0:
		return &NW;

	case 1:
		return &NE;

	case 2:
		return &SE;

	case 3:
		return &SW;

	default:
		return nullptr;
	}
}

IntercardinalDirection::IntercardinalDirection(int mIndex, int cIndex) :
		Direction(mIndex, cIndex) {

}
