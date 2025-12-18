#include "ExtFNT.h"
#include "Patch.h"
#include "ToolFunc.h"
#include "Debug.h"


//acutally timeGetTime() >> 4;
DWORD sub_6C8C40()
{
	JMP_STD(0x6C8C40);
}

struct MessageListClassImpl
{
    inline static std::wstring MessageBufferEx[14] {};

    TextLabelClass* AddMessage(const wchar_t* Name, int ID, const wchar_t* Message, int ColorSchemeIdx, TextPrintType Style, int Timeout, bool SinglePlayer)
    {
#ifdef CalcEx_UnitTest
		Debug::Log("[IH] MessageListClass::AddMessage Invoked\n");
        Debug::Log("[IH] ORIGINAL\n%s\n", UnicodetoUTF8(Message).c_str());
        Debug::Log("[IH] ORIGINAL PARSED\n%s\n", UnicodetoUTF8(ParseStringForTest(Message)).c_str());
#endif
        auto This = reinterpret_cast<MessageListClass*>(this);
        auto pFont = BitFont::Instance();
		if (!Message)
			return nullptr;
		if (!pFont)
			return nullptr;
		std::wstring String;
		if (Name)
		{
			String += Name;
			String += L":";
		}

		size_t v11 = String.length();
		auto width1 = String.empty() ? 0 : pFont->GetTextDimension_B(String.c_str(), 0);
		size_t v34 = wcslen(Message);

		const int WidthDelta = 24;//Original is 8 , but it seems not enough in some situation

		if (This->Width - width1 - WidthDelta <= 0)
            return nullptr;
        int MaxLen;
        const wchar_t *StrBefore, *StrAfter;

		AutoWrapTextEx(
			pFont,
			Message,
			This->Width - width1 - WidthDelta,
			111,
			true,
            MaxLen,
            StrBefore,
            StrAfter
		);

#ifdef CalcEx_UnitTest
        Debug::Log("[IH] Width %d ,Width Limit %d\n", This->Width, This->Width - width1 - WidthDelta);
        Debug::Log("[IH] BEFORE\n%s\n", UnicodetoUTF8(StrBefore).c_str());
        Debug::Log("[IH] BEFORE PARSED\n%s\n", UnicodetoUTF8(ParseStringForTest(StrBefore)).c_str());
#endif

        if(!MaxLen)
            return nullptr;

		size_t v14 = MaxLen;
		size_t _MaxLen = MaxLen;

		String.append(StrBefore);

        if (StrAfter)
        {
			GameDeleteArray(StrBefore, MaxLen);
        }

		int v15 = This->MaxMessageCount;

        if (v15 > 0)
        {
			GadgetClass* v16 = This->MessageList;
            int i;
            for (i = 0; v16; v16 = v16->GetNext())
                ++i;
            if (This->IsEdit && This->AdjustEdit)
                ++i;
            if (i + 1 > This->MaxMessageCount)
            {
                if (!This->MessageList)
                    return nullptr;
                auto v18 = This->MessageList;
				This->MessageList = (TextLabelClass*)v18->Remove();
                auto v19 = This->BufferAvail;
                auto v20 = MessageBufferEx;
                auto v21 = 14;
                do
                {
                    if (v18->Text == v20->c_str())
                        *v19 = 1;
                    ++v19;
                    ++v20;
                    --v21;
                } while (v21);
                GameDelete(v18);
            }
        }
 
        auto v23 = Style;
        //BYTE1(v23) = BYTE1(Style) | 0x80;
        v23 |= TextPrintType::UnknownColor;// = 0x8000
        auto NewTextLabel = GameCreate<TextLabelClass>(
            String.data(),
            This->MessagePos.X,
            This->MessagePos.Y,
            ColorSchemeIdx,
            v23
        );

        if(!NewTextLabel)
            return nullptr;

        if (SessionClass::Instance().GameMode == GameMode::Internet)
        {
			if (!SinglePlayer && SessionClass::Instance().unknown_8AA[5911])
			{
				NewTextLabel->Animate = true;
			}
			else
			{
				NewTextLabel->Animate = false;
			}
		}
		else if (SessionClass::Instance().GameMode == GameMode::LAN)
		{
			if (SinglePlayer || !SessionClass::Instance().unknown_8AA[5910])
			{
				NewTextLabel->Animate = false;
			}
			else
			{
				NewTextLabel->Animate = true;
			}
		}
        else
        {
            NewTextLabel->Animate = !SinglePlayer;
        }

        if (Timeout == -1)
        {
			NewTextLabel->UserData1 = 0;
		}
        else
        {
			DWORD unk_887340 = *reinterpret_cast<DWORD*>(0x887340);
			DWORD dword_887338 = *reinterpret_cast<DWORD*>(0x887338);
			int v25 = unk_887340;
			if (dword_887338 != -1)
				v25 += sub_6C8C40() - dword_887338;
			NewTextLabel->UserData1 = (void*)(Timeout + v25);
        }

		NewTextLabel->UserData2 = (void*)ID;
		NewTextLabel->PixWidth = This->Width;
		int v26 = 0;
		for (wchar_t* j = This->BufferAvail; !*j; ++j)
		{
			if (++v26 >= 14)
			{
				GameDelete(NewTextLabel);
                return nullptr;
			}
		}
		This->BufferAvail[v26] = 0;
		auto v28 = This->MessageBuffers[v26];
		memset(v28, 0, 160u);
		This->MessageBuffers[v26][80] = 0;
		wcsncpy(v28, String.c_str(), 80);
        MessageBufferEx[v26] = String;
		NewTextLabel->Text = MessageBufferEx[v26].c_str();
#ifdef CalcEx_UnitTest
        Debug::Log("!!!!ADDED %s\n", UnicodetoUTF8(ParseStringForTest(NewTextLabel->Text)).c_str());
		Debug::Log("[IH] Chosen BufferIdx = %d\n", v26);
#endif

        if (!SinglePlayer)
            VocClass::PlayGlobal(
                RulesClass::Instance()->IncomingMessage,
                0x2000u,
                1.0,
                nullptr
            );
#ifdef CalcEx_UnitTest
        const auto PrintNodeRelation = [](const char* pre, LinkClass* list) {
			Debug::Log("%s", pre);
			for (auto node = list; node; node = node->GetNext())
			{
				Debug::Log("%08X -> ", node);
			}
			Debug::Log("NULL\n");
        };
#endif

        if (This->MessageList)
        {
            NewTextLabel->AddTail(*This->MessageList);
        }
		else
			This->MessageList = NewTextLabel;
#ifdef CalcEx_UnitTest
        int FinalLen = 0;
        for (GadgetClass* start = This->MessageList; start; start = start->GetNext())
        {
            FinalLen++;
        }
        Debug::Log("[IH] Final List Len = %d\n", FinalLen);
#endif

		int v29;
		if (This->IsEdit && This->AdjustEdit)
			v29 = This->MessagePos.Y + This->Height;
		else
			v29 = This->MessagePos.Y;
		for (auto k = This->MessageList; k; v29 += This->Height)
		{
			k->Y = v29;
			k = (TextLabelClass*)k->GetNext();
		}

        if (StrAfter)
        {
            AddMessage(Name, ID, StrAfter, ColorSchemeIdx, Style, Timeout, true);
            GameDeleteArray(StrAfter, wcslen(StrAfter));
        }

		return NewTextLabel;
  
  /*
  if ( !SinglePlayer )
    VocClass::PlayGlobal(RulesClass_Instance->IncomingMessage, 0x2000u, 1.0, 0);
  if ( this->MessageList )
    NewTextLabel->AddTail(NewTextLabel, this->MessageList);
  else
    this->MessageList = NewTextLabel;
  if ( this->IsEdit && this->AdjustEdit )
    v29 = this->MessagePos.Y + this->Height;
  else
    v29 = this->MessagePos.Y;
  for ( k = this->MessageList; k; v29 += this->Height )
  {
    v31 = k->__vftable;
    k->Y = v29;
    k = v31->GetNext(k);
  }
  if ( _MaxLen < v34 )
  {
    v32 = &Message[_MaxLen];
    for ( l = *v32; l; ++v32 )
    {
      if ( l >= 0x20u )
        break;
      l = v32[1];
    }
    if ( *v32 )
      MessageListClass::AddMessage(this, Name, ID, v32, ColorSchemeIdx, Style, Timeout, 1);
  }
  return NewTextLabel;
        */
    }

/*
UnitTest

ORIGINAL
#{Key.DeployObject.Name} : #{CSF.Longer}yajusenpai#{Key.DeployObject.Name}
ORIGINAL PARSED
部署物件 : 本人也是经过了深思熟虑，在每个日日夜夜思考这个问题。从这个角度来看，这是不可避免的。 每个人都不得不面对这些问题。在面对这种问题时，这种事实对本人来说意义重大，相信对这个世界也是有一定意义的。了解清楚航味麻酱比普通麻酱好吃到底是一种怎么样的存在，是解决一切问题的关键。从这个角度来看，这种事实对本人来说意义重大，相信对这个世界也是有一定意义的。在这种困难的抉择下，本人思来想去，寝食难安。这种事实对本人来说意义重大，相信对这个世界也是有一定意义的。我们一般认为，抓住了问题的关键，其他一切则会迎刃而解。现在，解决航味麻酱比普通麻酱好吃的问题，是非常非常重要的。 所以，带着这些问题，我们来审视一下航味麻酱比普通麻酱好吃。 我们不得不面对一个非常尴尬的事实，那就是，我们都知道，只要有意义，那么就必须慎重考虑。本人也是经过了深思熟虑，在每个日日夜夜思考这个问题。 yajusenpai部署物件
PARSED
部署物件 : 本人也是经过了深思熟虑，在每个日日夜夜思考这个问题。从这个角度来看，这是不可避免的。 每个人都不得不面对这些问题。在面对这种问题时，这种事实对本人来说意义重大，相信对这个世界也是有
一定意义的。了解清楚航味麻酱比普通麻酱好吃到底是一种怎么样的存在，是解决一切问题的关键。从这个角度来看，这种事实对本人来说意义重大，相信对这个世界也是有一定意义的。在这种困难的抉择下，本人
思来想去，寝食难安。这种事实对本人来说意义重大，相信对这个世界也是有一定意义的。我们一般认为，抓住了问题的关键，其他一切则会迎刃而解。现在，解决航味麻酱比普通麻酱好吃的问题，是非常非常重要的。
所以，带着这些问题，我们来审视一下航味麻酱比普通麻酱好吃。 我们不得不面对一个非常尴尬的事实，那就是，我们都知道，只要有意义，那么就必须慎重考虑。本人也是经过了深思熟虑，在每个日日夜夜思考这个问
题。 yajusenpai部署物件

*/
};


void ExtMsgList_InitBeforeEverything()
{
    Patch::Apply_LJMP(0x5D3BA0, union_cast<void*>(&MessageListClassImpl::AddMessage));
};
