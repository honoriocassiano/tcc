/*
 * Scale.cpp
 *
 *  Created on: 6 de jul de 2016
 *      Author: cassiano
 */

#include "Scale.h"

namespace pssg {

float Scale::length = 1000000; /**< @hideinitializer */
//float Scale::time = 20000000; /**< @hideinitializer */
//float Scale::time = 60 * 60 * 24; /**< @hideinitializer */
float Scale::time = 0; /**< @hideinitializer */

float Scale::getLength() {
	return length;
}

void Scale::setLength(float value) {
	length = value;
}

float Scale::getTime() {
	return time;
}

void Scale::setTime(float value) {
	time = value;
}
}
