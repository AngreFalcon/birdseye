#pragma once
#include "nlohmann/json.hpp"
#include <string>

struct ImageURI {
    using json = nlohmann::json;
    inline ImageURI(const json& j) {
        this->small = j.contains("small") ? std::optional(j["small"]) : std::nullopt;
        this->normal = j.contains("normal") ? std::optional(j["normal"]) : std::nullopt;
        this->large = j.contains("large") ? std::optional(j["large"]) : std::nullopt;
        this->png = j.contains("png") ? std::optional(j["png"]) : std::nullopt;
        this->art_crop = j.contains("art_crop") ? std::optional(j["art_crop"]) : std::nullopt;
        this->border_crop = j.contains("border_crop") ? std::optional(j["border_crop"]) : std::nullopt;
    }
    ImageURI(void) = default;
    ~ImageURI() = default;

    std::optional<std::string> small;
    std::optional<std::string> normal;
    std::optional<std::string> large;
    std::optional<std::string> png;
    std::optional<std::string> art_crop;
    std::optional<std::string> border_crop;
};