#include <LocalData.h>
#include "ECInterprocess.h"

ReturnInfoPtr __cdecl ExternalFunctionCaller(JsonObject Obj, CallerContext* Ctx)
{
	//NOTE : Ctx->Function is this function itself
	RemoteCallSendInfo SendInfo
	{
		Ctx->Target,
		Ctx->Method,
		Ctx->Version,
		Obj
	};
	auto Result = RemoteComponentManager::CallComponentMethod(std::move(SendInfo));
	ReturnInfoPtr Ret;
	Ret.Ptr = std::make_unique<RemoteCallReturnInfo>(Result);
	return Ret;
}