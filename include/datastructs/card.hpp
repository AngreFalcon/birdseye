#pragma once
#include "face.hpp"
#include "finish.hpp"
#include "frameeffect.hpp"
#include "keyword.hpp"
#include "legality.hpp"
#include "multiverseid.hpp"
#include "promotype.hpp"
#include "relatedcardobject.hpp"
#include <nlohmann/json.hpp>

struct Card {
    using json = nlohmann::json;
    Card(const json& j);
    Card() = default;
    ~Card() = default;

    // core card fields
    std::optional<uint32_t> arena_id;
    std::string id;
    std::string lang;
    std::optional<uint32_t> mtgo_id;
    std::optional<uint32_t> mtgo_foil_id;
    std::vector<MultiverseID> multiverse_ids;
    std::optional<uint32_t> tcgplayer_id;
    std::optional<uint32_t> tcgplayer_etched_id;
    std::optional<uint32_t> cardmarket_id;
    std::string layout;
    std::optional<std::string> oracle_id;
    std::string rulings_uri;
    std::string scryfall_uri;
    // gameplay fields
    std::vector<RelatedCardObject> all_parts;
    std::vector<Face> card_faces;
    double cmc;
    std::vector<Color> color_identity;
    std::optional<std::string> defense;
    std::optional<std::string> hand_modifier;
    std::vector<Keyword> keywords;
    std::vector<Legality> legalities;
    std::optional<std::string> life_modifier;
    std::optional<std::string> loyalty;
    std::string name;
    std::vector<Color> produced_mana;
    bool reserved;
    std::string type_line;
    // print fields
    bool booster;
    std::string border_color;
    std::optional<std::string> card_back_id;
    std::string collector_number;
    std::optional<bool> content_warning;
    bool digital;
    std::vector<Finish> finishes;
    std::optional<std::string> flavor_name;
    std::vector<FrameEffect> frame_effects;
    std::string frame;
    bool full_art;
    bool highres_image;
    std::string image_status;
    bool oversized;
    std::optional<std::string> printed_name;
    std::optional<std::string> printed_text;
    std::optional<std::string> printed_type_line;
    bool promo;
    std::vector<PromoType> promo_types;
    std::string rarity;
    std::string released_at;
    bool reprint;
    std::string set_id;
    bool story_spotlight;
    bool textless;
    bool variation;
    std::optional<std::string> variation_of;
    std::optional<std::string> security_stamp;
};

inline Card::Card(const json& j) {
    this->arena_id = j.contains("arena_id") ? std::optional(j["arena_id"]) : std::nullopt;
    this->id = j["id"];
    this->lang = j["lang"];
    this->mtgo_id = j.contains("mtgo_id") ? std::optional(j["mtgo_id"]) : std::nullopt;
    this->mtgo_foil_id = j.contains("mtgo_foil_id") ? std::optional(j["mtgo_foil_id"]) : std::nullopt;
    if (j.contains("multiverse_ids")) {
		for (auto& i : j["multiverse_ids"].items()) {
        	multiverse_ids.emplace_back(static_cast<uint32_t>(i.value()));
		}
	}
    this->tcgplayer_id = j.contains("tcgplayer_id") ? std::optional(j["tcgplayer_id"]) : std::nullopt;
    this->tcgplayer_etched_id = j.contains("tcgplayer_etched_id") ? std::optional(j["tcgplayer_etched_id"]) : std::nullopt;
    this->cardmarket_id = j.contains("cardmarket_id") ? std::optional(j["cardmarket_id"]) : std::nullopt;
    this->layout = j["layout"];
    this->oracle_id = j.contains("oracle_id") ? std::optional(j["oracle_id"]) : std::nullopt;
    this->rulings_uri = j["rulings_uri"];
    this->scryfall_uri = j["scryfall_uri"];
    if (j.contains("all_parts")) {
		for (auto& i : j["all_parts"].items()) {
        	all_parts.emplace_back(i.value());
		}
	}
    if (j.contains("card_faces")) {
        for (auto& i : j["card_faces"].items()) {
            card_faces.emplace_back(i.value());
        }
    }
    else {
        card_faces.emplace_back(j);
    }
    this->cmc = j["cmc"];
	if (j.contains("color_identity")) {
    	for (auto& i : j["color_identity"].items()) {
        	color_identity.emplace_back(static_cast<std::string>(i.value()));
		}
	}
    this->defense = j.contains("defense") ? std::optional(j["defense"]) : std::nullopt;
    this->hand_modifier = j.contains("hand_modifier") ? std::optional(j["hand_modifier"]) : std::nullopt;
    if (j.contains("keywords")) {
    	for (auto& i : j["keywords"].items()) {
        	keywords.emplace_back(static_cast<std::string>(i.value()));
		}
	}
    if (j.contains("legalities")) {
    	for (auto& i : j["legalities"].items()) {
    	    legalities.emplace_back(static_cast<std::string>(i.key()), static_cast<std::string>(i.value()));
		}
	}
    this->life_modifier = j.contains("life_modifier") ? std::optional(j["life_modifier"]) : std::nullopt;
    this->loyalty = j.contains("loyalty") ? std::optional(j["loyalty"]) : std::nullopt;
    this->name = j["name"];
    if (j.contains("produced_mana")) {
    	for (auto& i : j["produced_mana"].items()) {
        	produced_mana.emplace_back(static_cast<std::string>(i.value()));
		}
	}
    this->reserved = j["reserved"];
    this->type_line = j["type_line"];
    this->booster = j["booster"];
    this->border_color = j["border_color"];
    this->card_back_id = j.contains("card_back_id") ? std::optional(j["card_back_id"]) : std::nullopt;
    this->collector_number = j["collector_number"];
    this->content_warning = j.contains("content_warning") ? std::optional(j["content_warning"]) : std::nullopt;
    this->digital = j["digital"];
    if (j.contains("finishes")) {
    	for (auto& i : j["finishes"].items()) {
        	finishes.emplace_back(static_cast<std::string>(i.value()));
		}
	}
    this->flavor_name = j.contains("flavor_name") ? std::optional(j["flavor_name"]) : std::nullopt;
    if (j.contains("frame_effects")) {
    	for (auto& i : j["frame_effects"].items()) {
        	frame_effects.emplace_back(static_cast<std::string>(i.value()));
		}
	}
    this->frame = j["frame"];
    this->full_art = j["full_art"];
    this->highres_image = j["highres_image"];
    this->image_status = j["image_status"];
    this->oversized = j["oversized"];
    this->printed_name = j.contains("printed_name") ? std::optional(j["printed_name"]) : std::nullopt;
    this->printed_text = j.contains("printed_text") ? std::optional(j["printed_text"]) : std::nullopt;
    this->printed_type_line = j.contains("printed_type_line") ? std::optional(j["printed_type_line"]) : std::nullopt;
    this->promo = j["promo"];
    if (j.contains("promo_types")) {
    	for (auto& i : j["promo_types"].items()) {
        	promo_types.emplace_back(static_cast<std::string>(i.value()));
		}
	}
    this->rarity = j["rarity"];
    this->released_at = j["released_at"];
    this->reprint = j["reprint"];
    this->set_id = j["set_id"];
    this->story_spotlight = j["story_spotlight"];
    this->textless = j["textless"];
    this->variation = j["variation"];
    this->variation_of = j.contains("variation_of") ? std::optional(j["variation_of"]) : std::nullopt;
    this->security_stamp = j.contains("security_stamp") ? std::optional(j["security_stamp"]) : std::nullopt;
}
