#pragma once
#include <string>

struct Format {
	Format(const std::string& name) {
		this->name = name;
	}
	Format(void) = default;
	~Format() = default;

	std::string name;
};