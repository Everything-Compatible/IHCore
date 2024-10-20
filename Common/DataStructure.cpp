
#include "DataStructure.h"

CStringAW::CStringAW(const StdStringAW& Std) :A(Std.A.c_str()), W(Std.W.c_str()) {}
CStringAW::operator StdStringAW()
{
	return StdStringAW(*this);
}

StdStringAW::StdStringAW(const CStringAW& CS) :A(CS.A), W(CS.W) {}
StdStringAW::operator CStringAW()
{
	return CStringAW(*this);
}

CStringAWU::CStringAWU(const StdStringAWU& Std) :A(Std.A.c_str()), W(Std.W.c_str()), U(Std.U.c_str()) {}
CStringAWU::operator StdStringAWU()
{
	return StdStringAWU(*this);
}

StdStringAWU::StdStringAWU(const CStringAWU& CS) :A(CS.A), W(CS.W), U(CS.U) {}
StdStringAWU::operator CStringAWU()
{
	return CStringAWU(*this);
}