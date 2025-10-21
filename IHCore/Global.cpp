#include "Global.h"

const char* DbgLogName = "Game.log";
LogClass DbgLog{ DbgLogName };

HINSTANCE hInstDLL;
int RemoteComponentMaxPingMilliseconds = 2000;
int RemoteComponentPingInterval = 10000;

