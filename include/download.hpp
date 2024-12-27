#pragma once
#include <ctime>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

struct Set {
	std::string uuid; // unique ID
	std::string setCode;
	std::string mtgoCode;
	std::string arenaCode;
	uint32_t tcgplayerId;
	std::string name;
	std::string type;
	std::time_t releaseDate;
	std::string blockCode;
	std::string block;
	std::string parentCode;
	uint16_t cardCount;
	uint16_t printedSize;
	bool digital;
	bool foilOnly;
	bool nonfoilOnly;
	std::string IconURI;
};

std::string downloadData(const std::string& dlURL);
size_t writeFunction(void *ptr, size_t size, size_t nmemb, std::string* data);
Set jsonToSet(json& set);
time_t stringToTime(json& date);