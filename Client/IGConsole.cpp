#include "IGConsole.h"
#include "ImGuiFonts.h"
#include "Game.h"

IGConsole::IGConsole() {
	ClearLog();
	memset(InputBuf, 0, sizeof(InputBuf));
	HistoryPos = -1;
	Commands.push_back("HELP");
	Commands.push_back("CLEAR");

	AddLog("Welcome to Dear ImGui!");
	AddLog("Welcome to Dear ImGui2!");
}


IGConsole::~IGConsole() {
	ClearLog();
	for (int i = 0; i < History.Size; i++)
		free(History[i]);
}

void IGConsole::ExecCommand(const char* command_line) {
	


	// Process command
	if (Stricmp(command_line, "CLEAR") == 0) {
		AddLog("# %s\n", command_line);
		ClearLog();
	}
	else if (Stricmp(command_line, "HELP") == 0) {
		AddLog("# %s\n", command_line);
		AddLog("Commands:");
		for (int i = 0; i < Commands.Size; i++)
			AddLog("- %s", Commands[i]);
	}
	else {
		AddLog("%s\n", command_line);
	}

	ScrollToBottom = true;
}

int IGConsole::TextEditCallback(ImGuiTextEditCallbackData* data) {
	
	switch (data->EventFlag) {
		case ImGuiInputTextFlags_CallbackCompletion: {
			// Example of TEXT COMPLETION

			// Locate beginning of current word
			const char* word_end = data->Buf + data->CursorPos;
			const char* word_start = word_end;
			while (word_start > data->Buf) {
				const char c = word_start[-1];
				if (c == ' ' || c == '\t' || c == ',' || c == ';')
					break;
				word_start--;
			}

			// Build a list of candidates
			ImVector<const char*> candidates;
			for (int i = 0; i < Commands.Size; i++)
				if (Strnicmp(Commands[i], word_start, (int)(word_end - word_start)) == 0)
					candidates.push_back(Commands[i]);

			if (candidates.Size == 0) {
				// No match
				AddLog("No match for \"%.*s\"!\n", (int)(word_end - word_start), word_start);
			}
			else if (candidates.Size == 1) {
				// Single match. Delete the beginning of the word and replace it entirely so we've got nice casing
				data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
				data->InsertChars(data->CursorPos, candidates[0]);
				data->InsertChars(data->CursorPos, " ");
			}
			else {
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
	}
	return 0;

}

void IGConsole::render(Game* g, IGManager* manager) {

	ImGui::PushFont(g->fonts.getFont(ImGuiFonts::PRODIGY_TINY, 15));

	ImGui::SetNextWindowPos(ImVec2(10, 532), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(320, 225), ImGuiCond_FirstUseEver);
	if (!ImGui::Begin("Console", &visible, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus)) {
		ImGui::End();
		ImGui::PopFont();
		return;
	}

	if (ImGui::BeginPopupContextItem()) {
		if (ImGui::MenuItem("Close Console"))
			visible = false;
		ImGui::EndPopup();
	}

	ImGui::SameLine();

	const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetItemsLineHeightWithSpacing();
	// 1 separator, 1 input text
	ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false,
	                  ImGuiWindowFlags_HorizontalScrollbar); // Leave room for 1 separator + 1 InputText
	if (ImGui::BeginPopupContextWindow()) {
		if (ImGui::Selectable("Clear")) ClearLog();
		ImGui::EndPopup();
	}

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing

	ImVec4 col_default_text = ImGui::GetStyleColorVec4(ImGuiCol_Text);
	for (int i = 0; i < Items.Size; i++) {
		const char* item = Items[i];
		ImVec4 col = col_default_text;
		if (strstr(item, "[error]")) col = ImColor(1.0f, 0.4f, 0.4f, 1.0f);
		else if (strncmp(item, "# ", 2) == 0) col = ImColor(1.0f, 0.78f, 0.58f, 1.0f);
		ImGui::PushStyleColor(ImGuiCol_Text, col);
		ImGui::TextWrapped(item);
		ImGui::PopStyleColor();
	}
	
	if (ScrollToBottom)
		ImGui::SetScrollPosHere();
	ScrollToBottom = false;
	ImGui::PopStyleVar();
	ImGui::EndChild();
	ImGui::Separator();

	ImGui::PushItemWidth(-1);
	if (ImGui::InputText("", InputBuf, IM_ARRAYSIZE(InputBuf),
	                     ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion , &TextEditCallbackStub, (void*)this)) {
		char* s = InputBuf;
		Strtrim(s);
		if (s[0])
			ExecCommand(s);

		strcpy(s, "");
		remove_focus = true;
	}

	ImGui::PopItemWidth();

	if(remove_focus) {
		ImGui::SetWindowFocus(NULL);
		remove_focus = false;
	}


	if (reclaim_focus) {
		reclaim_focus = false;
		ImGui::SetWindowFocus();
		ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget
	}

	position = ImGui::GetWindowPos();
	size = ImGui::GetWindowSize();
	focused = ImGui::IsWindowFocused();

	ImGui::End();
	ImGui::PopFont();
}
