#pragma once
#include <ctime>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

std::string downloadData(const std::string& dlURL);
void downloadDataToFile(const std::string& dlURL, const std::string& dest);
void getBulkDownload(const std::string& dlURL, const std::string& dest);