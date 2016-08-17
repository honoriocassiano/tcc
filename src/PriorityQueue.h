/*
 * IndexedPriorityQueue.h
 *
 *  Created on: 4 de ago de 2016
 *      Author: cassiano
 */

#ifndef SRC_PRIORITYQUEUE_H_
#define SRC_PRIORITYQUEUE_H_

#include <queue>
#include <vector>

template< class T, typename Compare>
class PriorityQueue : public std::priority_queue<T, std::vector<T>, Compare> {
public:
	PriorityQueue();
	virtual ~PriorityQueue();

	bool remove(const T& element);
};

#include "PriorityQueue.cpp"

#endif /* SRC_PRIORITYQUEUE_H_ */
