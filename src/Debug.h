/*
 * Debug.h
 *
 *  Created on: 21 de jul de 2016
 *      Author: cassiano
 */

#ifndef SRC_DEBUG_H_
#define SRC_DEBUG_H_

#define DEBUG 1

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

#if DEBUG
#define Log(args...) \
		printf(ANSI_COLOR_CYAN "[%s : %d] ", __FILENAME__, __LINE__); \
		printf(ANSI_COLOR_RESET args); \
		printf("\n");

#define Error(args...) \
		fprintf(stderr, ANSI_COLOR_RED "[%s : %d] ", __FILENAME__, __LINE__); \
		fprintf(stderr, ANSI_COLOR_RESET args); \
		fprintf(stderr, "\n");

#else
	#define Log(args...)
	#define Error(args...)

#endif

#endif /* SRC_DEBUG_H_ */
