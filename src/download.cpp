#include "download.hpp"
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

#define USERAGENT "birdseye/0.1"

size_t writeFunction(void* ptr, size_t size, size_t nmemb, std::string* data) {
    data->append((char*)ptr, size * nmemb);
    return size * nmemb;
}

std::string downloadData(const std::string& dlURL) {
    std::string* data;
    CURL* curl = curl_easy_init();
    CURLcode dlResult;
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, dlURL.c_str());
        curl_easy_setopt(curl, CURLOPT_USERAGENT, USERAGENT);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, data);
        dlResult = curl_easy_perform(curl);
    }
    curl_easy_cleanup(curl);
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // ensure we respect scryfall's API guidelines by waiting at least 100 milliseconds before we can possibly initiate another connection
    return *data;
}

void downloadDataToFile(const std::string& dlURL, const std::string& cacheFileLoc) {
	FILE* data;
    fopen_s(&data, cacheFileLoc.c_str(), "wb");
    CURL* curl = curl_easy_init();
    CURLcode dlResult;
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, dlURL.c_str());
        curl_easy_setopt(curl, CURLOPT_USERAGENT, USERAGENT);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, data);
        dlResult = curl_easy_perform(curl);
    }
    curl_easy_cleanup(curl);
	std::this_thread::sleep_for(std::chrono::milliseconds(100)); // ensure we respect scryfall's API guidelines by waiting at least 100 milliseconds before we can possibly initiate another connection
    fclose(data);
    return;
}

void getBulkDownload(const std::string& dlURL, const std::string& cacheFileLoc) {
    std::string str = downloadData(dlURL);
    nlohmann::json j = nlohmann::json::parse(str, nullptr, false);
    str = j["download_uri"];
    downloadDataToFile(str, cacheFileLoc);
    return;
}