#ifndef IGWINDOW_H
#define IGWINDOW_H

#include <string>
#include <vector>
#include <imgui.h>

class IGManager;
class Game;

class IGWindow {
public:
	IGWindow();
	IGWindow(const bool visible, const float alpha);
	virtual ~IGWindow();

	void draw(Game* g, IGManager* manager);

	void open();
	void close();
	bool isOpened() const;
	bool isFocused() const;

	bool containsPosition(sf::Vector2f point) const;

	float getAlpha() const;
	void setAlpha(float alpha);

protected:
	virtual void beforeRender(Game* g);
	virtual void afterRender(Game* g);

	virtual void render(Game* g, IGManager* manager) = 0;

	bool alpha;
	bool visible;
	bool focused = false;

	sf::Vector2f size = sf::Vector2f(-1, -1);
	sf::Vector2f position = sf::Vector2f(-1, -1);

	void setSize(sf::Vector2f size);
	void setPosition(sf::Vector2f position);
	std::string convertFloatToString(float number, int precision) const;

private:
	float prevAlpha;

};


static auto vector_getter = [](void* vec, int idx, const char** out_text) {
	auto& vector = *static_cast<std::vector<std::string>*>(vec);
	if (idx < 0 || idx >= static_cast<int>(vector.size())) { return false; }
	*out_text = vector.at(idx).c_str();
	return true;
};

inline bool ListBox(const char* label, int* currIndex, std::vector<std::string>& values, int maxLength = -1) {
	if (maxLength == -1)
		maxLength = values.size();

	if (values.empty()) { return false; }
	return ImGui::ListBox(label, currIndex, vector_getter,
		static_cast<void*>(&values), values.size(), maxLength);
}

#endif
