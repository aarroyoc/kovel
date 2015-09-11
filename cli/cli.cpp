/**
 * @file cli.cpp
 * @brief Entry point for Command Line kovel interface
 */

#include <iostream>
#include "../core/version.hpp"

int main(int argc, char** argv)
{
	std::cout << "==============================" << std::endl;
	std::cout << "KOVEL " << KOVEL_VERSION << std::endl;
	std::cout << "(C) " << KOVEL_COPYRIGHT << std::endl;
	std::cout << "Help: " << KOVEL_URL << std::endl;
	std::cout << "==============================" << std::endl;

	/* Parse command line options, show help, version and run exporter */

	return 0;
}
