/*
 * Clock.h
 *
 *  Created on: 15 de abr de 2016
 *      Author: cassiano
 */

#ifndef SRC_CLOCK_H_
#define SRC_CLOCK_H_

#include <time.h>
#include <chrono>

#include "Time.h"

class Clock {
public:
	Clock();
	virtual ~Clock();

	void start();
	void restart();

//	long int getElapsedTime();

	Time getTime();

private:
//	time_t mPreviousTime;
//	time_t mCurrentTime;

	std::chrono::system_clock::time_point mPrev;
	std::chrono::system_clock::time_point mCurr;

	bool mIsStarted;
};

#endif /* SRC_CLOCK_H_ */
