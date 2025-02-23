#pragma once
#include <string>
#include <vector>
#include <YRPP.h>

using ECString = std::basic_string<char, std::char_traits<char>, GameAllocator<char>>;

template<typename T>
using ECVector = std::vector<T, GameAllocator<T>>;