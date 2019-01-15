#include "IGConsole.h"
#include "ImGuiFonts.h"
#include "Game.h"
#include "Utils.h"
#include "EventDispatcher.h"

IGConsole::IGConsole() {
	ClearLog();
	memset(InputBuf, 0, sizeof(InputBuf));
	HistoryPos = -1;
	Commands.push_back("HELP");
	Commands.push_back("CLEAR");

	EventDispatcher<EventSendMessage>::addSubscriber(this);
}


IGConsole::~IGConsole() {
	EventDispatcher<EventSendMessage>::removeSubscriber(this);
	ClearLog();
	for (int i = 0; i < History.Size; i++)
		free(History[i]);

}

void IGConsole::ExecCommand(const char* command_line) {
	
	// Process command
	if (Stricmp(command_line, "CLEAR") == 0) {
		ClearLog();
	}
	else if (Stricmp(command_line, "HELP") == 0) {
		ConsoleItem* item = new ConsoleItem();
		item->message = "Commands: ";
		item->time = Utils::utcTimeToLocalTime(Utils::getActualUtcTime());
		item->type = MessageType::SERVER_ANNOUNCEMENT;

		for (int i = 0; i < Commands.Size; i++) {
			item->message += "\n - " + std::string(Commands[i]);
		}
		
		AddLog(item);
	} else {
		EventSendMessage e;
		e.message = std::string(command_line);
		e.messageType = MessageType::SAY;
		e.time = Utils::getActualUtcTime();
		e.playerId = game->getAccount()->getPlayerEntity()->getId();

		sf::Packet* p = e.toPacket();
		game->packet_manager->sendPacket(p);
		delete p;
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
				ConsoleItem* item = new ConsoleItem();
				item->message = "No match";
				item->time = Utils::utcTimeToLocalTime(Utils::getActualUtcTime());
				item->type = MessageType::SERVER_ANNOUNCEMENT;
				AddLog(item);
			}
			else if (candidates.Size == 1) {
				// Single match. Delete the beginning of the word and replace it entirely so we've got nice casing
				data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
				data->InsertChars(data->CursorPos, candidates[0]);
				data->InsertChars(data->CursorPos, " ");
			}
			else {
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

				ConsoleItem* item = new ConsoleItem();
				item->message = "Possible matches: ";
				item->time = Utils::utcTimeToLocalTime(Utils::getActualUtcTime());
				item->type = MessageType::SERVER_ANNOUNCEMENT;
			
				for (int i = 0; i < candidates.Size; i++) {
					item->message += "\n - " + std::string(candidates[i]);
				}

				AddLog(item);
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
	for (unsigned int i = 0; i < messages.size(); i++) {
		ConsoleItem* ci = messages[i];

		char buff[20];
		strftime(buff, 20, "%H:%M:%S", localtime(&ci->time));
		std::string strMessage = std::string(buff);

		if (!ci->player.empty()) {
			strMessage += " " + ci->player;
		}

		strMessage += ": " + ci->message;


		ImVec4 col = col_default_text;
		if (ci->type == MessageType::SERVER_ANNOUNCEMENT) col = ImColor(0,0,205);

		ImGui::PushStyleColor(ImGuiCol_Text, col);
		ImGui::TextWrapped(strMessage.c_str());
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
		ImGui::SetWindowFocus(nullptr);
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

void IGConsole::handleEvent(GameEvent* event) {
	switch (event->getId()) {
		case SEND_MESSAGE: {
			EventSendMessage* e = (EventSendMessage*) event;
			ConsoleItem* item = new ConsoleItem();
			item->message = e->message;
			item->type = e->messageType;
			item->time = Utils::utcTimeToLocalTime(e->time);
			item->playerId = e->playerId;
			item->player = "";

			Map* map = game->getMap();
			Player* p = map->getPlayerById(e->playerId);
			if (p) {
				item->player = p->getName();
			}

			AddLog(item);
			break;
		}
		default: break;
	} 

}
