#pragma once

namespace prev {

	class ImGuiLogger : public Layer {
	public:
		ImGuiLogger();
		virtual void OnImGuiUpdate() override;
		virtual void OnAttach() override;
		virtual void OnDetach() override;
	};

}