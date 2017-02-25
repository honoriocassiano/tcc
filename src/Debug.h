/*
 * Debug.h
 *
 *  Created on: 21 de jul de 2016
 *      Author: cassiano
 */

#ifndef SRC_DEBUG_H_
#define SRC_DEBUG_H_

#include "flags.h"

#include <stdio.h>
#include <string.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#if CAN_DEBUG
namespace debug {
static bool DEBUG_IS_ACTIVE = false;
}

inline void ACTIVATE_DEBUG() {
	debug::DEBUG_IS_ACTIVE = true;
}

inline void DEACTIVATE_DEBUG() {
	debug::DEBUG_IS_ACTIVE = false;
}

inline void TOGGLE_DEBUG() {
	debug::DEBUG_IS_ACTIVE = !debug::DEBUG_IS_ACTIVE;
}

#define Log(args...) \
		if(debug::DEBUG_IS_ACTIVE) {\
		printf(ANSI_COLOR_CYAN "[%s : %d] ", __FILENAME__, __LINE__); \
		printf(ANSI_COLOR_RESET args); \
		printf("\n");}

#define Error(args...) \
		if(debug::DEBUG_IS_ACTIVE) {\
		fprintf(stderr, ANSI_COLOR_RED "[%s : %d] ", __FILENAME__, __LINE__); \
		fprintf(stderr, ANSI_COLOR_RESET args); \
		fprintf(stderr, "\n");}

#else

#define Log(args...)
#define Error(args...)

#define ACTIVATE_DEBUG()
#define DEACTIVATE_DEBUG()
#define TOGGLE_DEBUG()

#endif

#endif /* SRC_DEBUG_H_ */
