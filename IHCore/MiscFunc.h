#pragma once

#include <YRPP.h>

bool __declspec(dllexport) PlayAnimOnObject(AnimTypeClass* pAnimType, ObjectClass* Object, byte Iteration);//0xFFu = Infinite Loop
bool __declspec(dllexport) PlayAnimOnObject(const char* AnimID, ObjectClass* Object, byte Iteration);//0xFFu = Infinite Loop