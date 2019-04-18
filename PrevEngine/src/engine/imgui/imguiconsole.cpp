#include "pch.h"
#include "imguiconsole.h"

#include <imgui.h>

struct AppConsole {

	using CmdParam = const std::vector<std::string> &;
	using CmdFunc = std::function<void(CmdParam)>;

	struct Command {
		std::string		CommandName;
		std::string		CommandDescription;
		CmdFunc			CommandFunction;
	};

	std::array<char, 256>		InputBuf;
	std::vector<std::string>	Items;
	std::vector<Command>		Commands;
	std::vector<std::string>	History;
	int							HistoryPos;    // -1: new line, 0..History.Size-1 browsing history.
	ImGuiTextFilter				Filter;
	bool						AutoScroll;
	bool						ScrollToBottom;

	AppConsole() {
		ClearLog();
		InputBuf.fill(0);
		HistoryPos = -1;
		Commands.push_back({ "help", "List all the commands", [this](CmdParam cmdParam) -> void {
			AddLog("Commands:");
			for (int i = 0; i < Commands.size(); i++)
				AddLog("- %s", Commands[i].CommandName.c_str());
		}});
		Commands.push_back({ "history", "List the previous executed commands", [this](CmdParam cmdParam) -> void {
			int first = History.size() - 10;
			for (int i = first > 0 ? first : 0; i < History.size(); i++)
				AddLog("%3d: %s\n", i, History[i].c_str());
		}});
		Commands.push_back({ "clear", "Clear the console screen", [this](CmdParam cmdParam) -> void { ClearLog(); } });
		Commands.push_back({ "classify", "Doesn't do anything", [this](CmdParam cmdParam) -> void { AddLog("Classify called"); } });
		AutoScroll = true;
		ScrollToBottom = true;
		AddLog("Welcome to Dear ImGui!");
	}

	~AppConsole() {
		ClearLog();
	}

	// Portable helpers
	//Compare string till a certain length. Case in-sensitive.
	static int Strnicmp(const char * str1, const char * str2, int n) { 
		int d = 0; 
		while (n > 0 && (d = toupper(*str2) - toupper(*str1)) == 0 && *str1) { 
			str1++; str2++; n--; 
		} 
		return d; 
	}
	//Remove black space from then end of the string
	static void TrimString(std::string & str) { 
		str = str.erase(str.find_last_not_of(" \n\r\t") + 1); 
	}
	//Case in-sensitive comparison
	static bool CompareString(const std::string & a, const std::string & b) {
		if (a.size() != b.size()) return false;
		for (unsigned int i = 0; i < a.size(); i++) {
			if (std::toupper(a[i]) != std::toupper(b[i]))
				return false;
		}
		return true;
	}

	static std::vector<std::string> SplitString(const std::string & str, const char delimiter) {
		std::vector<std::string> strVector;
		std::istringstream iss(str);
		std::string s;
		while (std::getline(iss, s, delimiter)) {
			strVector.push_back(s);
		}
		return strVector;
	}

	void ClearLog() {
		Items.clear();
		ScrollToBottom = true;
	}

	void AddLog(const char * fmt, ...) IM_FMTARGS(2) {
		// FIXME-OPT
		char buf[1024];
		va_list args;
		va_start(args, fmt);
		vsnprintf(buf, IM_ARRAYSIZE(buf), fmt, args);
		buf[IM_ARRAYSIZE(buf) - 1] = 0;
		va_end(args);
		Items.push_back(buf);
		if (AutoScroll)
			ScrollToBottom = true;
	}

	void Draw(const char * title, bool * p_open) {
		ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
		if (!ImGui::Begin(title, p_open)) {
			ImGui::End();
			return;
		}

		// As a specific feature guaranteed by the library, after calling Begin() the last Item represent the title bar. So e.g. IsItemHovered() will return true when hovering the title bar.
		// Here we create a context menu only available from the title bar.
		if (ImGui::BeginPopupContextItem()) {
			if (ImGui::MenuItem("Close Console"))
				* p_open = false;
			ImGui::EndPopup();
		}

		ImGui::TextWrapped("This example implements a console with basic coloring, completion and history. A more elaborate implementation may want to store entries along with extra data such as timestamp, emitter, etc.");
		ImGui::TextWrapped("Enter 'HELP' for help, press TAB to use text completion.");

		// TODO: display items starting from the bottom

		if (ImGui::SmallButton("Add Dummy Text")) { AddLog("%d some text", Items.size()); AddLog("some more text"); AddLog("display very important message here!"); } ImGui::SameLine();
		if (ImGui::SmallButton("Add Dummy Error")) { AddLog("[error] something went wrong"); } ImGui::SameLine();
		if (ImGui::SmallButton("Clear")) { ClearLog(); } ImGui::SameLine();
		bool copy_to_clipboard = ImGui::SmallButton("Copy"); ImGui::SameLine();
		if (ImGui::SmallButton("Scroll to bottom")) ScrollToBottom = true;
		//static float t = 0.0f; if (ImGui::GetTime() - t > 0.02f) { t = ImGui::GetTime(); AddLog("Spam %f", t); }

		ImGui::Separator();

		// Options menu
		if (ImGui::BeginPopup("Options")) {
			if (ImGui::Checkbox("Auto-scroll", &AutoScroll))
				if (AutoScroll)
					ScrollToBottom = true;
			ImGui::EndPopup();
		}

		// Options, Filter
		if (ImGui::Button("Options"))
			ImGui::OpenPopup("Options");
		ImGui::SameLine();
		Filter.Draw("Filter (\"incl,-excl\") (\"error\")", 180);
		ImGui::Separator();

		const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing(); // 1 separator, 1 input text
		ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar); // Leave room for 1 separator + 1 InputText
		if (ImGui::BeginPopupContextWindow()) {
			if (ImGui::Selectable("Clear")) ClearLog();
			ImGui::EndPopup();
		}

		// Display every line as a separate entry so we can change their color or add custom widgets. If you only want raw text you can use ImGui::TextUnformatted(log.begin(), log.end());
		// NB- if you have thousands of entries this approach may be too inefficient and may require user-side clipping to only process visible items.
		// You can seek and display only the lines that are visible using the ImGuiListClipper helper, if your elements are evenly spaced and you have cheap random access to the elements.
		// To use the clipper we could replace the 'for (int i = 0; i < Items.Size; i++)' loop with:
		//     ImGuiListClipper clipper(Items.Size);
		//     while (clipper.Step())
		//         for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
		// However, note that you can not use this code as is if a filter is active because it breaks the 'cheap random-access' property. We would need random-access on the post-filtered list.
		// A typical application wanting coarse clipping and filtering may want to pre-compute an array of indices that passed the filtering test, recomputing this array when user changes the filter,
		// and appending newly elements as they are inserted. This is left as a task to the user until we can manage to improve this example code!
		// If your items are of variable size you may want to implement code similar to what ImGuiListClipper does. Or split your data into fixed height items to allow random-seeking into your list.
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing
		if (copy_to_clipboard)
			ImGui::LogToClipboard();
		for (int i = 0; i < Items.size(); i++) {
			const char * item = Items[i].c_str();
			if (!Filter.PassFilter(item))
				continue;

			// Normally you would store more information in your item (e.g. make Items[] an array of structure, store color/type etc.)
			bool pop_color = false;
			if (strstr(item, "[error]")) { 
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.4f, 0.4f, 1.0f)); 
				pop_color = true; 
			} else if (strncmp(item, "# ", 2) == 0) { 
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.8f, 0.6f, 1.0f)); 
				pop_color = true; 
			} else if (strncmp(item, "[HELP COMMAND :", 15) == 0) {
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.5f, 0.0f, 1.0f));
				pop_color = true;
			}
			ImGui::TextUnformatted(item);
			if (pop_color)
				ImGui::PopStyleColor();
		}
		if (copy_to_clipboard)
			ImGui::LogFinish();
		if (ScrollToBottom)
			ImGui::SetScrollHereY(1.0f);
		ScrollToBottom = false;
		ImGui::PopStyleVar();
		ImGui::EndChild();
		ImGui::Separator();

		// Command-line
		bool reclaim_focus = false;
		if (ImGui::InputText("Input", &InputBuf.front(), InputBuf.max_size(), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory, &TextEditCallbackStub, (void *)this)) {
			std::string s = &InputBuf.front();
			TrimString(s);
			if (s[0])
				ExecCommand(s);
			InputBuf.fill(0);
			reclaim_focus = true;
		}

		// Auto-focus on window apparition
		ImGui::SetItemDefaultFocus();
		if (reclaim_focus)
			ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget

		ImGui::End();
	}

	void ExecCommand(const std::string & command_line) {
		AddLog("# %s\n", command_line.c_str());

		// Insert into history. First find match and delete it so it can be pushed to the back. This isn't trying to be smart or optimal.
		HistoryPos = -1;
		for (int i = History.size() - 1; i >= 0; i--)
			if (History[i] == command_line) {
				History.erase(History.begin() + i);
				break;
			}
		History.push_back(command_line);

		std::vector<std::string> strVector = SplitString(command_line, ' ');

		bool command_found = false;

		for (const Command & command : Commands) {
			if (CompareString(strVector[0], command.CommandName)) {
				if (strVector.size() > 1 && strVector[1] == "-help") {
					AddLog("[HELP COMMAND : %s]"
						   "\n----------------------------\n%s"
						   , command.CommandName.c_str(), command.CommandDescription.c_str());
				} else {
					command.CommandFunction(strVector);
				}
				command_found = true;
			}
		}

		if (!command_found) {
			// TODO: Implement color changing
			AddLog("Unknown command: '%s'\n", strVector[0].c_str());
		}

		// On commad input, we scroll to bottom even if AutoScroll==false
		ScrollToBottom = true;
	}

	static int TextEditCallbackStub(ImGuiInputTextCallbackData * data) // In C++11 you are better off using lambdas for this sort of forwarding callbacks
	{
		AppConsole * console = (AppConsole *)data->UserData;
		return console->TextEditCallback(data);
	}

	int TextEditCallback(ImGuiInputTextCallbackData * data) {
		//AddLog("cursor: %d, selection: %d-%d", data->CursorPos, data->SelectionStart, data->SelectionEnd);
		switch (data->EventFlag) {
		case ImGuiInputTextFlags_CallbackCompletion:
		{
			// Example of TEXT COMPLETION

			// Locate beginning of current word
			const char * word_end = data->Buf + data->CursorPos;
			const char * word_start = word_end;
			while (word_start > data->Buf) {
				const char c = word_start[-1];
				if (c == ' ' || c == '\t' || c == ',' || c == ';')
					break;
				word_start--;
			}

			// Build a list of candidates
			ImVector<const char *> candidates;
			for (int i = 0; i < Commands.size(); i++)
				if (Strnicmp(Commands[i].CommandName.c_str(), word_start, (int)(word_end - word_start)) == 0)
					candidates.push_back(Commands[i].CommandName.c_str());

			if (candidates.Size == 0) {
				// No match
				AddLog("No match for \"%.*s\"!\n", (int)(word_end - word_start), word_start);
			} else if (candidates.Size == 1) {
				// Single match. Delete the beginning of the word and replace it entirely so we've got nice casing
				data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
				data->InsertChars(data->CursorPos, candidates[0]);
				data->InsertChars(data->CursorPos, " ");
			} else {
				// Multiple matches. Complete as much as we can, so inputing "C" will complete to "CL" and display "CLEAR" and "CLASSIFY"
				int match_len = (int)(word_end - word_start);
				for (;;) {
					int c = 0;
					bool all_candidates_matches = true;
					for (int i = 0; i < candidates.Size && all_candidates_matches; i++)
						if (i == 0)
							c = toupper(candidates[i][match_len]);
						else if (c == 0 || c != toupper(candidates[i][match_len]))
							all_candidates_matches = false;
					if (!all_candidates_matches)
						break;
					match_len++;
				}

				if (match_len > 0) {
					data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
					data->InsertChars(data->CursorPos, candidates[0], candidates[0] + match_len);
				}

				// List matches
				AddLog("Possible matches:\n");
				for (int i = 0; i < candidates.Size; i++)
					AddLog("- %s\n", candidates[i]);
			}

			break;
		}
		case ImGuiInputTextFlags_CallbackHistory:
		{
			// Example of HISTORY
			const int prev_history_pos = HistoryPos;
			if (data->EventKey == ImGuiKey_UpArrow) {
				if (HistoryPos == -1)
					HistoryPos = History.size() - 1;
				else if (HistoryPos > 0)
					HistoryPos--;
			} else if (data->EventKey == ImGuiKey_DownArrow) {
				if (HistoryPos != -1)
					if (++HistoryPos >= History.size())
						HistoryPos = -1;
			}

			// A better implementation would preserve the data on the current input line along with cursor position.
			if (prev_history_pos != HistoryPos) {
				const char * history_str = (HistoryPos >= 0) ? History[HistoryPos].c_str() : "";
				data->DeleteChars(0, data->BufTextLen);
				data->InsertChars(0, history_str);
			}
		}
		}
		return 0;
	}
};

namespace prev {

	static AppConsole s_Console;
	static bool s_IsOpen = true;

	ImGuiConsole::ImGuiConsole() {
	}

	ImGuiConsole::~ImGuiConsole() {
	}

	void ImGuiConsole::AddConsoleCommand(const std::string & commandName, 
										 const std::string & commandDescription, 
										 std::function<void(std::vector<std::string>)> func) {
		s_Console.Commands.push_back({ commandName, commandDescription, func });
	}

	void ImGuiConsole::OnImGuiUpdate() {
		if (s_IsOpen)
			s_Console.Draw("Console", &s_IsOpen);
	}

}