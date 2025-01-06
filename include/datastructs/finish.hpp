#pragma once
#include <string>

struct Finish {
	Finish(const std::string& type) {
		this->type = type;
	}
	Finish(void) = default;
	~Finish() = default;

	std::string type;
};