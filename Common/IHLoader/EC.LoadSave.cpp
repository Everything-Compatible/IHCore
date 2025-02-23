#include "EC.LoadSave.h"
#include "EC.LoadSaveImpl.h"
#include "IH.Initial.h"
#include "IH.InitialService.h"
#include "EC.Misc.h"

//------------------------------------------------------------------------------------------------------------------------------------

void CALLBACK ISF_PointerGotInvalid(AbstractClass* const pObject, bool bRemoved);
void CALLBACK ISF_LoadGame(IStream* pStream);
void CALLBACK ISF_FinalSwizzle(IStream* pStream, SIFinalSwizzleFn SIFinalSwizzle);
void CALLBACK ISF_SaveGame(IStream* pStream);


LoadSaveInterface LocalLSInterface
{
	ISF_LoadGame,
	ISF_FinalSwizzle,
	ISF_SaveGame,
	ISF_PointerGotInvalid,
};

void RegisterLoadSaveInterface()
{
	InitialLoad::CreateRequestAndSubmit<InitialLoadParam_RegisterObject<LoadSaveInterface>>(
		"EC::Internal::LoadSave", Internal_GetLibRegName(), LocalLSInterface);
}



//------------------------------------------------------------------------------------------------------------------------------------



bool EnableStreamLoadCheck{ true };
void StreamReaderLoadCheck(bool bCheck)
{
	EnableStreamLoadCheck = bCheck;
}
bool StreamReaderLoadCheck()
{
	return EnableStreamLoadCheck;
}

bool EnableStreamSaveCheck{ true };
void StreamWriterSaveCheck(bool bCheck)
{
	EnableStreamSaveCheck = bCheck;
}
bool StreamWriterSaveCheck()
{
	return EnableStreamSaveCheck;
}



bool ECStreamReader::ExpectEndOfBlock() const
{
	if (!this->Success() || this->stream->Size() != this->stream->Offset())
	{
		if (EnableStreamLoadCheck) this->EmitExpectEndOfBlockWarning();
		return false;
	}
	return true;
}
bool ECStreamReader::Read(ECStreamByte::data_t* Value, size_t Size)
{
	if (!this->stream->Read(Value, Size))
	{
		if(EnableStreamLoadCheck) this->EmitLoadWarning(Size);
		this->success = false;
		return false;
	}
	return true;
}
bool ECStreamReader::Expect(unsigned int value)
{
	unsigned int buffer = 0;
	if (this->Load(buffer))
	{
		if (buffer == value)
		{
			return true;
		}
		if (EnableStreamLoadCheck)this->EmitExpectWarning(buffer, value);
	}
	return false;
}
bool ECStreamReader::RegisterChange(void* pNew)
{
	static_assert(sizeof(long) == sizeof(void*), "EC : long and void* need to be of same size.");
	long pOld = 0;
	if (this->Load(pOld))
	{
		if (SUCCEEDED(SwizzleManagerClass::Instance().Here_I_Am(pOld, pNew)))
		{
			return true;
		}
		if (EnableStreamLoadCheck)this->EmitSwizzleWarning(pOld, pNew);
	}
	return false;
}

void ECStreamReader::EmitExpectEndOfBlockWarning() const
{
	Internal_DebugLog("[EC] Read %X bytes instead of %X!\n", this->stream->Offset(), this->stream->Size());
	auto Handler = GetStreamReadErrorHandler();
	if (Handler)Handler(ECStreamErrorType::EmitExpectEndOfBlockWarning);
}

void ECStreamReader::EmitLoadWarning(size_t size) const
{
	Internal_DebugLog("[EC] Could not read data of length %u at %X of %X.\n", size, this->stream->Offset() - size, this->stream->Size());
	auto Handler = GetStreamReadErrorHandler();
	if (Handler)Handler(ECStreamErrorType::EmitLoadWarning);
}

void ECStreamReader::EmitExpectWarning(unsigned int found, unsigned int expect) const
{
	Internal_DebugLog("[EC] Found %X, expected %X\n", found, expect);
	auto Handler = GetStreamReadErrorHandler();
	if (Handler)Handler(ECStreamErrorType::EmitExpectWarning);
}

void ECStreamReader::EmitSwizzleWarning(long id, void* pointer) const
{
	Internal_DebugLog("[EC] Could not register change from %X to %p\n", id, pointer);
	auto Handler = GetStreamReadErrorHandler();
	if (Handler)Handler(ECStreamErrorType::EmitSwizzleWarning);
}


//------------------------------------------------------------------------------------------------------------------------------------


ECStreamErrorHandler ECRS_ErrorHandler{ nullptr };
ECStreamErrorHandler ECWS_ErrorHandler{ nullptr };

void SetStreamReadErrorHandler(ECStreamErrorHandler Handler)
{
	ECRS_ErrorHandler = Handler;
}
void SetStreamWriteErrorHandler(ECStreamErrorHandler Handler)
{
	ECWS_ErrorHandler = Handler;
}
ECStreamErrorHandler GetStreamReadErrorHandler()
{
	return ECRS_ErrorHandler;
}
ECStreamErrorHandler GetStreamWriteErrorHandler()
{
	return ECWS_ErrorHandler;
}



//------------------------------------------------------------------------------------------------------------------------------------

void RaiseReadBlockError()
{
	if (EnableStreamLoadCheck)
	{
		auto Handler = GetStreamReadErrorHandler();
		if (Handler)Handler(ECStreamErrorType::ReadBlockError);
	}
}

void RaiseWriteBlockError()
{
	if (EnableStreamSaveCheck)
	{
		auto Handler = GetStreamWriteErrorHandler();
		if (Handler)Handler(ECStreamErrorType::WriteBlockError);
	}
}


void RaiseExpectEndOfBlockError()
{
	if (EnableStreamLoadCheck)
	{
		auto Handler = GetStreamReadErrorHandler();
		if (Handler)Handler(ECStreamErrorType::EmitExpectEndOfBlockWarning);
	}
}

void RaiseEmitLoadWarning()
{
	if (EnableStreamLoadCheck)
	{
		auto Handler = GetStreamReadErrorHandler();
		if (Handler)Handler(ECStreamErrorType::EmitLoadWarning);
	}
}

SIFinalSwizzleFn SIFinalSwizzleFunc;
bool SIFinalSwizzleImpl(void* pOld, void*& pNew)
{
	SIFinalSwizzleFunc(pOld, pNew);
}


//bRemoved : true=真寄了 false=临时离场
void CALLBACK ISF_PointerGotInvalid(AbstractClass* const pObject, bool bRemoved)
{
	ECGameClass_PointerGotInvalid(pObject, bRemoved);
}

void CALLBACK ISF_FinalSwizzle(IStream* pStream, SIFinalSwizzleFn SIFinalSwizzle)
{
	if (!SIFinalSwizzle)
	{
		Internal_DebugLog("[EC] SIFinalSwizzle == nullptr\n");
		Internal_DebugLog("[EC] Failed to trigger FinalSwizzle!!\n");
		return;
	}
	SIFinalSwizzleFunc = SIFinalSwizzle;
	ECGameClass_FinalSwizzle();
}

void CALLBACK ISF_LoadGame(IStream* pStream)
{
	ECStreamByte ReadStm(0);

	//auto Version = Internal_GetLibVersion();
	//Version Check

	ECGameClass_LoadGame(pStream);
}

void CALLBACK ISF_SaveGame(IStream* pStream)
{
	ECStreamByte WriteStm;

	//auto Version = Internal_GetLibVersion();
	//Version Check

	ECGameClass_SaveGame(pStream);
}

#if 0
/*
	ULONG out = 0;
	UINT X = 0;
	pStream->Read(&X, sizeof(X), &out);
	sprintf_s(bb, "读出 %d, 应 %u 字节, 实 %u 字节", X, sizeof(X), out);
	MessageBoxA(Game::hWnd, bb, "万物互通", MB_OK);
*/
char bb[10000];
TechnoClass* test__;
#include "EC.Listener.h"
void CALLBACK AFG()
{
	if (test__)sprintf_s(bb, "读出  0x%08p(%s)", test__, UTF16ToGBK(test__->GetUIName()).c_str());
	else sprintf_s(bb, "WTF");
	MessageBoxA(Game::hWnd, bb, "万物互通", MB_OK);
}
void CALLBACK ISF_LoadGame(IStream* pStream)
{
	ULONG out = 0;
	pStream->Read(&test__, sizeof(test__), &out);
	SwizzleManagerClass::Instance().Swizzle((void**)&test__);
	//ECListener::Listen_AfterLoadGame(AFG);
}

void CALLBACK ISF_SaveGame(IStream* pStream)
{
	AFG();
	ULONG out = 0;
	test__ = TechnoClass::Array()->Items[0];
	pStream->Write(&test__, sizeof(test__), &out);
	sprintf_s(bb, "写入 0x%08p(%s)", test__, UTF16ToGBK(test__->GetUIName()).c_str());
	MessageBoxA(Game::hWnd, bb, "万物互通", MB_OK);
}




#include "EC.ObjBase.h"

ECRTTI_ExportRTTI(TestType1)
ECObjType TestType1 : EC_USERTTI, EC_USEUNIQUEID
{
	ECRTTI_DefineRTTIFunction(TestType1)
};


ECRTTI_ExportRTTI(TestType2)
ECObjType TestType2 : EC_USERTTI, EC_USEUNIQUEID
{
	ECRTTI_DefineRTTIFunction(TestType2)
};

ECRTTI_ExportRTTI(TestType4, TestType2)
ECObjType TestType4 : public TestType2
{
	ECRTTI_DefineRTTIFunction(TestType4)
};

ECRTTI_ExportRTTI(TestType4, TestType2)
ECObjType TestType4 : public TestType2
{
	ECRTTI_DefineRTTIFunction(TestType4)
};

ECRTTI_ExportRTTI(TestType3, TestType1)
ECObjType TestType3 : public TestType1
{
	ECRTTI_DefineRTTIFunction(TestType3)
	void Test()
	{
		char x[10000];
		auto V = WhatAmI();
		sprintf_s(x, "ID : %d\nType : { %s : 0x%08X }\nBase { %s : 0x%08X }\nthis = %p -> TestType1: %p TestType2: %p",
			GetUniqueID(), V->GetName(), V, V->GetFirstBaseClassName(),
			V->GetFirstBaseClassInfo(), this, DynamicCast<TestType1>(), DynamicCast<TestType2>());
		MessageBoxA(Game::hWnd, x, "万物互通", MB_OK);
	}
};

#endif

