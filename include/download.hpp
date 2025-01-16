#pragma once
#include <string>

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
 * @param dest The FILE pointer used to write our data
 */
void downloadDataToFile(const std::string& dlURL, const std::string& cacheFileLoc);

/**
 * @brief This function is used to retrieve a JSON object or JSON objects containing the bulk data from the specified Scryfall API URL as a file
 *
 * @param dlURL The Scryfall API URL that specifies the data to be downloaded
 * @param dest The FILE pointer used to write our data
 */
void getBulkDownload(const std::string& dlURL, const std::string& cacheFileLoc);