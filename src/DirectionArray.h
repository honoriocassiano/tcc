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

template<class DType, class Element>
class DirectionArray {
public:
	DirectionArray(const std::vector<DType>& allElements, Element defaultValue);
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
