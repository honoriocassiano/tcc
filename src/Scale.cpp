/*
 * Scale.cpp
 *
 *  Created on: 6 de jul de 2016
 *      Author: cassiano
 */

#include "Scale.h"

Scale* Scale::m_scale = nullptr;

float Scale::getLength() {
	if(!m_scale) {
		m_scale = new Scale();
	}

	return m_scale->m_length;
}

void Scale::setLength(float value) {
	if(!m_scale) {
		m_scale = new Scale();
	}

	m_scale->m_length = value;
}

float Scale::getTime() {
	if(!m_scale) {
		m_scale = new Scale();
	}

	return m_scale->m_time;
}

void Scale::setTime(float value) {
	if(!m_scale) {
		m_scale = new Scale();
	}

	m_scale->m_time = value;
}

Scale::Scale() {
	m_length = 1000000;
	m_time = 200;
}

Scale::~Scale() {
	m_scale = nullptr;
}

