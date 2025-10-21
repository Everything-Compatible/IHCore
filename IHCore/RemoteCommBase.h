#pragma once
#include "ECInterprocess.h"

template<typename T>
concept RemoteCommType = requires(T Comm)
{
	{ Comm.WriteString(std::u8string_view{}) } -> std::same_as<void>;
	{ Comm.ReadString() } -> std::same_as<std::u8string>;
	{ Comm.HasStringToRead() } -> std::same_as<bool>;

	{ Comm.Connect(std::u8string_view{}, std::u8string_view{}) } -> std::same_as<bool>;
	{ Comm.Disconnect() } -> std::same_as<void>;
};

template <RemoteCommType T>
class RemoteComponentBase
{
	std::unique_ptr<std::jthread> RemoteCommThread;
	InfoStack<RemoteCallRecvInfo> SyncRecvQueue;
	InfoStack<RemoteCallRecvInfo> SyncSendQueue;

	std::mutex DiscardedCallIDMutex;
	std::unordered_set<int64_t> DiscardedCallIDs;

	T* inst() { return reinterpret_cast<T*>(this); }
public:
	static void RemoteCommThreadFunction(std::stop_token stok);

	void SendCallUnchecked(const RemoteCallSendInfo& Info, int64_t CallID, bool Discard);
	RemoteCallRecvInfo ReceiveCall();
	bool HasReceivedCall();
	bool Connect(std::u8string_view CmdLine, std::u8string_view Location);
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

template <RemoteCommType T>
void RemoteComponentBase<T>::RemoteCommThreadFunction(std::stop_token stok)
{
	while (!stok.stop_requested())
	{
		if (inst()->HasStringToRead())
		{
			auto Str = inst()->ReadString();
			auto fs = GetJSONFromText(~Str);
			for (JsonFile& f : fs)
			{
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

		for (RemoteCallRecvInfo& Send : SyncSendQueue.Release())
		{
			auto Text = Send.GenerateCompactText();
			inst()->WriteString(Text);
		}
	}
}

template <RemoteCommType T>
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

template <RemoteCommType T>
RemoteCallRecvInfo RemoteComponentBase<T>::ReceiveCall()
{
	while (SyncRecvQueue.Empty())Sleep(0);
	return SyncRecvQueue.Pop();
}

template <RemoteCommType T>
bool RemoteComponentBase<T>::HasReceivedCall()
{
	return !SyncRecvQueue.Empty();
}

template <RemoteCommType T>
bool RemoteComponentBase<T>::Connect(std::u8string_view CmdLine, std::u8string_view Location)
{
	inst()->Connect(CmdLine, Location);

	SyncRecvQueue.Clear();
	SyncSendQueue.Clear();
	DiscardedCallIDs.clear();

	RemoteCommThread = std::make_unique<std::jthread>(RemoteCommThreadFunction);
}

template <RemoteCommType T>
void RemoteComponentBase<T>::Disconnect()
{
	if (RemoteCommThread)
	{
		RemoteCommThread->get_stop_source().request_stop();
		RemoteCommThread->join();
		RemoteCommThread.reset();
	}

	inst()->Disconnect();
}

template <RemoteCommType T>
void RemoteComponentBase<T>::InterruptRecv(RemoteCallRecvInfo&& Recv)
{
	SyncRecvQueue.Push(std::move(Recv));
}