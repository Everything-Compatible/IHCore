#pragma once
#include "ECInterprocess.h"

template<typename T>
concept RemoteCommType = requires(T Comm, std::u8string& U)
{
	{ Comm.WriteString(std::u8string_view{}) } -> std::same_as<void>;
	{ Comm.ReadString() } -> std::same_as<std::u8string>;
	{ Comm.HasStringToRead() } -> std::same_as<bool>;
	{ Comm.HasStringToRead() } -> std::same_as<bool>;

	{ Comm.BaseConnect(std::u8string_view{}, std::u8string_view{},  U) } -> std::same_as<bool>;
	{ Comm.BaseDisconnect() } -> std::same_as<void>;
	{ Comm.BaseConnected() } -> std::same_as<bool>;
};

template <typename T>
class RemoteComponentBase
{
	std::unique_ptr<std::jthread> RemoteCommThread;
	InfoStack<RemoteCallRecvInfo> SyncRecvQueue;
	InfoStack<RemoteCallRecvInfo> SyncSendQueue;

	std::mutex DiscardedCallIDMutex;
	std::unordered_set<int64_t> DiscardedCallIDs;

	T* inst() { return reinterpret_cast<T*>(this); }
public:
	void RemoteCommThreadFunction(std::stop_token stok);

	void SendCallUnchecked(const RemoteCallSendInfo& Info, int64_t CallID, bool Discard);
	RemoteCallRecvInfo ReceiveCall();
	bool HasReceivedCall();
	bool Connect(std::u8string_view RegName, std::u8string_view CmdLine, std::u8string& Location);
	void Disconnect();
	void InterruptRecv(RemoteCallRecvInfo&& Recv);
};

/**
 * @brief 从一段文本中提取所有并排的无分隔符 JSON 对象
 * @param Str 输入文本
 * @return 成功解析出的 JsonFile 列表
 */
inline std::vector<JsonFile> GetJSONFromText(std::string Str)
{
	std::vector<JsonFile> result;
	cJSON_Minify(Str.data());
	auto str = Str.c_str();

	const char* cur = str;
	const char* end = str + strlen(str);

	while (cur < end)
	{
		// 跳过非 '{' 的字符
		while (*cur != '{' && cur < end) ++cur;
		if (cur >= end) break;

		// 尝试解析
		const char* parse_end = nullptr;
		cJSON* obj = cJSON_ParseWithOpts(cur, &parse_end, 0);
		if (obj)
		{
			result.push_back(obj);
			cur = parse_end;        // 跳过已解析部分
		}
		else
		{
			++cur;                  // 只前进 1 字节继续试探
		}
	}
	return result;
}



template <typename T>
void RemoteComponentBase<T>::RemoteCommThreadFunction(std::stop_token stok)
{
	while (!stok.stop_requested())
	{
		for (RemoteCallRecvInfo& Send : SyncSendQueue.Release())
		{
			auto Text = Send.GenerateCompactText();

			LogCommFlow(u8" SEND: " + Text + u8"\n");

			inst()->WriteString(Text);
		}

		if (inst()->HasStringToRead())
		{
			auto Str = inst()->ReadString();

			LogCommFlow(u8" RECV RAW: " + Str + u8"\n");

			auto fs = GetJSONFromText(~Str);
			for (JsonFile& f : fs)
			{
				LogCommFlow(u8" RECV OBJECT: \n" + ~f.GetObj().GetText());

				RemoteCallRecvInfo Recv;
				auto o = f.GetObj();
				auto oCID = o.GetObjectItem("CallID");

				if (!oCID) continue;
				if (!oCID.IsTypeString()) continue;
				auto CIDStr = oCID.GetString();
				Recv.CallID = strtoll(CIDStr.c_str(), nullptr, 10);

				{
					std::scoped_lock Lock(DiscardedCallIDMutex);
					if (DiscardedCallIDs.erase(Recv.CallID) > 0)
						continue;
				}


				auto oCom = o.GetObjectItem("Component");
				auto oSrc = o.GetObjectItem("Source");
				auto oMet = o.GetObjectItem("Method");
				auto oVer = o.GetObjectItem("Version");
				auto oCtx = o.GetObjectItem("Context");
				if (!oCom || !oSrc || !oMet || !oVer || !oCtx) continue;
				if (!oCom.IsTypeString() || !oSrc.IsTypeString() || !oMet.IsTypeString() || !oVer.IsTypeNumber()) continue;

				Recv.Component = ~oCom.GetString();
				Recv.Source = ~oSrc.GetString();
				Recv.Method = ~oMet.GetString();
				Recv.Version = oVer.GetInt();
				Recv.Context.DuplicateFromObject(oCtx, true);

				SyncRecvQueue.Push(std::move(Recv));
			}
		}

		
	}
}

template <typename T>
void RemoteComponentBase<T>::SendCallUnchecked(const RemoteCallSendInfo& Info, int64_t CallID, bool Discard)
{
	if (Discard)
	{
		std::scoped_lock Lock(DiscardedCallIDMutex);
		DiscardedCallIDs.insert(CallID);
	}

	RemoteCallRecvInfo Recv;
	Recv.CallID = CallID;
	Recv.Component = Info.Component;
	Recv.Source = Info.Source;
	Recv.Method = Info.Method;
	Recv.Version = Info.Version;
	Recv.Context.DuplicateFromObject(Info.Context, true);

	SyncSendQueue.Push(std::move(Recv));
}

template <typename T>
RemoteCallRecvInfo RemoteComponentBase<T>::ReceiveCall()
{
	IPC_Log("[EC] IPC Lock SyncRecvQueue: " __FUNCTION__ "\n");
	while (SyncRecvQueue.Empty())Sleep(0);
	return SyncRecvQueue.Pop();
}

template <typename T>
bool RemoteComponentBase<T>::HasReceivedCall()
{
	return !SyncRecvQueue.Empty();
}

template <typename T>
bool RemoteComponentBase<T>::Connect(std::u8string_view RegName, std::u8string_view CmdLine, std::u8string& Location)
{
	IPC_Log("[EC] RCB : Connecting Base \"%s\" Initial Location \"%s\"\n", conv RegName.data(), conv Location.c_str());
	if (!inst()->BaseConnect(RegName, CmdLine, Location))return false;
	IPC_Log("[EC] RCB : Complete. Location Redirected to \"%s\"\n", conv Location.c_str());

	SyncRecvQueue.Clear();
	SyncSendQueue.Clear();
	DiscardedCallIDs.clear();

	RemoteCommThread = std::make_unique<std::jthread>([this](std::stop_token stok)
		{
			RemoteCommThreadFunction(stok);
		});
	IPC_Log("[EC] RCB : Comm Thread Started\n");

	return inst()->BaseConnected();
}

template <typename T>
void RemoteComponentBase<T>::Disconnect()
{
	if (RemoteCommThread)
	{
		RemoteCommThread->get_stop_source().request_stop();
		RemoteCommThread->join();
		RemoteCommThread.reset();
	}

	inst()->BaseDisconnect();
}

template <typename T>
void RemoteComponentBase<T>::InterruptRecv(RemoteCallRecvInfo&& Recv)
{
	IPC_Log("[EC] IPC Lock SyncRecvQueue: " __FUNCTION__ "\n");
	IPC_Log("[EC] RCB : Interrupting Recv Call : \n%s\n", Recv.GenerateText().c_str());
	SyncRecvQueue.Push(std::move(Recv));;
}