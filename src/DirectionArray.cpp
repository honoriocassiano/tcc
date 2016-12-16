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
DirectionArray<DType, Element>::DirectionArray(const std::vector<const DType*>& allElements, Element defaultValue) {

	for(auto e : allElements) {
		elements[*e] = defaultValue;
	}
}

template<class DType, class Element>
inline DirectionArray<DType, Element>::DirectionArray(
		std::initializer_list<std::pair< typename std::add_const<DType>::type, Element> > values) :
		elements(values) {

	for (auto d: DType::getAll()) {
		if ( elements.find(*d) == elements.end() ) {
			throw std::runtime_error("All members must be initialized!");
		}
	}
}

template<class DType, class Element>
DirectionArray<DType, Element>::DirectionArray(
		const std::vector<const DType*>& allElements,
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
