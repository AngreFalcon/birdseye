#pragma once
#include "artist.hpp"
#include "color.hpp"
#include "imageuri.hpp"
#include <optional>
#include <string>
#include <vector>

struct Face {
	using json = nlohmann::json;
	Face(const json& faceObj, const json& baseObj) {

		/// /// //
		// this->artists = j.contains("artists") ? j["artists"].get<std::vector<Artist>>() : std::vector<Artist>();
		// this->cmc = j.contains("cmc") ? std::optional(j["cmc"]) : std::nullopt;
		// this->color_indicator = j.contains("color_indicator") ? j["color_indicator"].get<std::vector<Color>>() : std::vector<Color>();
		// this->colors = j.contains("colors") ? j["colors"].get<std::vector<Color>>() : std::vector<Color>();
		// this->defense = j.contains("defense") ? std::optional(j["defense"]) : std::nullopt;
		// this->flavor_text = j.contains("flavor_text") ? std::optional(j["flavor_text"]) : std::nullopt;
		// this->illustration_id = j.contains("illustration_id") ? std::optional(j["illustration_id"]) : std::nullopt;
		// this->image_uris = j.contains("image_uris") ? std::optional(j["image_uris"].get<ImageURI>()) : std::nullopt;
		// this->layout = j.contains("layout") ? std::optional(j["layout"]) : std::nullopt;
		// this->loyalty = j.contains("loyalty") ? std::optional(j["loyalty"]) : std::nullopt;
		// this->mana_cost = j.contains("mana_cost") ? j["mana_cost"] : "";
		// this->name = j["name"];
		// this->oracle_id = j.contains("oracle_id") ? std::optional(j["oracle_id"]) : std::nullopt;
		// this->oracle_text = j.contains("oracle_text") ? std::optional(j["oracle_text"]) : std::nullopt;
		// this->power = j.contains("power") ? std::optional(j["power"]) : std::nullopt;
		// this->printed_name = j.contains("printed_name") ? std::optional(j["printed_name"]) : std::nullopt;
		// this->printed_type_line = j.contains("printed_type_line") ? std::optional(j["printed_type_line"]) : std::nullopt;
		// this->toughness = j.contains("toughness") ? std::optional(j["toughness"]) : std::nullopt;
		// this->type_line = j.contains("type_line") ? std::optional(j["type_line"]) : std::nullopt;
		// this->watermark = j.contains("watermark") ? std::optional(j["watermark"]) : std::nullopt;
	}
	Face(const json& baseObj) {

		/// /// //
		// this->artists = j.contains("artists") ? j["artists"].get<std::vector<Artist>>() : std::vector<Artist>();
		// this->cmc = j.contains("cmc") ? std::optional(j["cmc"]) : std::nullopt;
		// this->color_indicator = j.contains("color_indicator") ? j["color_indicator"].get<std::vector<Color>>() : std::vector<Color>();
		// this->colors = j.contains("colors") ? j["colors"].get<std::vector<Color>>() : std::vector<Color>();
		// this->defense = j.contains("defense") ? std::optional(j["defense"]) : std::nullopt;
		// this->flavor_text = j.contains("flavor_text") ? std::optional(j["flavor_text"]) : std::nullopt;
		// this->illustration_id = j.contains("illustration_id") ? std::optional(j["illustration_id"]) : std::nullopt;
		// this->image_uris = j.contains("image_uris") ? std::optional(j["image_uris"].get<ImageURI>()) : std::nullopt;
		// this->layout = j.contains("layout") ? std::optional(j["layout"]) : std::nullopt;
		// this->loyalty = j.contains("loyalty") ? std::optional(j["loyalty"]) : std::nullopt;
		// this->mana_cost = j.contains("mana_cost") ? j["mana_cost"] : "";
		// this->name = j["name"];
		// this->oracle_id = j.contains("oracle_id") ? std::optional(j["oracle_id"]) : std::nullopt;
		// this->oracle_text = j.contains("oracle_text") ? std::optional(j["oracle_text"]) : std::nullopt;
		// this->power = j.contains("power") ? std::optional(j["power"]) : std::nullopt;
		// this->printed_name = j.contains("printed_name") ? std::optional(j["printed_name"]) : std::nullopt;
		// this->printed_type_line = j.contains("printed_type_line") ? std::optional(j["printed_type_line"]) : std::nullopt;
		// this->toughness = j.contains("toughness") ? std::optional(j["toughness"]) : std::nullopt;
		// this->type_line = j.contains("type_line") ? std::optional(j["type_line"]) : std::nullopt;
		// this->watermark = j.contains("watermark") ? std::optional(j["watermark"]) : std::nullopt;
	}
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
};