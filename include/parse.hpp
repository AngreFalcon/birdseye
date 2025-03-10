#pragma once
#include "datastructs/card.hpp"
#include "datastructs/cardset.hpp"
#include <nlohmann/json.hpp>
#include <stack>

using json = nlohmann::json;

/**
 * @brief Performs a download of all cardsets using the Scryfall API, parsing them in-memory and then inserting them into a vector in preparation for database insertion.
 * 
 * @param jsonSrc A const reference to the string containing the raw JSON data downloaded from Scryfall.
 * @return std::vector<CardSet> A vector containing the now-parsed cardset data, ready for insertion into the database.
 */
std::vector<CardSet> parseSets(const std::string& jsonSrc);

/**
 * @brief Performs a bulk download of card data using the Scryfall API, parsing that data from an on-disk cache file while performing simultaneous database insertion via multithreaded operation.
 * 
 * @param cacheFileLoc The string containing the file path to where the temporary cache file should be located when downloading and parsing data to be inserted into the database.
 * @param cardFunc The callback function that will be used during json parsing of the cache file.
 */
void parseCards(const std::string& cacheFileLoc, std::function<void(Card& c)> cardFunc);

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
	std::function<void(Card& c)> cardFunc;
    std::stack<json> workingObj;
    std::stack<std::string> keys;

	template <typename T>
	void insertVal(T val);

};
