#pragma once

#include <iostream>
#include <string>

namespace Nullcore {

    class Log {
    public:
        static void Init() {
            std::cout << "[NULLCORE] Logging system initialized\n";
        }

        static void CoreTrace(const std::string& msg) {
            std::cout << "[NULLCORE] " << msg << "\n";
        }

        static void CoreInfo(const std::string& msg) {
            std::cout << "[NULLCORE] [INFO] " << msg << "\n";
        }

        static void CoreWarn(const std::string& msg) {
            std::cout << "[NULLCORE] [WARN] " << msg << "\n";
        }

        static void CoreError(const std::string& msg) {
            std::cerr << "[NULLCORE] [ERROR] " << msg << "\n";
        }

        static void Trace(const std::string& msg) {
            std::cout << "[APP] " << msg << "\n";
        }

        static void Info(const std::string& msg) {
            std::cout << "[APP] [INFO] " << msg << "\n";
        }

        static void Warn(const std::string& msg) {
            std::cout << "[APP] [WARN] " << msg << "\n";
        }

        static void Error(const std::string& msg) {
            std::cerr << "[APP] [ERROR] " << msg << "\n";
        }
    };

}

// макросы для удобства
#define NC_CORE_TRACE(msg)    ::Nullcore::Log::CoreTrace(msg)
#define NC_CORE_INFO(msg)     ::Nullcore::Log::CoreInfo(msg)
#define NC_CORE_WARN(msg)     ::Nullcore::Log::CoreWarn(msg)
#define NC_CORE_ERROR(msg)    ::Nullcore::Log::CoreError(msg)

#define NC_TRACE(msg)         ::Nullcore::Log::Trace(msg)
#define NC_INFO(msg)          ::Nullcore::Log::Info(msg)
#define NC_WARN(msg)          ::Nullcore::Log::Warn(msg)
#define NC_ERROR(msg)         ::Nullcore::Log::Error(msg)