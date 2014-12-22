/*
 * Configure_test.cpp
 *
 *  Created on: Dec 21, 2014
 *      Author: cli
 */

#include "Configure.h"

void test_configure() {
	Configure config("name.lst");
	std::cout << config << std::endl;
}
