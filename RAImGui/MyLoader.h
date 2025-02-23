#pragma once


#include <EC.h>
#include "Version.h"
#include <unordered_map>
#include <YRPP.h>


void MyInit(InitResult& Result);

extern const std::unordered_map<std::string, FuncInfo>Funcs;