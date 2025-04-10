#include "templater.h"
#include <iostream>

int main(int argc, char* argv[])
{
    constexpr auto full = Templater::compile::Concat<"Hello, ", "World!">::value.data();
    std::cout << full;
}