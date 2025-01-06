#pragma once
#include <string>

struct Color {
	Color(const std::string& color) {
		this->color = color;
	}
	Color(void) = default;
	~Color() = default;
	
	std::string color;
};