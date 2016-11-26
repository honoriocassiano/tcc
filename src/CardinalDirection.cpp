/*
 * CardinalDirection.cpp
 *
 *  Created on: 19 de nov de 2016
 *      Author: cassiano
 */

#include "CardinalDirection.h"

std::vector<CardinalDirection> CardinalDirection::all { N, E, W, S };

CardinalDirection CardinalDirection::N = CardinalDirection(0, 0);
CardinalDirection CardinalDirection::E = CardinalDirection(1, 1);
CardinalDirection CardinalDirection::W = CardinalDirection(2, 3);
CardinalDirection CardinalDirection::S = CardinalDirection(3, 2);

std::size_t CardinalDirection::getCount() {
	return 4;
}

const std::vector<CardinalDirection>& CardinalDirection::getAll() {
	return all;
}
