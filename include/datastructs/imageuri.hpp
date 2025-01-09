#pragma once
#include "nlohmann/json.hpp"
#include <string>

struct ImageURI {
    using json = nlohmann::json;
    inline ImageURI(const json& j) {
        this->small = j["small"];
        this->normal = j["normal"];
        this->large = j["large"];
        this->png = j["png"];
        this->art_crop = j["art_crop"];
        this->border_crop = j["border_crop"];
    }
    ImageURI(void) = default;
    ~ImageURI() = default;

    std::string small;
    std::string normal;
    std::string large;
    std::string png;
    std::string art_crop;
    std::string border_crop;
};