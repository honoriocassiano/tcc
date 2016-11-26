/*
 * DirectionArray.cpp
 *
 *  Created on: 19 de nov de 2016
 *      Author: cassiano
 */

#include "DirectionArray.h"

#ifndef _SRC_DIRECTIONARRAY_CPP_
#define _SRC_DIRECTIONARRAY_CPP_

template<class DType, class Element>
DirectionArray<DType, Element>::DirectionArray(const std::vector<DType>& allElements, Element defaultValue) {
	for(auto& e : allElements) {
		elements[e] = defaultValue;
	}
}

template<class DType, class Element>
DirectionArray<DType, Element>::~DirectionArray() {

}

template<class DType, class Element>
inline Element& DirectionArray<DType, Element>::getAt(const DType& position) {
	return elements[position];
}

template<class DType, class Element>
inline const Element& DirectionArray<DType, Element>::getAt(
		const DType& position) const {
	return elements[position];
}

template<class DType, class Element>
inline Element& DirectionArray<DType, Element>::operator [](
		const DType& position) {
	return elements[position];
}

template<class DType, class Element>
inline const Element& DirectionArray<DType, Element>::operator [](
		const DType& position) const {
	return elements[position];
}

#endif
