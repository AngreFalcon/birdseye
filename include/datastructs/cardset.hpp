#pragma once
#include <string>
#include <vector>
#include <optional>

struct CardSet {
	using json = nlohmann::json;
	CardSet(const json& j) {
		this->id = j["id"];
		this->code = j["code"];
		this->mtgo_code = j.contains("mtgo_code") ? std::optional(j["mtgo_code"]) : std::nullopt;
		this->arena_code = j.contains("arena_code") ? std::optional(j["arena_code"]) : std::nullopt;
		this->tcgplayer_id = j.contains("tcgplayer_id") ? std::optional(j["tcgplayer_id"]) : std::nullopt;
		this->name = j["name"];
		this->set_type = j["set_type"];
		this->released_at = j.contains("released_at") ? std::optional(j["released_at"]) : std::nullopt;
		this->block_code = j.contains("block_code") ? std::optional(j["block_code"]) : std::nullopt;
		this->block = j.contains("block") ? std::optional(j["block"]) : std::nullopt;
		this->parent_set_code = j.contains("parent_set_code") ? std::optional(j["parent_set_code"]) : std::nullopt;
		this->card_count = j["card_count"];
		this->printed_size = j.contains("printed_size") ? std::optional(j["printed_size"]) : std::nullopt;
		this->digital = j["digital"];
		this->foil_only = j["foil_only"];
		this->nonfoil_only = j["nonfoil_only"];
		this->icon_svg_uri = j["icon_svg_uri"];
	}
	CardSet(void) = default;
	~CardSet() = default;

	std::string id;
	std::string code;
	std::optional<std::string> mtgo_code;
	std::optional<std::string> arena_code;
	std::optional<uint32_t> tcgplayer_id;
	std::string name;
	std::string set_type;
	std::optional<std::string> released_at;
	std::optional<std::string> block_code;
	std::optional<std::string> block;
	std::optional<std::string> parent_set_code;
	uint32_t card_count;
	std::optional<uint32_t> printed_size;
	bool digital;
	bool foil_only;
	bool nonfoil_only;
	std::string icon_svg_uri;
};