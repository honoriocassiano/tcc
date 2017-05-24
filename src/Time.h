/*
 * Time.h
 *
 *  Created on: 16 de abr de 2016
 *      Author: cassiano
 */

#ifndef SRC_TIME_H_
#define SRC_TIME_H_

#include <chrono>

#include <stdio.h>

namespace pssg {

class Time {
public:
	Time(const std::chrono::system_clock::duration& duration) :
			mDuration(duration) {
	}
	~Time() {
	}

	double getAsSeconds() const {
		return std::chrono::duration_cast<std::chrono::milliseconds>(mDuration).count()
				/ 1000.0d;
	}

	long int getAsMilliseconds() const {
		return std::chrono::duration_cast<std::chrono::milliseconds>(mDuration).count();
	}

private:
	std::chrono::system_clock::duration mDuration;
};
}

#endif /* SRC_TIME_H_ */
