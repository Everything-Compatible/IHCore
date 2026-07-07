// Logger.h
// 作用：提供轻量日志接口，便于调试 Agent 决策链路。可重定向到文件/引擎日志。

#pragma once

#include <string>
#include <cstdarg>

namespace ihagent {

enum class LogLevel { Trace, Debug, Info, Warn, Error };

// 日志接口（可注入实现）
class ILogger {
public:
    virtual ~ILogger() = default;
    virtual void log(LogLevel level, const std::string& tag, const std::string& msg) = 0;
};

// 全局默认日志器：输出到调试输出/标准错误（占位实现）
class DefaultLogger : public ILogger {
public:
    // 输出形如 [LEVEL][tag] msg 的日志（占位实现，可替换为引擎日志）
    void log(LogLevel level, const std::string& tag, const std::string& msg) override;
};

// 全局访问的默认日志器，便于简单调用
ILogger* default_logger();

// 便捷宏
#define IH_LOG_TRACE(tag, msg) ::ihagent::default_logger()->log(::ihagent::LogLevel::Trace, tag, msg)
#define IH_LOG_DEBUG(tag, msg) ::ihagent::default_logger()->log(::ihagent::LogLevel::Debug,  tag, msg)
#define IH_LOG_INFO(tag, msg)  ::ihagent::default_logger()->log(::ihagent::LogLevel::Info,   tag, msg)
#define IH_LOG_WARN(tag, msg)  ::ihagent::default_logger()->log(::ihagent::LogLevel::Warn,  tag, msg)
#define IH_LOG_ERROR(tag, msg) ::ihagent::default_logger()->log(::ihagent::LogLevel::Error, tag, msg)

} // namespace ihagent