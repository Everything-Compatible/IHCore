#pragma once
#include "RemoteCommBase.h"
#include "RemoteComponents.h"



struct RemoteComm_NamedPipe : public RemoteComponentBase<RemoteComm_NamedPipe>
{
	ServiceProcessManager Proc;
	HANDLE pipe_handle_ = INVALID_HANDLE_VALUE;
	std::vector<char8_t> read_buffer_;
	static constexpr size_t BUFFER_SIZE = 4096;
	bool Connected;

	bool BaseConnected() const { return Connected; }
	bool BaseConnect(std::u8string_view RegName, std::u8string_view CmdLine, std::u8string& Location);
	void BaseDisconnect();

	std::u8string ReadString();
	void WriteString(std::u8string_view Str);
	bool HasStringToRead();
};

struct RemoteComm_TCP : public RemoteComponentBase<RemoteComm_TCP>
{
	ServiceProcessManager Proc;
	SOCKET socket_ = INVALID_SOCKET;
	std::vector<char8_t> read_buffer_;
	static constexpr size_t BUFFER_SIZE = 4096;
	bool Connected;

	bool ParseAddress(std::u8string_view Location, std::string& host, int& port);
	bool ConnectToServer(const std::string& host, int port);

	bool BaseConnected() const { return Connected; }
	bool BaseConnect(std::u8string_view RegName, std::u8string_view CmdLine, std::u8string& Location);
	void BaseDisconnect();

	std::u8string ReadString();
	void WriteString(std::u8string_view Str);
	bool HasStringToRead();
};

using RemoteComponent_NamedPipe = RemoteComponent_CommType<RemoteComm_NamedPipe>;
using RemoteComponent_TCP = RemoteComponent_CommType<RemoteComm_TCP>;