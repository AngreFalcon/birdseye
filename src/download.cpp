#include "download.hpp"
#include "database.hpp"
#include <ctime>
#include <curl/curl.h>
#include <spdlog/spdlog.h>

#define USERAGENT "birdseye/0.1"

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

void downloadDataToFile(const std::string& dlURL, const std::string& dest) {
	CURL *curl = curl_easy_init();
	CURLcode dlResult;
	FILE *data;
	if (curl) {
		fopen_s(&data, dest.c_str(), "wb");
		curl_easy_setopt(curl, CURLOPT_URL, dlURL.c_str());
		curl_easy_setopt(curl, CURLOPT_USERAGENT, USERAGENT);
    	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, data);
		dlResult = curl_easy_perform(curl);
	}
	fclose(data);
	curl_easy_cleanup(curl);
	return;
}

void getBulkDownload(const std::string& dlURL, const std::string& dest) {
	std::string str = downloadData(dlURL);
	json j = json::parse(str, nullptr, false);
	str = j["download_uri"];
	downloadDataToFile(str, dest);
	return;
}

bool sax_event_consumer::start_object(std::size_t elements) {
	layer++;
	if (layer == 2) {
		tempCard = new json;
		return true;
	}
    return true;
}

bool sax_event_consumer::start_array(std::size_t elements) {
	layer++;
    //events.push_back("start_array(elements=" + std::to_string(elements) + ")");
    return true;
}

bool sax_event_consumer::key(string_t& val) {
    //events.push_back("key(val=" + val + ")");
    return true;
}

bool sax_event_consumer::null() {
    //events.push_back("null()");
    return true;
}

bool sax_event_consumer::binary(json::binary_t& val) {
    //events.push_back("binary(val=[...])");
    return true;
}

bool sax_event_consumer::boolean(bool val) {
    //events.push_back("boolean(val=" + std::string(val ? "true" : "false") + ")");
    return true;
}

bool sax_event_consumer::number_integer(number_integer_t val) {
    //events.push_back("number_integer(val=" + std::to_string(val) + ")");
    return true;
}

bool sax_event_consumer::number_unsigned(number_unsigned_t val) {
    //events.push_back("number_unsigned(val=" + std::to_string(val) + ")");
    return true;
}

bool sax_event_consumer::number_float(number_float_t val, const string_t& s) {
    //events.push_back("number_float(val=" + std::to_string(val) + ", s=" + s + ")");
    return true;
}

bool sax_event_consumer::string(string_t& val) {
    //events.push_back("string(val=" + val + ")");
    return true;
}

bool sax_event_consumer::end_object() {
	layer--;
	// pass our card json off to a database function to write the contents to our db
	// need to be able to pass in our database object so that we can perform database insertions from within our parse operation
	this->db->insertCard(*this->tempCard);
	delete tempCard;
    //events.push_back("end_object()");
    return true;
}

bool sax_event_consumer::end_array() {
	layer--;
    //events.push_back("end_array()");
    return true;
}

bool sax_event_consumer::parse_error(std::size_t position, const std::string& last_token, const json::exception& ex) {
    SPDLOG_TRACE("parse_error(position={}, last_token={},\n            ex={})", position, last_token, ex.what());
	exit(EXIT_FAILURE);
    return false;
}
