/*
 * CardinalDirection.cpp
 *
 *  Created on: 19 de nov de 2016
 *      Author: cassiano
 */

#include "CardinalDirection.h"

std::vector<const CardinalDirection*> CardinalDirection::_all { &N, &E, &W, &S };

const CardinalDirection CardinalDirection::N(0, 0);
const CardinalDirection CardinalDirection::E(2, 1);
const CardinalDirection CardinalDirection::W(1, 3);
const CardinalDirection CardinalDirection::S(3, 2);

std::size_t CardinalDirection::getCount() {
	return 4;
}

const CardinalDirection* CardinalDirection::getAtMatrixIndex(int index) {
	switch (index) {
	case 0:
		return &N;
	case 1:
		return &W;

	case 2:
		return &E;

	case 3:
		return &S;

	default:
		return nullptr;
	}
}

const CardinalDirection* CardinalDirection::getAtClockwiseIndex(int index) {
	switch (index) {
	case 0:
		return &N;
	case 1:
		return &E;

	case 2:
		return &S;

	case 3:
		return &W;

	default:
		return nullptr;
	}
}

const std::vector<const CardinalDirection*>& CardinalDirection::all() {
	return _all;
}

std::string std::to_string(const CardinalDirection& direction) {

	switch (direction.getClockwiseIndex()) {
	case 0:
		return "N";
	case 1:
		return "E";
	case 2:
		return "S";
	case 3:
		return "W";
	}

	return "";
}
