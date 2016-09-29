/*
 * Scale.cpp
 *
 *  Created on: 6 de jul de 2016
 *      Author: cassiano
 */

#include "Scale.h"

Scale Scale::instance = Scale();

float Scale::getLength() {
	return instance.length;
}

void Scale::setLength(float value) {
	instance.length = value;
}

float Scale::getTime() {
	return instance.time;
}

void Scale::setTime(float value) {
	instance.time = value;
}

Scale::Scale() {
	length = 1000000;
	time = 20000000;
}

Scale::~Scale() {

}

