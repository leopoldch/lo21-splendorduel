//
// Created by leo on 01/11/2023.
//

#ifndef LO21_SPLENDOR_DUEL_EXCEPTION_H
#define LO21_SPLENDOR_DUEL_EXCEPTION_H
#include <iostream>

class SplendorException {
	std::string info;

  public:
	SplendorException(const std::string &str) : info(str) {}
	std::string getInfo() const { return info; }
};

#endif // LO21_SPLENDOR_DUEL_EXCEPTION_H
