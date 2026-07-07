// Logger.cpp
// 作用：Logger 的默认实现——输出到调试输出/标准错误（占位）。

#include "pch.h"
#include "Logger.h"
#include <cstdio>

namespace ihagent {

// 全局默认日志器单例
static DefaultLogger g_default_logger;
ILogger* default_logger() { return &g_default_logger; }

// 输出格式形如 [LEVEL][tag] msg
void DefaultLogger::log(LogLevel level, const std::string& tag, const std::string& msg) {
    const char* lvl = "?";
    switch (level) {
        case LogLevel::Trace: lvl = "TRACE"; break;
        case LogLevel::Debug:  lvl = "DEBUG"; break;
        case LogLevel::Info:   lvl = "INFO";  break;
        case LogLevel::Warn:   lvl = "WARN";  break;
        case LogLevel::Error:  lvl = "ERROR"; break;
    }
    // TODO: 接入引擎日志系统或写入文件；此处暂时使用标准错误
    std::fprintf(stderr, "[%s][%s] %s\n", lvl, tag.c_str(), msg.c_str());
}

} // namespace ihagent