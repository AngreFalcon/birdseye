#include "download.hpp"

#define USERAGENT "birdseye/0.1"

Set jsonToSet(json& set) {
	Set s;
	s.uuid = set["id"];
	s.setCode = set["code"];
	s.mtgoCode = set.value("mtgo_code", "null");
	s.arenaCode = set.value("arena_code", "null");
	s.tcgplayerId = set.value("tcgplayer_id", 0);
	s.name = set["name"];
	s.type = set["set_type"];
	s.releaseDate = stringToTime(set["released_at"]);
	s.blockCode = set.value("block_code", "null");
	s.block = set.value("block", "null");
	s.parentCode = set.value("parent_set_code", "null");
	s.cardCount = set["card_count"];
	s.printedSize = set.value("printed_size", 0);
	s.digital = set["digital"];
	s.foilOnly = set["foil_only"];
	s.nonfoilOnly = set["nonfoil_only"];
	s.IconURI = set["icon_svg_uri"];
	return s;
}

time_t stringToTime(json& date) {
	std::istringstream dateStream;
	dateStream.str() = date;
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
		curl_easy_setopt(curl, CURLOPT_USERAGENT, USERAGENT);
    	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
		dlResult = curl_easy_perform(curl);
	}
	curl_easy_cleanup(curl);
	return data;
}