#include <LocalData.h>
#include "ECInterprocess.h"
#include "Debug.h"

void __cdecl ExternalFunctionCaller(ReturnInfoPtr& Ret, JsonObject Obj, CallerContext* Ctx)
{
	//NOTE : Ctx->Function is this function itself
	RemoteCallSendInfo SendInfo
	{
		.Source = u8"",
		.Component = Ctx->Target,
		.Method = Ctx->Method,
		.Version = Ctx->Version,
		.Context = Obj
	};

	//Debug::Log("[EC] EFC : &Ret 0x%08X, &Obj 0x%08X, &Ctx 0x%08X, Obj 0x%08X, Ctx 0x%08X\n", &Ret, &Obj, &Ctx, Obj, Ctx);
	Debug::Log("[EC] EFC : \nTarget : %s\nMethod : %s\nVersion : %d\nContext : \n%s\n",
		conv Ctx->Target.c_str(),
		conv Ctx->Method.c_str(),
		Ctx->Version,
		Obj.GetText().c_str()
	);

	auto Result = RemoteComponentManager::CallComponentMethod(std::move(SendInfo));
	Ret.Ptr = std::make_unique<RemoteCallReturnInfo>(std::move(Result));

	//Debug::Log("[EC] EFC : &Ret 0x%08X, Ret 0x%08X\n", &Ret, Ret.Ptr.get());
}