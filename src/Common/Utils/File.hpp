/*
 * File.hpp
 *
 *  Created on: 10 févr. 2016
 *      Author: pmx
 */

#ifndef COMMON_UTILS_FILE_HPP_
#define COMMON_UTILS_FILE_HPP_

#include <string>

std::string getExt(std::string pathName)
{
	// Finds the last persiod character of the string
	int period = pathName.find_last_of(".");
	// I use  + 1 because I don't really need the to include the period
	std::string ext = pathName.substr(period + 1);
	return ext;
}



#endif /* COMMON_UTILS_FILE_HPP_ */
