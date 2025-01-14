#include "parse.hpp"
#include <cstdio>
#include <spdlog/spdlog.h>

std::vector<CardSet> parseSets(const std::string& jsonSrc) {
    std::vector<CardSet> cardsets;
    json j = json::parse(jsonSrc, nullptr, false);
    for (const auto& i : j["data"].items()) {
        cardsets.emplace_back(i.value());
    }
    return cardsets;
}

void parseCards(const std::string& cacheFileLoc, std::function<void(Card& c)> cardFunc) {
    FILE* cacheFile;
    fopen_s(&cacheFile, cacheFileLoc.c_str(), "r");
    sax_event_consumer sec(cardFunc);
    bool result = json::sax_parse(cacheFile, &sec);
    if (!result) // json file structure turned out invalid
    fclose(cacheFile);
    return;
}

sax_event_consumer::sax_event_consumer(std::function<void(Card& c)> cardFunc) {
    this->cardFunc = cardFunc;
}

bool sax_event_consumer::start_object(std::size_t elements) {
    workingObj.push(json::object());
    return true;
}

bool sax_event_consumer::start_array(std::size_t elements) {
    if (workingObj.size() != 0) {
        workingObj.top().emplace(keys.top(), json::array());
    }
    return true;
}

bool sax_event_consumer::key(string_t& val) {
    if (workingObj.size() > keys.size())
        keys.push(val);
    else {
        keys.top() = val;
    }
    return true;
}

bool sax_event_consumer::null() {
    return true;
}

bool sax_event_consumer::binary(json::binary_t& val) {
    insertVal(val);
    return true;
}

bool sax_event_consumer::boolean(bool val) {
    insertVal(val);
    return true;
}

bool sax_event_consumer::number_integer(number_integer_t val) {
    insertVal(val);
    return true;
}

bool sax_event_consumer::number_unsigned(number_unsigned_t val) {
    insertVal(val);
    return true;
}

bool sax_event_consumer::number_float(number_float_t val, const string_t& s) {
    insertVal(val);
    return true;
}

bool sax_event_consumer::string(string_t& val) {
    insertVal(val);
    return true;
}

bool sax_event_consumer::end_object() {
    json j = workingObj.top();
    workingObj.pop();
    if (keys.size() > workingObj.size()) 
		keys.pop();
    if (workingObj.size() == 0) {
        Card c(j);
        cardFunc(c);
    }
    else if (workingObj.top().contains(keys.top()) && workingObj.top()[keys.top()].is_array()) {
    	workingObj.top()[keys.top()].insert(workingObj.top()[keys.top()].end(), j);
    }
    else {
        workingObj.top().emplace(keys.top(), j);
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

template <typename T>
void sax_event_consumer::insertVal(T val) {
    json* j = &workingObj.top();
    if (j->contains(keys.top()) && j->at(keys.top()).is_array()) {
        j->at(keys.top()).insert(j->at(keys.top()).end(), val);
    }
    else {
        j->emplace(keys.top(), val);
    }
    return;
}
