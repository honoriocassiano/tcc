/*
 * Greeting.h
 *
 *  Created on: 8 de abr de 2017
 *      Author: cassiano
 */

#ifndef SRC_GREETING_H_
#define SRC_GREETING_H_

#include <chrono>
#include <ctime>
#include <iostream>
#include <string>

// Yes, this texts are written in Brazilian Portuguese
void sayHello(unsigned int numChars = 70) {

	auto now = std::chrono::system_clock::now();
	auto tt = std::chrono::system_clock::to_time_t(now);
	auto lt = *std::localtime(&tt);
	auto hour = lt.tm_hour;

	std::string message;

	if ((hour >= 18) || (hour < 6)) {
		message = "BOA NOITE!!!!";
	} else if (hour < 12) {
		message = "BOM DIA!!!!";
	} else {
		message = "BOA TARDE!!!!";
	}

	std::cout << std::string(numChars, '*') << "\n";
	std::cout << std::string((numChars - message.size()) / 2 - 1, '*') << " "
			<< message << " "
			<< std::string((numChars + 1 - message.size()) / 2 - 1, '*')
			<< "\n";
	std::cout << std::string(numChars, '*') << "\n" << std::endl;
}

#endif /* SRC_GREETING_H_ */
