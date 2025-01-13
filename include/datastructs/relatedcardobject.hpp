#pragma once
#include "nlohmann/json.hpp"
#include <string>

struct RelatedCardObject {
    using json = nlohmann::json;
    RelatedCardObject(const json& j) {
        this->id = j.contains("id") ? std::optional(j["id"]) : std::nullopt;
        this->component = j.contains("component") ? std::optional(j["component"]) : std::nullopt;
        this->name = j.contains("name") ? std::optional(j["name"]) : std::nullopt;
        this->type_line = j.contains("type_line") ? std::optional(j["type_line"]) : std::nullopt;
    }
    RelatedCardObject(void) = default;
    ~RelatedCardObject() = default;

    std::optional<std::string> id;
    std::optional<std::string> component;
    std::optional<std::string> name;
    std::optional<std::string> type_line;
};