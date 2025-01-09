#pragma once
#include "datastructs/card.hpp"
#include "datastructs/cardset.hpp"
#include <BS_thread_pool.hpp>
#include <nlohmann/json.hpp>
#include <stack>
#include <queue>

using json = nlohmann::json;

std::vector<CardSet> parseSets(const std::string& jsonSrc);
void parseCards(const std::string& cacheFile, std::function<void(Card& c)> cardFunc);

class Database;

class sax_event_consumer : public json::json_sax_t {
public:
    sax_event_consumer(std::function<void(Card& c)> cardFunc);
    sax_event_consumer(void) = default;
    ~sax_event_consumer() = default;

    bool start_object(std::size_t elements) override;
    bool start_array(std::size_t elements) override;
    bool key(string_t& val) override;
    bool null(void) override;
    bool binary(json::binary_t& val) override;
    bool boolean(bool val) override;
    bool number_integer(number_integer_t val) override;
    bool number_unsigned(number_unsigned_t val) override;
    bool number_float(number_float_t val, const string_t& s) override;
    bool string(string_t& val) override;
    bool end_object(void) override;
    bool end_array(void) override;
    bool parse_error(std::size_t position, const std::string& last_token, const json::exception& ex) override;

private:
    std::stack<json> workingObj;
    std::stack<std::string> keys;
	std::function<void(Card& c)> cardFunc;
};