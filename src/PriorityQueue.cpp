/*
 * IndexedPriorityQueue.cpp
 *
 *  Created on: 4 de ago de 2016
 *      Author: cassiano
 */

#ifndef _SRC_PRIORITYQUEUE_H_
#define _SRC_PRIORITYQUEUE_H_

#include "PriorityQueue.h"

template< typename T, typename Compare>
PriorityQueue<T, Compare>::PriorityQueue() {

}

template< typename T, typename Compare>
PriorityQueue<T, Compare>::~PriorityQueue() {

}

template<class T, typename Compare>
bool PriorityQueue<T, Compare>::remove(const T& element) {

	bool removed = false;

	auto it = this->c.begin();

	while(it != this->c.end()) {

		if(element == *it) {
			this->c.erase(it);

			removed = true;
			break;
		}

		it++;
	}

	return removed;
}

#endif
