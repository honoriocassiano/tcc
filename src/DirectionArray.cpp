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

//template<class DType, class Element>
//DirectionArray<DType, Element>::DirectionArray(
//		const std::vector<DType>& allElements, Element values...) {
//
//	auto params = std::make_tuple(values);
//	auto size = std::tuple_size<decltype(params)>::value;
//
//	assert((size == 1 && size == allElements.size()) && "Invalid number of arguments");
//
//	if( size == 1 ) {
//		for(auto i = 0; i < allElements.size(); ++i) {
//			elements[allElements[i]] = std::get<0>(params);
//		}
//	} else {
//		for(int i = 0; i < allElements.size(); ++i) {
//			//elements[allElements[i]] = std::get<i>(params);
//			elements[allElements[i]] = params[i];
//		}
//	}
//}

template<class DType, class Element>
DirectionArray<DType, Element>::DirectionArray(
		const std::vector<DType>& allElements,
		const std::vector<Element>& values) {

	assert((values.size() == allElements.size()) &&"Invalid number of arguments");

	for(auto i = 0; i < allElements.size(); ++i) {
		elements[allElements[i]] = values[i];
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
	return elements.at(position);
}

#endif
