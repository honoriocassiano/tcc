/*
 * DirectionArray.h
 *
 *  Created on: 19 de nov de 2016
 *      Author: cassiano
 */

#ifndef SRC_DIRECTIONARRAY_H_
#define SRC_DIRECTIONARRAY_H_

#include <vector>
#include <map>
#include <tuple>
#include <utility>
#include <exception>
#include <type_traits>
#include <initializer_list>

template<class DType, class Element>
class DirectionArray {
public:

	// todo Create an constructor that receive all elements and values
	DirectionArray(const std::vector<const DType*>& allElements,
			Element defaultValue);

	DirectionArray(const std::vector<const DType*>& allElements,
			const std::vector<Element>& values);

	DirectionArray(
			std::initializer_list<
					std::pair< typename std::add_const<DType>::type, Element> > values);

	virtual ~DirectionArray();

	Element& getAt(const DType& position);
	const Element& getAt(const DType& position) const;

	Element& operator[](const DType& position);
	const Element& operator[](const DType& position) const;

private:
	std::map<DType, Element> elements;
};

#include "DirectionArray.cpp"

#endif /* SRC_DIRECTIONARRAY_H_ */
