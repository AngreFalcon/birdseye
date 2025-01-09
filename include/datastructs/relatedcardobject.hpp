#pragma once
#include "nlohmann/json.hpp"
#include <string>

struct RelatedCardObject {
    using json = nlohmann::json;
    RelatedCardObject(const json& j) {
        this->id = j["id"];
        this->component = j["component"];
        this->name = j["name"];
        this->type_line = j["type_line"];
    }
    RelatedCardObject(void) = default;
    ~RelatedCardObject() = default;

    std::string id;
    std::string component;
    std::string name;
    std::string type_line;
};