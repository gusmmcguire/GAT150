#pragma once
#include <string>

namespace gme {
	class Engine;
	std::string string_tolower(const std::string& str);
	bool istring_compare(const std::string& str1, const std::string& str2);
	std::string unique_string(const std::string& str);
	bool IsButtonPressed(int buttonID, Engine* engine);
	bool IsButtonHeld(int buttonID, Engine* engine);
}