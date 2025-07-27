#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <chrono>

#ifdef _WIN32
#define ENABLE_STACKTRACE 1
#include <boost/stacktrace.hpp>
#else
#define ENABLE_STACKTRACE 1
#include <execinfo.h>
#endif

enum class LogType {
    Log,
    Warning,
    Error
};

class DebugManager : public IManager {
public:
    static DebugManager& Instance() {
        static DebugManager instance;
        return instance;
    }

    void Update() override {}

    void Log(const std::string& msg) {
        Print(msg, LogType::Log);
    }

    void LogWarning(const std::string& msg) {
        Print(msg, LogType::Warning);
    }

    void LogError(const std::string& msg) {
        Print(msg, LogType::Error);
        PrintStackTrace();
    }

    bool EnableGizmos = true;

private:
    DebugManager() = default;
    ~DebugManager() = default;
    DebugManager(const DebugManager&) = delete;
    DebugManager& operator=(const DebugManager&) = delete;

    void Print(const std::string& msg, LogType type) {
        std::ostringstream oss;
        std::string prefix;
        std::string color;

        auto now = std::chrono::system_clock::now();
        std::time_t now_c = std::time(nullptr);
        std::tm local_tm;
#ifdef _WIN32
        localtime_s(&local_tm, &now_c);
#else
        localtime_r(&now_c, &local_tm);
#endif
        oss << "[" << std::put_time(&local_tm, "%H:%M:%S") << "] ";

        switch (type) {
        case LogType::Log:
            prefix = "[LOG]  ";
            color = "\033[0m"; // Default
            break;
        case LogType::Warning:
            prefix = "[WARN] ";
            color = "\033[33m"; // Yellow
            break;
        case LogType::Error:
            prefix = "[ERROR] ";
            color = "\033[31m"; // Red
            break;
        }
        oss << prefix << TimeManager::frameCount << " " << msg;
        std::cout << color << oss.str() << "\033[0m" << std::endl;
    }

    void PrintStackTrace() {
#if ENABLE_STACKTRACE
#ifdef _WIN32
        std::cout << "\033[90mCall Stack:\n" << boost::stacktrace::stacktrace() << "\033[0m";
#else
        void* callstack[128];
        int frames = backtrace(callstack, 128);
        char** symbols = backtrace_symbols(callstack, frames);

        std::cout << "\033[90mCall Stack:\n";
        for (int i = 1; i < frames && i < 10; ++i) {
            std::cout << "  [" << i << "] " << symbols[i] << "\n";
        }
        std::cout << "\033[0m";
        free(symbols);
#endif
#else
        std::cout << "\033[90m(Call stack tracing not available on this platform)\033[0m" << std::endl;
#endif
    }
};


class Debug {
public:
    static void Log(const std::string& msg) {
        DebugManager::Instance().Log(msg);
    }

    static void LogWarning(const std::string& msg) {
        DebugManager::Instance().LogWarning(msg);
    }

    static void LogError(const std::string& msg) {
        DebugManager::Instance().LogError(msg);
    }
};
