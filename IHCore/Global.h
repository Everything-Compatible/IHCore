#pragma once
#include "ExtLog.h"
#include <Windows.h>

extern LogClass DbgLog;
extern const char* DbgLogName;

extern HINSTANCE hInstDLL;
extern int RemoteComponentMaxPingMilliseconds;
extern int RemoteComponentPingInterval;
extern int RemoteComponentMaxRetry;
extern int RemoteComponentRetryInterval;