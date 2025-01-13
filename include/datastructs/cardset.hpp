#pragma once
#include <optional>
#include <string>
#include <vector>

struct CardSet {
    using json = nlohmann::json;
    CardSet(const json& j) {
        this->id = j.contains("id") ? std::optional(j["id"]) : std::nullopt;
        this->code = j.contains("code") ? std::optional(j["code"]) : std::nullopt;
        this->mtgo_code = j.contains("mtgo_code") ? std::optional(j["mtgo_code"]) : std::nullopt;
        this->arena_code = j.contains("arena_code") ? std::optional(j["arena_code"]) : std::nullopt;
        this->tcgplayer_id = j.contains("tcgplayer_id") ? std::optional(j["tcgplayer_id"]) : std::nullopt;
        this->name = j.contains("name") ? std::optional(j["name"]) : std::nullopt;
        this->set_type = j.contains("set_type") ? std::optional(j["set_type"]) : std::nullopt;
        this->released_at = j.contains("released_at") ? std::optional(j["released_at"]) : std::nullopt;
        this->block_code = j.contains("block_code") ? std::optional(j["block_code"]) : std::nullopt;
        this->block = j.contains("block") ? std::optional(j["block"]) : std::nullopt;
        this->parent_set_code = j.contains("parent_set_code") ? std::optional(j["parent_set_code"]) : std::nullopt;
        this->card_count = j.contains("card_count") ? std::optional(j["card_count"]) : std::nullopt;
        this->printed_size = j.contains("printed_size") ? std::optional(j["printed_size"]) : std::nullopt;
        this->digital = j.contains("digital") ? std::optional(j["digital"]) : std::nullopt;
        this->foil_only = j.contains("foil_only") ? std::optional(j["foil_only"]) : std::nullopt;
        this->nonfoil_only = j.contains("nonfoil_only") ? std::optional(j["nonfoil_only"]) : std::nullopt;
        this->icon_svg_uri = j.contains("icon_svg_uri") ? std::optional(j["icon_svg_uri"]) : std::nullopt;
    }
    CardSet(void) = default;
    ~CardSet() = default;

    std::optional<std::string> id;
    std::optional<std::string> code;
    std::optional<std::string> mtgo_code;
    std::optional<std::string> arena_code;
    std::optional<uint32_t> tcgplayer_id;
    std::optional<std::string> name;
    std::optional<std::string> set_type;
    std::optional<std::string> released_at;
    std::optional<std::string> block_code;
    std::optional<std::string> block;
    std::optional<std::string> parent_set_code;
    std::optional<uint32_t> card_count;
    std::optional<uint32_t> printed_size;
    std::optional<bool> digital;
    std::optional<bool> foil_only;
    std::optional<bool> nonfoil_only;
    std::optional<std::string> icon_svg_uri;
};