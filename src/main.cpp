#include <curl/curl.h>
#include <SQLiteCpp/SQLiteCpp.h>
#include <spdlog/spdlog.h>
#include <wolfssl/openssl/ssl.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <ctime>

struct Set {
	std::string uuid; // unique ID
	std::string setCode;
	std::string mtgoCode;
	std::string arenaCode;
	uint16_t tcgplayerId;
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

using json = nlohmann::json;

std::string downloadData(const std::string& dlURL);
size_t writeFunction(void *ptr, size_t size, size_t nmemb, std::string* data);

Set jsonToSet(json& set);
time_t stringToTime(json& date);

int main(int argc, char** argv) {
	json j;
	std::vector<Set> sets;
	std::string rawJSON = downloadData("https://api.scryfall.com/sets");
	j = json::parse(rawJSON, nullptr, false);
	for (const auto& i : j["data"].items()) {
		sets.emplace_back(jsonToSet(i.value()));
	}
	std::cout << "\n" << sets.size();
	return EXIT_SUCCESS;
}

Set jsonToSet(json& set) {
	Set s;
	s.uuid = set["id"];
	s.setCode = set["code"];
	s.mtgoCode = set.value("mtgo_code", "");
	s.arenaCode = set.value("arena_code", "");
	s.tcgplayerId = set.value("tcgplayer_id", 0);
	s.name = set["name"];
	s.type = set["set_type"];
	s.releaseDate = stringToTime(set["released_at"]);
	s.blockCode = set.value("block_code", "");
	s.block = set.value("block", "");
	s.parentCode = set.value("parent_set_code", "");
	s.cardCount = set["card_count"];
	s.printedSize = set.value("printed_size", 0);
	s.digital = set["digital"];
	s.foilOnly = set["foil_only"];
	s.nonfoilOnly = set["nonfoil_only"];
	s.IconURI = set["icon_svg_uri"];
	return s;
}

time_t stringToTime(json& date) {
	std::string dateStr = date;
	std::istringstream dateStream(dateStr);
	tm tm = {};
	dateStream >> std::get_time(&tm, "%Y-%m-%d");
	return mktime(&tm);
}

size_t writeFunction(void *ptr, size_t size, size_t nmemb, std::string* data) {
    data->append((char*) ptr, size * nmemb);
    return size * nmemb;
}

std::string downloadData(const std::string& dlURL) {
	CURL *curl = curl_easy_init();
	CURLcode dlResult;
	std::string data;
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, dlURL.c_str());
		curl_easy_setopt(curl, CURLOPT_USERAGENT, "birdseye/0.1");
    	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
		dlResult = curl_easy_perform(curl);
	}
	curl_easy_cleanup(curl);
	return data;
}

//https://gatherer.wizards.com/Handlers/Image.ashx?multiverseid=!set:muid!&type=card
//https://gatherer.wizards.com/Handlers/Image.ashx?name=!name!&type=card