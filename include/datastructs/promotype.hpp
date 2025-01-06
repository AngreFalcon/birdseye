#pragma once
#include <string>

struct PromoType {
	PromoType(const std::string& type) {
		this->type = type;
	}
	PromoType(void) = default;
	~PromoType() = default;
	
	std::string type;
};