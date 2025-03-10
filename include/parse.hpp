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

	/**
	 * @brief Open curly bracket encountered. Begins the construction of a new JSON object.
	 * 
	 * @param elements The number of object elements or -1 if unknown.
	 * @return true Parsing should continue.
	 * @return false Parsing will be halted.
	 */
	bool start_object(std::size_t elements) override;

	/**
	 * @brief Open parenthesis encountered. Begins the construction of a new array.
	 * 
	 * @param elements The number of array elements or -1 if unknown.
	 * @return true Parsing should continue.
	 * @return false Parsing will be halted.
	 */
	bool start_array(std::size_t elements) override;

	/**
	 * @brief Key encountered. Begins the construction of a new key/value pair.
	 * 
	 * @param val The object key.
	 * @return true Parsing should continue.
	 * @return false Parsing will be halted.
	 */
	bool key(string_t& val) override;

	/**
	 * @brief Null value encountered. Key/value pair is ignored.
	 * 
	 * @return true Parsing should continue.
	 * @return false Parsing will be halted.
	 */
	bool null(void) override;

	/**
	 * @brief Binary value encountered. Inserted into the most recent container, whether that be an object or array, alongside its key if applicable.
	 * 
	 * @param val The binary value encountered.
	 * @return true Parsing should continue.
	 * @return false Parsing will be halted.
	 */
	bool binary(json::binary_t& val) override;

	/**
	 * @brief Boolean value encountered. Inserted into the most recent container, whether that be an object or array, alongside its key if applicable.
	 * 
	 * @param val The boolean value encountered.
	 * @return true Parsing should continue.
	 * @return false Parsing will be halted.
	 */
	bool boolean(bool val) override;

	/**
	 * @brief Integer value encountered. Inserted into the most recent container, whether that be an object or array, alongside its key if applicable.
	 * 
	 * @param val The integer value encountered.
	 * @return true Parsing should continue.
	 * @return false Parsing will be halted.
	 */
	bool number_integer(number_integer_t val) override;

	/**
	 * @brief Unsigned integer encountered. Inserted into the most recent container, whether that be an object or array, alongside its key if applicable.
	 * 
	 * @param val The unsigned integer encountered.
	 * @return true Parsing should continue.
	 * @return false Parsing will be halted.
	 */
	bool number_unsigned(number_unsigned_t val) override;

	/**
	 * @brief Floating point value encountered. Inserted into the most recent container, whether that be an object or array, alongside its key if applicable.
	 * 
	 * @param val The floating point value encountered.
	 * @param s The raw token value.
	 * @return true Parsing should continue.
	 * @return false Parsing will be halted.
	 */
	bool number_float(number_float_t val, const string_t& s) override;

	/**
	 * @brief String value encountered. Inserted in the most recent container, whether that be an object or array, alongside its key if applicable.
	 * 
	 * @param val The string value encountered.
	 * @return true Parsing should continue.
	 * @return false Parsing will be halted.
	 */
	bool string(string_t& val) override;

	/**
	 * @brief Ending curly brace encountered. Marks the completed construction of a JSON object. If this marks the completed construction of a card object, the card is then inserted into our database.
	 * 
	 * @return true Parsing should continue.
	 * @return false Parsing will be halted.
	 */
	bool end_object(void) override;

	/**
	 * @brief Ending parenthesis encountered. Marks the completed construction of an array. That array is then emplaced into the most recent container, whether that be an object or another array, alongside its key if applicable.
	 * 
	 * @return true Parsing should continue.
	 * @return false Parsing will be halted.
	 */
	bool end_array(void) override;

	/**
	 * @brief A parse error has occurred. This is treated as fatal and causes the program to terminate.
	 * 
	 * @param position The position in the input that the error occurred.
	 * @param last_token The last read token when the error occurred.
	 * @param ex An exception object describing the failure.
	 * @return true Parsing should continue.
	 * @return false Parsing will be halted.
	 */
	bool parse_error(std::size_t position, const std::string& last_token, const json::exception& ex) override;

private:
	std::function<void(Card& c)> cardFunc;
    std::stack<json> workingObj;
    std::stack<std::string> keys;

	/**
	 * @brief The method used to insert the most recent value, alongside its key if applicable, into the most recent container, whether that be an object or an array
	 * 
	 * @tparam T Any type that may be utilized in JSON for its values.
	 * @param val The value to be inserted into the most recent container.
	 */
	template <typename T>
	void insertVal(T val);

};
