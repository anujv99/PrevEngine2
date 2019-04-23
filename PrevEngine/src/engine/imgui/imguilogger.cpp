#include "pch.h"
#include "imguilogger.h"

#include <imgui.h>


struct ImGuiAppLog {
private:
	struct MessageLog {
		std::string m_Message;
		prev::LogLevel m_Level;
	};

public:
	std::vector<MessageLog> Messages;
	ImGuiTextFilter			Filter;
	bool					ScrollToBottom, check;

	void Clear() {
		Messages.clear();
	}

	void AddLog(prev::LogLevel level, std::string message) {
		Messages.push_back({ message, level });
		check = true;
	}

	void Draw(const char * title, std::map<prev::LogLevel, ImVec4> & colors, bool * p_open = NULL) {
		ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
		if (!ImGui::Begin(title, p_open)) {
			ImGui::End();
			return;
		}
		if (ImGui::Button("Clear")) Clear();
		ImGui::SameLine();
		bool copy = ImGui::Button("Copy");
		ImGui::SameLine();
		Filter.Draw("Filter", -100.0f);
		ImGui::NewLine();
		ImGui::Checkbox("Auto Scroll", &ScrollToBottom);
		ImGui::SameLine();
		static bool showFPSinLog = prev::Timer::IsLoggingFPSCounter();
		if (ImGui::RadioButton("Show FPS in log", showFPSinLog)) {
			showFPSinLog = !showFPSinLog;
			prev::Timer::FPSCounter(showFPSinLog);
		}
		ImGui::Separator();
		ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
		if (copy)
			ImGui::LogToClipboard();

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

		if (Filter.IsActive()) {
			for (auto & message : Messages) {
				if (Filter.PassFilter(message.m_Message.c_str())) {
					ImGui::PushStyleColor(ImGuiCol_Text, colors[message.m_Level]);
					ImGui::TextUnformatted(message.m_Message.c_str());
					ImGui::PopStyleColor();
				}
			}
		} else {
			for (auto & message : Messages) {
				ImGui::PushStyleColor(ImGuiCol_Text, colors[message.m_Level]);
				ImGui::TextUnformatted(message.m_Message.c_str());
				ImGui::PopStyleColor();
			}
		}

		ImGui::PopStyleVar();

		if (ScrollToBottom && check) {
			ImGui::SetScrollHereY(1.0f);
			check = false;
		}
		ImGui::EndChild();
		ImGui::End();
	}
};

namespace prev {

	std::function<void(std::string, LogLevel)> Log::m_LogFunction;
	static bool is_logging = true;
	static std::map<LogLevel, ImVec4> m_LogColors;
	static ImGuiAppLog log;

	ImGuiLogger::ImGuiLogger() : Layer("IMGUI_LOGGER_LAYER") {
		{
			m_LogColors[LogLevel::PV_INFO] = ImVec4(0, 1, 0, 1);
			m_LogColors[LogLevel::PV_WARN] = ImVec4(1, 1, 0, 1);
			m_LogColors[LogLevel::PV_ERROR] = ImVec4(1, 0, 0, 1);
			m_LogColors[LogLevel::PV_FATAL] = ImVec4(1, 0, 1, 1);
		}

		Log::m_LogFunction = [](std::string s, LogLevel level) -> void {
			log.AddLog(level, s);
		};

		PV_IMGUI_LOG("ImGui Logging Layer Created", LogLevel::PV_INFO);
	}

	void ImGuiLogger::OnImGuiUpdate() {
		if (is_logging)
			log.Draw("Log", m_LogColors, &is_logging);
	}

	void ImGuiLogger::OnAttach() {
		PV_IMGUI_LOG("ImGui Logging Layer Attached", LogLevel::PV_INFO);
	}

	void ImGuiLogger::OnDetach() {
		PV_IMGUI_LOG("ImGui Logging Layer Detached", LogLevel::PV_INFO);
	}

}