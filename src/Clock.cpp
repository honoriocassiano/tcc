/*
 * Clock.cpp
 *
 *  Created on: 15 de abr de 2016
 *      Author: cassiano
 */

#include "Clock.h"

#include <stdlib.h>
#include <stdio.h>

Clock::Clock() :
//		mPreviousTime(0), mCurrentTime(0), mIsStarted(false) {
		mIsStarted(false) {
}

Clock::~Clock() {

}

void Clock::restart() {
	if (mIsStarted) {
//		mPreviousTime = time(&mCurrentTime);
		mPrev = std::chrono::system_clock::now();
	}
}

void Clock::start() {
	if (!mIsStarted) {
//		mPreviousTime = time(nullptr);
		mPrev = std::chrono::system_clock::now();

		mIsStarted = true;
	}
}

//long int Clock::getElapsedTime() {
//	if (mIsStarted) {
//		time(&mCurrentTime);
//		mCurr = std::chrono::system_clock::now();
//
//		std::chrono::milliseconds elapsed = std::chrono::duration_cast<
//				std::chrono::milliseconds>(mCurr - mPrev);
//
//		return elapsed.count();
//	} else {
//		return 0.0d;
//	}
//}

Time Clock::getTime() {
	if (mIsStarted) {
		mCurr = std::chrono::system_clock::now();

		return Time(mCurr - mPrev);
	} else {
		return Time(std::chrono::system_clock::duration::zero());
	}
}
