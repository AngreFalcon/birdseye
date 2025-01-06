#pragma once
#include <string>

struct Keyword {
	Keyword(const std::string& name) {
		this->name = name;
	}
	Keyword(void) = default;
	~Keyword() = default;

	std::string name;
};