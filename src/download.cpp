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

bool sax_event_consumer::start_object(std::size_t elements) {
    SPDLOG_TRACE("enter start_object");
    layer++;
    this->workingObj.push(json::object());
    return true;
}

bool sax_event_consumer::start_array(std::size_t elements) {
    SPDLOG_TRACE("enter start_array");
    if (this->layer != 0) {
        this->workingObj.back().emplace(this->keys.back(), json::array());
        if (!this->workingObj.back().contains(this->keys.back())) {
            SPDLOG_TRACE("failed to emplace element");
            exit(EXIT_FAILURE);
        }
    }
    return true;
}

bool sax_event_consumer::key(string_t& val) {
    SPDLOG_TRACE("enter key");
    if (this->layer > this->keys.size())
        this->keys.push(val);
    else {
        if (!this->keys.empty() && !this->workingObj.back().contains(this->keys.back()) && this->keys.back() == "id") {
            SPDLOG_TRACE("failed to emplace element {}", this->keys.back());
            exit(EXIT_FAILURE);
        }
        this->keys.back() = val;
    }
    return true;
}

bool sax_event_consumer::null() {
    SPDLOG_TRACE("enter null");
    return true;
}

bool sax_event_consumer::binary(json::binary_t& val) {
    SPDLOG_TRACE("enter binary");
    json* j = &this->workingObj.back()[this->keys.back()];
    if (this->workingObj.back().contains(this->keys.back()) && (*j).is_array()) {
        (*j).insert((*j).end(), val);
    }
    else {
        *j = json::binary_t(val);
        // this->workingObj.back().emplace(this->keys.back(), val);
        if (!this->workingObj.back().contains(this->keys.back())) {
            SPDLOG_TRACE("failed to emplace element");
            exit(EXIT_FAILURE);
        }
    }
    return true;
}

bool sax_event_consumer::boolean(bool val) {
    SPDLOG_TRACE("enter boolean");
    json* j = &this->workingObj.back()[this->keys.back()];
    if (this->workingObj.back().contains(this->keys.back()) && (*j).is_array()) {
        (*j).insert((*j).end(), val);
    }
    else {
        *j = val;
        // this->workingObj.back().emplace(this->keys.back(), val);
        if (!this->workingObj.back().contains(this->keys.back())) {
            SPDLOG_TRACE("failed to emplace element");
            exit(EXIT_FAILURE);
        }
    }
    return true;
}

bool sax_event_consumer::number_integer(number_integer_t val) {
    SPDLOG_TRACE("enter number_integer");
    json* j = &this->workingObj.back()[this->keys.back()];
    if (this->workingObj.back().contains(this->keys.back()) && (*j).is_array()) {
        (*j).insert((*j).end(), val);
    }
    else {
        *j = json::number_integer_t(val);
        // this->workingObj.back().emplace(this->keys.back(), val);
        if (!this->workingObj.back().contains(this->keys.back())) {
            SPDLOG_TRACE("failed to emplace element");
            exit(EXIT_FAILURE);
        }
    }
    return true;
}

bool sax_event_consumer::number_unsigned(number_unsigned_t val) {
    SPDLOG_TRACE("enter number_unsigned");
    json* j = &this->workingObj.back()[this->keys.back()];
    if (this->workingObj.back().contains(this->keys.back()) && (*j).is_array()) {
        (*j).insert((*j).end(), val);
    }
    else {
        *j = json::number_unsigned_t(val);
        // this->workingObj.back().emplace(this->keys.back(), val);
        if (!this->workingObj.back().contains(this->keys.back())) {
            SPDLOG_TRACE("failed to emplace element");
            exit(EXIT_FAILURE);
        }
    }
    return true;
}

bool sax_event_consumer::number_float(number_float_t val, const string_t& s) {
    SPDLOG_TRACE("enter number_float");
    json* j = &this->workingObj.back()[this->keys.back()];
    if (this->workingObj.back().contains(this->keys.back()) && (*j).is_array()) {
        (*j).insert((*j).end(), val);
    }
    else {
        *j = json::number_float_t(val);
        // this->workingObj.back().emplace(this->keys.back(), val);
        if (!this->workingObj.back().contains(this->keys.back())) {
            SPDLOG_TRACE("failed to emplace element");
            exit(EXIT_FAILURE);
        }
    }
    return true;
}

bool sax_event_consumer::string(string_t& val) {
    SPDLOG_TRACE("enter string");
    json* j = &this->workingObj.back()[this->keys.back()];
    if (this->workingObj.back().contains(this->keys.back()) && (*j).is_array()) {
        (*j).insert((*j).end(), val);
    }
    else {
        *j = json::string_t(val);
        // this->workingObj.back().emplace(this->keys.back(), val);
        if (!this->workingObj.back().contains(this->keys.back())) {
            SPDLOG_TRACE("failed to emplace element");
            exit(EXIT_FAILURE);
        }
    }
    return true;
}

bool sax_event_consumer::end_object() {
    SPDLOG_TRACE("enter end_object. layer = {}", this->layer);
    this->keys.pop();
    this->layer--;
    json::object_t j = this->workingObj.back();
    // delete this->workingObj.back();
    this->workingObj.pop();
    if (this->layer == 0) {
        SPDLOG_TRACE("contains id: {}", j.contains("id"));
        this->db->insertCard(j);
    }
    else if (this->workingObj.back().contains(this->keys.back()) && this->workingObj.back()[this->keys.back()].is_array()) {
        this->workingObj.back()[this->keys.back()].insert(this->workingObj.back()[this->keys.back()].end(), j);
    }
    else {
        this->workingObj.back().emplace(this->keys.back(), j);
    }
    return true;
}

bool sax_event_consumer::end_array() {
    SPDLOG_TRACE("enter end_array");
    return true;
}

bool sax_event_consumer::parse_error(std::size_t position, const std::string& last_token, const json::exception& ex) {
    SPDLOG_TRACE("parse_error(position={}, last_token={},\n            ex={})", position, last_token, ex.what());
    exit(EXIT_FAILURE);
    return false;
}
