#pragma once

#include "engine/layer/layer.h"

namespace prev {

	class ImGuiConsole : public Layer {
	public:
		ImGuiConsole();
		~ImGuiConsole();
		void AddConsoleCommand(const std::string & commandName, const std::string & commandDescription, std::function<void(std::vector<std::string>)> func);
	public:
		virtual void OnImGuiUpdate() override;
	};

}