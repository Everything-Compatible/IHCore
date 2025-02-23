#include "EC.GameClass.h"
#include <unordered_set>

void Internal_DebugLog(const char* pFormat, ...);
void RaiseReadBlockError();
void RaiseExpectEndOfBlockError();
void RaiseWriteBlockError();

struct ECGameClassToken
{
	ECGameClass_* pClass;
	bool Match(const ECGameClass_* rhs)
	{
		return pClass == rhs;
	}
	struct Hasher
	{
		inline size_t operator()(const ECGameClassToken& rhs) const
		{
			return reinterpret_cast<size_t>(rhs.pClass);
		}
	};
	bool operator==(const ECGameClassToken& rhs)const
	{
		return pClass == rhs.pClass;
	}
};


std::unordered_set<ECGameClassToken, ECGameClassToken::Hasher> GameClassList;

void ECGameClass_::Register()
{
	GameClassList.insert(ECGameClassToken{ this });
}
void ECGameClass_::Unregister()
{
	GameClassList.erase(ECGameClassToken{ this });
}

ECGameClass_::ECGameClass_()
{
	Register();
}
ECGameClass_::ECGameClass_(const ECGameClass_&)
{
	Register();
}
ECGameClass_::ECGameClass_(ECGameClass_&&)
{
	Register();
}

ECGameClass_::~ECGameClass_()
{
	Unregister();
}


void ECGameClass_PointerGotInvalid(AbstractClass* const pObject, bool bRemoved)
{
	for (auto& Token : GameClassList)
		Token.pClass->PointerGotInvalid(pObject, bRemoved);
}
void ECGameClass_FinalSwizzle()
{
	for (auto& Token : GameClassList)
		Token.pClass->FinalSwizzle();
}
void ECGameClass_LoadGame(IStream* pStream)
{
	for (auto& Token : GameClassList)
	{
		ECStreamByte ByteStm(0);
		if (!ByteStm.ReadBlockFromStream(pStream))
		{
			RaiseReadBlockError();
			continue;
		}
		ECStreamReader Stm(ByteStm);

		if(Stm.RegisterChange(Token.pClass))
		{
			Token.pClass->LoadGame(Stm);
		}

		if (!Stm.ExpectEndOfBlock())
		{
			RaiseExpectEndOfBlockError();
		}
	}
}
void ECGameClass_SaveGame(IStream* pStream)
{
	for (auto& Token : GameClassList)
	{
		ECStreamByte ByteStm(Token.pClass->GetSizeMax());
		ECStreamWriter Stm(ByteStm);

		Stm.Save(Token.pClass);

		Token.pClass->SaveGame(Stm);
		if (!ByteStm.WriteBlockToStream(pStream))
		{
			RaiseWriteBlockError();
		}
	}
}