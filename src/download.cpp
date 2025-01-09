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
    nlohmann::json j = nlohmann::json::parse(str, nullptr, false);
    str = j["download_uri"];
    downloadDataToFile(str, dest);
    return;
}
