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
#include <spdlog/spdlog.h>

struct Card {
    using json = nlohmann::json;
    Card(const json& j);
    Card() = default;
    ~Card() = default;

    // core card fields
    std::optional<uint32_t> arena_id;
    std::optional<std::string> id;
    std::optional<std::string> lang;
    std::optional<uint32_t> mtgo_id;
    std::optional<uint32_t> mtgo_foil_id;
    std::vector<MultiverseID> multiverse_ids;
    std::optional<uint32_t> tcgplayer_id;
    std::optional<uint32_t> tcgplayer_etched_id;
    std::optional<uint32_t> cardmarket_id;
    std::optional<std::string> layout;
    std::optional<std::string> oracle_id;
    std::optional<std::string> rulings_uri;
    std::optional<std::string> scryfall_uri;
    // gameplay fields
    std::vector<RelatedCardObject> all_parts;
    std::vector<Face> card_faces;
    std::optional<double> cmc;
    std::vector<Color> color_identity;
    std::optional<std::string> defense;
    std::optional<std::string> hand_modifier;
    std::vector<Keyword> keywords;
    std::vector<Legality> legalities;
    std::optional<std::string> life_modifier;
    std::optional<std::string> loyalty;
    std::optional<std::string> name;
    std::vector<Color> produced_mana;
    std::optional<bool> reserved;
    std::optional<std::string> type_line;
    // print fields
    std::optional<bool> booster;
    std::optional<std::string> border_color;
    std::optional<std::string> card_back_id;
    std::optional<std::string> collector_number;
    std::optional<bool> content_warning;
    std::optional<bool> digital;
    std::vector<Finish> finishes;
    std::optional<std::string> flavor_name;
    std::vector<FrameEffect> frame_effects;
    std::optional<std::string> frame;
    std::optional<bool> full_art;
    std::optional<bool> highres_image;
    std::optional<std::string> image_status;
    std::optional<bool> oversized;
    std::optional<std::string> printed_name;
    std::optional<std::string> printed_text;
    std::optional<std::string> printed_type_line;
    std::optional<bool> promo;
    std::vector<PromoType> promo_types;
    std::optional<std::string> rarity;
    std::optional<std::string> released_at;
    std::optional<bool> reprint;
    std::optional<std::string> set_id;
    std::optional<bool> story_spotlight;
    std::optional<bool> textless;
    std::optional<bool> variation;
    std::optional<std::string> variation_of;
    std::optional<std::string> security_stamp;
};

inline Card::Card(const json& j) {
    this->arena_id = j.contains("arena_id") ? std::optional(j["arena_id"]) : std::nullopt;
    this->id = j.contains("id") ? std::optional(j["id"]) : std::nullopt;
    this->lang = j.contains("lang") ? std::optional(j["lang"]) : std::nullopt;
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
    this->layout = j.contains("layout") ? std::optional(j["layout"]) : std::nullopt;
    this->oracle_id = j.contains("oracle_id") ? std::optional(j["oracle_id"]) : std::nullopt;
    this->rulings_uri = j.contains("rulings_uri") ? std::optional(j["rulings_uri"]) : std::nullopt;
    this->scryfall_uri = j.contains("scryfall_uri") ? std::optional(j["scryfall_uri"]) : std::nullopt;
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
    this->cmc = j.contains("cmc") ? std::optional(j["cmc"]) : std::nullopt;
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
    this->name = j.contains("name") ? std::optional(j["name"]) : std::nullopt;
    if (j.contains("produced_mana")) {
    	for (auto& i : j["produced_mana"].items()) {
        	produced_mana.emplace_back(static_cast<std::string>(i.value()));
		}
	}
    this->reserved = j.contains("reserved") ? std::optional(j["reserved"]) : std::nullopt;
    this->type_line = j.contains("type_line") ? std::optional(j["type_line"]) : std::nullopt;
    this->booster = j.contains("booster") ? std::optional(j["booster"]) : std::nullopt;
    this->border_color = j.contains("border_color") ? std::optional(j["border_color"]) : std::nullopt;
    this->card_back_id = j.contains("card_back_id") ? std::optional(j["card_back_id"]) : std::nullopt;
    this->collector_number = j["collector_number"];
    this->content_warning = j.contains("content_warning") ? std::optional(j["content_warning"]) : std::nullopt;
    this->digital = j.contains("digital") ? std::optional(j["digital"]) : std::nullopt;
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
    this->frame = j.contains("frame") ? std::optional(j["frame"]) : std::nullopt;
    this->full_art = j.contains("full_art") ? std::optional(j["full_art"]) : std::nullopt;
    this->highres_image = j.contains("highres_image") ? std::optional(j["highres_image"]) : std::nullopt;
    this->image_status = j.contains("image_status") ? std::optional(j["image_status"]) : std::nullopt;
    this->oversized = j.contains("oversized") ? std::optional(j["oversized"]) : std::nullopt;
    this->printed_name = j.contains("printed_name") ? std::optional(j["printed_name"]) : std::nullopt;
    this->printed_text = j.contains("printed_text") ? std::optional(j["printed_text"]) : std::nullopt;
    this->printed_type_line = j.contains("printed_type_line") ? std::optional(j["printed_type_line"]) : std::nullopt;
    this->promo = j.contains("promo") ? std::optional(j["promo"]) : std::nullopt;
    if (j.contains("promo_types")) {
    	for (auto& i : j["promo_types"].items()) {
        	promo_types.emplace_back(static_cast<std::string>(i.value()));
		}
	}
    this->rarity = j.contains("rarity") ? std::optional(j["rarity"]) : std::nullopt;
    this->released_at = j.contains("released_at") ? std::optional(j["released_at"]) : std::nullopt;
    this->reprint = j.contains("reprint") ? std::optional(j["reprint"]) : std::nullopt;
    this->set_id = j.contains("set_id") ? std::optional(j["set_id"]) : std::nullopt;
    this->story_spotlight = j.contains("story_spotlight") ? std::optional(j["story_spotlight"]) : std::nullopt;
    this->textless = j.contains("textless") ? std::optional(j["textless"]) : std::nullopt;
    this->variation = j.contains("variation") ? std::optional(j["variation"]) : std::nullopt;
    this->variation_of = j.contains("variation_of") ? std::optional(j["variation_of"]) : std::nullopt;
    this->security_stamp = j.contains("security_stamp") ? std::optional(j["security_stamp"]) : std::nullopt;
}
