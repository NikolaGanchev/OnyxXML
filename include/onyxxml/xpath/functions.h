#pragma once
#include <string>

#include "../node.h"
#include "xpath_object.h"

namespace onyx::dynamic::xpath::functions {
using XPathObject = onyx::dynamic::xpath::XPathObject;

std::string string(const XPathObject& obj);

bool boolean(const XPathObject& obj);

double number(const XPathObject& obj);

/**
 * @brief https://www.w3.org/TR/1999/REC-xpath-19991116/#function-round
 *
 * @param num
 * @return double
 */
double round(double num);

std::string substring(const std::string& str, double start,
                      double length = std::numeric_limits<double>::infinity());

std::string stringBefore(const std::string& str1, const std::string& str2);

std::string stringAfter(const std::string& str1, const std::string& str2);

/**
 * @brief Currently does not handle UTF-8
 * 
 * @param str1 
 * @param str2 
 * @param str3 
 * @return std::string 
 */
std::string translate(const std::string& str1, const std::string& str2, const std::string& str3);

std::string normalizeSpace(const std::string& str);
};  // namespace onyx::dynamic::xpath::functions