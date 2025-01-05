#pragma once
#include <nlohmann/json.hpp>
#include <string>
#include <queue>

using json = nlohmann::json;

/**
 * @brief This function is used to retrieve a JSON object containing the data from the specified Scryfall API URL as a string in memory
 *
 * @param dlURL The Scryfall API URL that specifies the data to be downloaded
 * @return std::string A string containing the JSON contents of the specified URL
 */
std::string downloadData(const std::string& dlURL);

/**
 * @brief This function is used to retrieve a JSON object containing the data from the specified Scryfall API URL as a file
 *
 * @param dlURL The Scryfall API URL that specifies the data to be downloaded
 * @param dest The file path for the specified data to be downloaded to
 */
void downloadDataToFile(const std::string& dlURL, const std::string& dest);

/**
 * @brief This function is used to retrieve a JSON object or JSON objects containing the bulk data from the specified Scryfall API URL as a file
 *
 * @param dlURL The Scryfall API URL that specifies the data to be downloaded
 * @param dest The file path for the specified data to be downloaded to
 */
void getBulkDownload(const std::string& dlURL, const std::string& dest);

class Database;

class sax_event_consumer : public json::json_sax_t {
public:
	int layer = 0;
	std::queue<json> workingObj;
	Database* db;
	std::queue<std::string> keys;

    bool start_object(std::size_t elements) override;
    bool start_array(std::size_t elements) override;
    bool key(string_t& val) override;
    bool null() override;
    bool binary(json::binary_t& val) override;
    bool boolean(bool val) override;
    bool number_integer(number_integer_t val) override;
    bool number_unsigned(number_unsigned_t val) override;
    bool number_float(number_float_t val, const string_t& s) override;
    bool string(string_t& val) override;
    bool end_object() override;
    bool end_array() override;
    bool parse_error(std::size_t position, const std::string& last_token, const json::exception& ex) override;
};
