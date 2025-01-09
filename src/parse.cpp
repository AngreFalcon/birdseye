#include "database.hpp"
#include "parse.hpp"
#include <spdlog/spdlog.h>
#include <cstdio>

std::vector<CardSet> parseSets(const std::string& jsonSrc) {
    std::vector<CardSet> cardsets;
    json j = json::parse(jsonSrc, nullptr, false);
    for (const auto& i : j["data"].items()) {
        cardsets.emplace_back(i.value());
    }
    return cardsets;
}

void parseCards(const std::string& cacheFile, std::function<void(Card& c)> cardFunc) {
    FILE* file;
    fopen_s(&file, cacheFile.c_str(), "r");
	sax_event_consumer sec(cardFunc);
    bool result = json::sax_parse(file, &sec);
    if (!result) {
        // json file structure turned out invalid
    }
	fclose(file);
	return;
}

sax_event_consumer::sax_event_consumer(std::function<void(Card& c)> cardFunc) {
	this->cardFunc = cardFunc;
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
        // we have finished assembling a json object that is ready to be converted into a card object
		// now we need to do something with it
		Card c(j);
		this->cardFunc(c);
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
