#ifndef IGWINDOW_H
#define IGWINDOW_H
#include <string>
#include <vector>
#include <imgui.h>

class Game;

class IGWindow
{
public:
	IGWindow();
	IGWindow(const bool visible, const float alpha);
	virtual ~IGWindow();

	void draw(Game* g);

	void open();
	void close();
	bool isOpened() const;
	bool isFocused() const;

	float getAlpha() const;
	void setAlpha(float alpha);

protected:
	virtual void beforeDraw(Game* g);
	virtual void afterDraw(Game* g);

	virtual void onDraw(Game* g) = 0;

	bool alpha;
	bool visible;
	bool focused = false;
private:
	float prevAlpha;

};



static auto vector_getter = [](void* vec, int idx, const char** out_text)
{
	auto& vector = *static_cast<std::vector<std::string>*>(vec);
	if (idx < 0 || idx >= static_cast<int>(vector.size())) { return false; }
	*out_text = vector.at(idx).c_str();
	return true;
};

inline bool ListBox(const char* label, int* currIndex, std::vector<std::string>& values, int maxLength = -1)
{
	if (maxLength == -1)
		maxLength = values.size();

	if (values.empty()) { return false; }
	return ImGui::ListBox(label, currIndex, vector_getter,
		static_cast<void*>(&values), values.size(), maxLength);
}

#endif
