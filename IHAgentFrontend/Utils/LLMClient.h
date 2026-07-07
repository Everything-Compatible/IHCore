// LLMClient.h
// 作用：抽象大模型调用接口。三层 Agent 均通过 ILLMClient 发起调用，
//       区分三种调用模式（分支决策/接管模式/行为校验，见 §5.4）以及
//       战略/调度层的意图生成调用。真实实现由外部桥接（如 HTTP 调用云端 LLM）。

#pragma once

#include <string>
#include <vector>
#include <map>
#include <optional>

namespace ihagent {

// LLM 调用请求
struct LLMRequest {
    std::string scene;                               // 调用场景："strategy"/"dispatch_parse"/"dispatch_split"/"branch"/"takeover"/"verify"
    std::string system_prompt;                       // 系统提示词
    std::vector<std::string> user_messages;          // 用户消息（对话历史）
    std::map<std::string, std::string> structured;   // 结构化输入（JSON 字段）
    int max_tokens = 1024;                           // 最大输出长度
    float temperature = 0.7f;
};

// LLM 调用响应
struct LLMResponse {
    std::string text;                                // 原始文本输出
    std::map<std::string, std::string> structured;   // 解析后的结构化字段（如有）
    bool ok = false;                                  // 调用是否成功
    int latency_ms = 0;                               // 延迟
    std::string error;                               // 错误信息
};

// LLM 客户端抽象接口
class ILLMClient {
public:
    virtual ~ILLMClient() = default;
    // 同步调用（约定不阻塞主线程过久，由调用方控制时序）
    virtual LLMResponse call(const LLMRequest& req) = 0;
};

// 桩实现：永远返回空响应，便于无服务情况下编译与单元测试
class StubLLMClient : public ILLMClient {
public:
    // 返回 ok=false 的空响应
    LLMResponse call(const LLMRequest& /*req*/) override {
        LLMResponse resp;
        resp.ok = false;
        resp.error = "StubLLMClient: no real LLM backend configured";
        return resp;
    }
};

} // namespace ihagent