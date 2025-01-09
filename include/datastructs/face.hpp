#pragma once
#include "artist.hpp"
#include "color.hpp"
#include "imageuri.hpp"
#include <optional>
#include <string>
#include <vector>

struct Face {
    using json = nlohmann::json;
    Face(const json& obj);
    Face(void) = default;
    ~Face() = default;

    std::vector<Artist> artists;
    std::optional<double> cmc;
    std::vector<Color> color_indicator;
    std::vector<Color> colors;
    std::optional<std::string> defense;
    std::optional<std::string> flavor_text;
    std::optional<std::string> illustration_id;
    std::optional<ImageURI> image_uris;
    std::optional<std::string> layout;
    std::optional<std::string> loyalty;
    std::string mana_cost;
    std::string name;
    std::optional<std::string> oracle_id;
    std::optional<std::string> oracle_text;
    std::optional<std::string> power;
    std::optional<std::string> printed_name;
    std::optional<std::string> printed_type_line;
    std::optional<std::string> toughness;
    std::optional<std::string> type_line;
    std::optional<std::string> watermark;

    std::vector<std::string> separateArtistNames(const std::string& names);
};

inline Face::Face(const json& obj) {
    if (obj.contains("artist_id")) {
        this->artists.emplace_back(obj["artist_id"], obj["artist"]);
    }
    else if (obj.contains("artist_ids")) {
        std::vector<std::string> artistNames = separateArtistNames(obj["artist"]);
        size_t i = 0;
        for (auto& j : obj["artist_ids"].items()) {
            this->artists.emplace_back(j.value(), artistNames[i++]);
        }
    }
    this->cmc = obj.contains("cmc") ? std::optional(obj["cmc"]) : std::nullopt;
    if (obj.contains("color_indicator")) {
        for (auto& j : obj["color_indicator"].items()) {
            this->color_indicator.emplace_back(j.value());
        }
    }
    if (obj.contains("colors")) {
        for (auto& j : obj["colors"].items()) {
            this->colors.emplace_back(j.value());
        }
    }
    this->defense = obj.contains("defense") ? std::optional(obj["defense"]) : std::nullopt;
    this->flavor_text = obj.contains("flavor_text") ? std::optional(obj["flavor_text"]) : std::nullopt;
    this->illustration_id = obj.contains("illustration_id") ? std::optional(obj["illustration_id"]) : std::nullopt;
    if (obj.contains("image_uris")) {
        for (auto& j : obj["image_uris"]) {
            this->image_uris = ImageURI(obj["image_uris"]);
        }
    }
    this->layout = obj.contains("layout") ? std::optional(obj["layout"]) : std::nullopt;
    this->loyalty = obj.contains("loyalty") ? std::optional(obj["loyalty"]) : std::nullopt;
    this->mana_cost = obj.contains("mana_cost") ? obj["mana_cost"] : "";
    this->name = obj["name"];
    this->oracle_id = obj.contains("oracle_id") ? std::optional(obj["oracle_id"]) : std::nullopt;
    this->oracle_text = obj.contains("oracle_text") ? std::optional(obj["oracle_text"]) : std::nullopt;
    this->power = obj.contains("power") ? std::optional(obj["power"]) : std::nullopt;
    this->printed_name = obj.contains("printed_name") ? std::optional(obj["printed_name"]) : std::nullopt;
    this->printed_type_line = obj.contains("printed_type_line") ? std::optional(obj["printed_type_line"]) : std::nullopt;
    this->toughness = obj.contains("toughness") ? std::optional(obj["toughness"]) : std::nullopt;
    this->type_line = obj.contains("type_line") ? std::optional(obj["type_line"]) : std::nullopt;
    this->watermark = obj.contains("watermark") ? std::optional(obj["watermark"]) : std::nullopt;
}

inline std::vector<std::string> Face::separateArtistNames(const std::string& names) {
    std::vector<std::string> artistNames = { "" };
    for (int i = 0; i < names.size(); i++) {
        if (names[i] == '&') {
            artistNames.emplace_back();
        }
        else if (i > 0 && names[i] == ' ' && names[i - 1] == '&') {
            continue;
        }
        else {
            artistNames.back() += names[i];
        }
    }
    for (std::string i : artistNames) {
        while (i.back() == ' ') {
            i.pop_back();
        }
    }
    return artistNames;
}