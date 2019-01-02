#ifndef IG_HELP_H
#define IG_HELP_H
#include "IGWindow.h"


class IGHelp:
	public IGWindow
{
public:
	IGHelp();
	~IGHelp();
protected:
	void render(Game* g, IGManager* manager) override;

	int selectedMenuItem;

	static void ShowHelpMarker(const char* desc)
	{
	    ImGui::TextDisabled("(?)");
	    if (ImGui::IsItemHovered())
	    {
	        ImGui::BeginTooltip();
	        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
	        ImGui::TextUnformatted(desc);
	        ImGui::PopTextWrapPos();
	        ImGui::EndTooltip();
	    }
	}
};

#endif
