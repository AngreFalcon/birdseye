#include "database.hpp"
#include "download.hpp"
#include <ctime>
#include <curl/curl.h>
#include <spdlog/spdlog.h>

#define USERAGENT "birdseye/0.1"

size_t writeFunction(void* ptr, size_t size, size_t nmemb, std::string* data) {
    data->append((char*)ptr, size * nmemb);
    return size * nmemb;
}

std::string downloadData(const std::string& dlURL) {
    CURL* curl = curl_easy_init();
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
    CURL* curl = curl_easy_init();
    CURLcode dlResult;
    FILE* data;
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

sax_event_consumer::sax_event_consumer(Database* db) {
	this->db = db;
}

bool sax_event_consumer::start_object(std::size_t elements) {
    this->workingObj.push(json::object());
    return true;
}

bool sax_event_consumer::start_array(std::size_t elements) {
    if (this->workingObj.size() != 0) {
        this->workingObj.top().emplace(this->keys.top(), json::array());
    }
    return true;
}

bool sax_event_consumer::key(string_t& val) {
    if (this->workingObj.size() > this->keys.size())
        this->keys.push(val);
    else {
        this->keys.top() = val;
    }
    return true;
}

bool sax_event_consumer::null() {
    return true;
}

bool sax_event_consumer::binary(json::binary_t& val) {
    json* j = &this->workingObj.top();
    if (j->contains(this->keys.top()) && j->at(this->keys.top()).is_array()) {
        j->at(this->keys.top()).insert(j->at(this->keys.top()).end(), val);
    }
    else {
        j->emplace(this->keys.top(), val);
    }
    return true;
}

bool sax_event_consumer::boolean(bool val) {
    json* j = &this->workingObj.top();
    if (j->contains(this->keys.top()) && j->at(this->keys.top()).is_array()) {
        j->at(this->keys.top()).insert(j->at(this->keys.top()).end(), val);
    }
    else {
        j->emplace(this->keys.top(), val);
    }
    return true;
}

bool sax_event_consumer::number_integer(number_integer_t val) {
    json* j = &this->workingObj.top();
    if (j->contains(this->keys.top()) && j->at(this->keys.top()).is_array()) {
        j->at(this->keys.top()).insert(j->at(this->keys.top()).end(), val);
    }
    else {
        j->emplace(this->keys.top(), val);
    }
    return true;
}

bool sax_event_consumer::number_unsigned(number_unsigned_t val) {
    json* j = &this->workingObj.top();
    if (j->contains(this->keys.top()) && j->at(this->keys.top()).is_array()) {
        j->at(this->keys.top()).insert(j->at(this->keys.top()).end(), val);
    }
    else {
        j->emplace(this->keys.top(), val);
    }
    return true;
}

bool sax_event_consumer::number_float(number_float_t val, const string_t& s) {
    json* j = &this->workingObj.top();
    if (j->contains(this->keys.top()) && j->at(this->keys.top()).is_array()) {
        j->at(this->keys.top()).insert(j->at(this->keys.top()).end(), val);
    }
    else {
        j->emplace(this->keys.top(), val);
    }
    return true;
}

bool sax_event_consumer::string(string_t& val) {
    json* j = &this->workingObj.top();
    if (j->contains(this->keys.top()) && j->at(this->keys.top()).is_array()) {
        j->at(this->keys.top()).insert(j->at(this->keys.top()).end(), val);
    }
    else {
        j->emplace(this->keys.top(), val);
    }
    return true;
}

bool sax_event_consumer::end_object() {
    json::object_t j = this->workingObj.top();
    this->workingObj.pop();
    this->keys.pop();
    if (this->workingObj.size() == 0) {
        this->db->insertCard(Card(j));
    }
	else if (this->workingObj.top().contains(this->keys.top()) && this->workingObj.top()[this->keys.top()].is_array()) {
        this->workingObj.top()[this->keys.top()].insert(this->workingObj.top()[this->keys.top()].end(), j);
    }
    else {
    	this->workingObj.top().emplace(this->keys.top(), j);
    }
    return true;
}

bool sax_event_consumer::end_array() {
    return true;
}

bool sax_event_consumer::parse_error(std::size_t position, const std::string& last_token, const json::exception& ex) {
    SPDLOG_TRACE("parse_error(position={}, last_token={},\n            ex={})", position, last_token, ex.what());
    exit(EXIT_FAILURE);
    return false;
}
