#pragma once

#include <functional>
#include <string>

namespace prev {

	enum class LogLevel {
		PV_INFO,
		PV_WARN,
		PV_ERROR,
		PV_FATAL
	};

	struct Log {
		friend class ImGuiLayer;
		static void ImGuiLog(std::string message, LogLevel errorLevel) {
			if (m_LogFunction) {
				m_LogFunction(message, errorLevel);
			}
		}
	private:
		static std::function<void(std::string, LogLevel)> m_LogFunction;
	};

}

#define PV_DEBUG_LOG(string) OutputDebugStringA(string); OutputDebugStringA("\n")
#define PV_IMGUI_LOG(string, errorLevel) prev::Log::ImGuiLog(string, errorLevel)