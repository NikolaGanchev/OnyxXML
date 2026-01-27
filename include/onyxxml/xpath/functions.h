#pragma once
#include <string>

#include "../node.h"
#include "xpath_object.h"

namespace onyx::dynamic::xpath::functions {
using XPathObject = onyx::dynamic::xpath::XPathObject;

std::string string(const XPathObject& obj);

bool boolean(const XPathObject& obj);

double number(const XPathObject& obj);
};  // namespace onyx::dynamic::xpath::functions