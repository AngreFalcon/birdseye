#include "database.hpp"

void Database::createTable(const std::string& tableName, const std::string& fields) {
    dropTable(tableName);
    std::string sql = "CREATE TABLE " + tableName + " (" + fields + ");";
    try {
        SQLite::Transaction transaction(*connection);
        connection->exec(sql);
        transaction.commit();
    } catch (std::exception& e) {
        SPDLOG_TRACE("{}", e.what());
        exit(EXIT_FAILURE);
    }
    return;
}

void Database::dropTable(const std::string& tableName) {
    std::string sql = "DROP TABLE IF EXISTS " + tableName;
    try {
        SQLite::Transaction transaction(*connection);
        connection->exec(sql);
        transaction.commit();
    } catch (std::exception& e) {
        SPDLOG_TRACE("{}", e.what());
        exit(EXIT_FAILURE);
    }
    return;
}

void Database::insertArtist(const Artist& artist, const uint32_t faceId) {
    std::string sql = "INSERT OR IGNORE INTO Artist(id, name) VALUES (?,?);";
    SQLite::Statement stmt(*connection, sql);
    stmt.bind(1, artist.id);
    stmt.bind(2, artist.name);
    buildJunction("INSERT INTO Face_Artist(face_id,artist_id) VALUES (?,?);", faceId, artist.id);
    executeSQL(stmt);
    return;
}

void Database::insertCardSet(const CardSet& cardSet) {
    std::string sql = "INSERT INTO CardSet(id,code,name,set_type,card_count,digital,foil_only,nonfoil_only,icon_svg_uri,mtgo_code,arena_code,tcgplayer_id,released_at,block_code,block,parent_set_code,printed_size) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?);";
    SQLite::Statement stmt(*connection, sql);
	bindOpt(stmt, 1, cardSet.id);
    bindOpt(stmt, 2, cardSet.code);
    bindOpt(stmt, 3, cardSet.name);
    bindOpt(stmt, 4, cardSet.set_type);
    bindOpt(stmt, 5, cardSet.card_count);
    bindOpt(stmt, 6, cardSet.digital);
    bindOpt(stmt, 7, cardSet.foil_only);
    bindOpt(stmt, 8, cardSet.nonfoil_only);
    bindOpt(stmt, 9, cardSet.icon_svg_uri);
    bindOpt(stmt, 10, cardSet.mtgo_code);
    bindOpt(stmt, 11, cardSet.arena_code);
    bindOpt(stmt, 12, cardSet.tcgplayer_id);
    bindOpt(stmt, 13, cardSet.released_at);
    bindOpt(stmt, 14, cardSet.block_code);
    bindOpt(stmt, 15, cardSet.block);
    bindOpt(stmt, 16, cardSet.parent_set_code);
    bindOpt(stmt, 17, cardSet.printed_size);
    executeSQL(stmt);
    return;
}

void Database::insertCard(const Card& card) {
    SPDLOG_TRACE("entered insert card");
    std::string sql = "INSERT INTO Card("
                      "id, "
                      "name,"
                      "lang,"
                      "layout,"
                      "rulings_uri,"
                      "scryfall_uri,"
                      "cmc,"
                      "reserved,"
                      "type_line,"
                      "booster,"
                      "border_color,"
                      "collector_number,"
                      "digital,"
                      "frame,"
                      "full_art,"
                      "highres_image,"
                      "image_status,"
                      "oversized,"
                      "promo,"
                      "rarity,"
                      "released_at,"
                      "reprint,"
                      "set_id,"
                      "story_spotlight,"
                      "textless,"
                      "variation,"
                      "arena_id,"
                      "mtgo_id,"
                      "mtgo_foil_id,"
                      "tcgplayer_id,"
                      "tcgplayer_etched_id,"
                      "cardmarket_id,"
                      "oracle_id,"
                      "defense,"
                      "hand_modifier,"
                      "life_modifier,"
                      "loyalty,"
                      "card_back_id,"
                      "content_warning,"
                      "flavor_name,"
                      "printed_name,"
                      "printed_text,"
                      "printed_type_line,"
                      "variation_of,"
                      "security_stamp"
                      ") VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?);";
	SQLite::Transaction transaction(*connection);
    SQLite::Statement stmt(*connection, sql);
    // core fields
    bindOpt(stmt, 1, card.id);
    bindOpt(stmt, 2, card.name);
    bindOpt(stmt, 3, card.lang);
    bindOpt(stmt, 4, card.layout);
    bindOpt(stmt, 5, card.rulings_uri);
    bindOpt(stmt, 6, card.scryfall_uri);
    bindOpt(stmt, 7, card.cmc);
    bindOpt(stmt, 8, card.reserved);
    bindOpt(stmt, 9, card.type_line);
    bindOpt(stmt, 10, card.booster);
    bindOpt(stmt, 11, card.border_color);
    bindOpt(stmt, 12, card.collector_number);
    bindOpt(stmt, 13, card.digital);
    bindOpt(stmt, 14, card.frame);
    bindOpt(stmt, 15, card.full_art);
    bindOpt(stmt, 16, card.highres_image);
    bindOpt(stmt, 17, card.image_status);
    bindOpt(stmt, 18, card.oversized);
    bindOpt(stmt, 19, card.promo);
    bindOpt(stmt, 20, card.rarity);
    bindOpt(stmt, 21, card.released_at);
    bindOpt(stmt, 22, card.reprint);
    bindOpt(stmt, 23, card.set_id);
    bindOpt(stmt, 24, card.story_spotlight);
    bindOpt(stmt, 25, card.textless);
    bindOpt(stmt, 26, card.variation);
    bindOpt(stmt, 27, card.arena_id);
    bindOpt(stmt, 28, card.mtgo_id);
    bindOpt(stmt, 29, card.mtgo_foil_id);
    bindOpt(stmt, 30, card.tcgplayer_id);
    bindOpt(stmt, 31, card.tcgplayer_etched_id);
    bindOpt(stmt, 32, card.cardmarket_id);
    bindOpt(stmt, 33, card.oracle_id);
    bindOpt(stmt, 34, card.defense);
    bindOpt(stmt, 35, card.hand_modifier);
    bindOpt(stmt, 36, card.life_modifier);
    bindOpt(stmt, 37, card.loyalty);
    bindOpt(stmt, 38, card.card_back_id);
    bindOpt(stmt, 39, card.content_warning);
    bindOpt(stmt, 40, card.flavor_name);
    bindOpt(stmt, 41, card.printed_name);
    bindOpt(stmt, 42, card.printed_text);
    bindOpt(stmt, 43, card.printed_type_line);
    bindOpt(stmt, 44, card.variation_of);
    bindOpt(stmt, 45, card.security_stamp);
    executeSQL(stmt);
    for (const Color& c : card.color_identity) {
        insertColor(c);
        buildJunction("INSERT INTO Card_ColorIdentity(card_id,color_id) VALUES (?,?);", card.id.value(), c.color);
    }
    for (const Color& c : card.produced_mana) {
        insertColor(c);
        buildJunction("INSERT INTO Card_ProducedMana(card_id,color_id) VALUES (?,?);", card.id.value(), c.color);
    }
    for (const Face& face : card.card_faces) {
        insertFace(face, card.id.value());
    }
    for (const Finish& finish : card.finishes) {
        insertFinish(finish, card.id.value());
    }
    for (const Legality& legality : card.legalities) {
        insertLegality(legality, card.id.value());
    }
    for (const FrameEffect& frameEffect : card.frame_effects) {
        insertFrameEffect(frameEffect, card.id.value());
    }
    for (const Keyword& keyword : card.keywords) {
        insertKeyword(keyword, card.id.value());
    }
    for (const MultiverseID& multiverseId : card.multiverse_ids) {
        insertMultiverseID(multiverseId, card.id.value());
    }
    for (const PromoType& promoType : card.promo_types) {
        insertPromoType(promoType, card.id.value());
    }
    for (const RelatedCardObject& relatedCardObject : card.all_parts) {
        insertRelatedCardObject(relatedCardObject, card.id.value());
    }
	transaction.commit();
    return;
}

void Database::insertColor(const Color& color) {
    std::string sql = "INSERT OR IGNORE INTO Color(id) VALUES (?);";
    SQLite::Statement stmt(*connection, sql);
    stmt.bind(1, color.color);
    executeSQL(stmt);
    return;
}

void Database::insertFace(const Face& face, const std::string& cardId) {
    std::string sql = "INSERT INTO Face("
                      "card_id,"
                      "mana_cost,"
                      "name,"
                      "cmc,"
                      "defense,"
                      "flavor_text,"
                      "illustration_id,"
                      "layout,"
                      "loyalty,"
                      "oracle_id,"
                      "oracle_text,"
                      "power,"
                      "printed_name,"
                      "printed_type_line,"
                      "toughness,"
                      "type_line,"
                      "watermark,"
                      "image_uris) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?) RETURNING *;";
    SQLite::Statement stmt(*connection, sql);
    bindOpt(stmt, 1, cardId);
    bindOpt(stmt, 2, face.mana_cost);
    bindOpt(stmt, 3, face.name);
    bindOpt(stmt, 4, face.cmc);
    bindOpt(stmt, 5, face.defense);
    bindOpt(stmt, 6, face.flavor_text);
    bindOpt(stmt, 7, face.illustration_id);
    bindOpt(stmt, 8, face.layout);
    bindOpt(stmt, 9, face.loyalty);
    bindOpt(stmt, 10, face.oracle_id);
    bindOpt(stmt, 11, face.oracle_text);
    bindOpt(stmt, 12, face.power);
    bindOpt(stmt, 13, face.printed_name);
    bindOpt(stmt, 14, face.printed_type_line);
    bindOpt(stmt, 15, face.toughness);
    bindOpt(stmt, 16, face.type_line);
    bindOpt(stmt, 17, face.watermark);
    std::optional<uint32_t> imageUriId = face.image_uris ? std::optional(insertImageURI(face.image_uris.value())) : std::nullopt;
    bindOpt(stmt, 18, imageUriId);
    uint32_t id = executeSQLGetInt(stmt, 0);
    for (const Artist& a : face.artists) {
        insertArtist(a, id);
    }
    for (const Color& c : face.color_indicator) {
        insertColor(c);
        buildJunction("INSERT INTO Face_ColorIndicator(face_id,color_id) VALUES (?,?);", id, c.color);
    }
    for (const Color& c : face.colors) {
        insertColor(c);
        buildJunction("INSERT INTO Face_Colors(face_id,color_id) VALUES (?,?);", id, c.color);
    }
    return;
}

void Database::insertFinish(const Finish& finish, const std::string& cardId) {
    std::string sql = "INSERT OR IGNORE INTO Finish(type) VALUES (?);";
    SQLite::Statement stmt(*connection, sql);
    stmt.bind(1, finish.type);
    executeSQL(stmt);
    buildJunction("INSERT INTO Card_Finish(card_id,type) VALUES (?,?);", cardId, finish.type);
    return;
}

void Database::insertFormat(const Legality& legality) {
    std::string sql = "INSERT OR IGNORE INTO Format(name) VALUES (?);";
    SQLite::Statement stmt(*connection, sql);
    stmt.bind(1, legality.format);
    return;
}

void Database::insertFrameEffect(const FrameEffect& frameEffect, const std::string& cardId) {
    std::string sql = "INSERT OR IGNORE INTO FrameEffect(type) VALUES (?);";
    SQLite::Statement stmt(*connection, sql);
    stmt.bind(1, frameEffect.type);
    executeSQL(stmt);
    buildJunction("INSERT INTO Card_FrameEffect(card_id,type) VALUES (?,?);", cardId, frameEffect.type);
    return;
}

void Database::insertLegality(const Legality& legality, const std::string& cardId) {
    insertFormat(legality);
    insertRuling(legality);
    buildJunction("INSERT INTO Card_Format_Ruling(card_id,format_id,ruling_id) VALUES (?,?,?);", cardId, legality.format, legality.ruling);
    return;
}

uint32_t Database::insertImageURI(const ImageURI& imageUri) {
    std::string sql = "INSERT INTO ImageURI(small,normal,large,png,art_crop,border_crop) VALUES (?,?,?,?,?,?) RETURNING *;";
    SQLite::Statement stmt(*connection, sql);
    bindOpt(stmt, 1, imageUri.small);
    bindOpt(stmt, 2, imageUri.normal);
    bindOpt(stmt, 3, imageUri.large);
    bindOpt(stmt, 4, imageUri.png);
    bindOpt(stmt, 5, imageUri.art_crop);
    bindOpt(stmt, 6, imageUri.border_crop);
    return executeSQLGetInt(stmt, 0);
}

void Database::insertKeyword(const Keyword& keyword, const std::string& cardId) {
    std::string sql = "INSERT OR IGNORE INTO Keyword(name) VALUES (?);";
    SQLite::Statement stmt(*connection, sql);
    stmt.bind(1, keyword.name);
    buildJunction("INSERT INTO Card_Keyword(card_id,name) VALUES (?,?);", cardId, keyword.name);
    executeSQL(stmt);
    return;
}

void Database::insertMultiverseID(const MultiverseID& multiverseId, const std::string& cardId) {
    std::string sql = "INSERT OR IGNORE INTO MultiverseID(id,card_id) VALUES (?,?);";
    SQLite::Statement stmt(*connection, sql);
    stmt.bind(1, multiverseId.id);
    stmt.bind(2, cardId);
    executeSQL(stmt);
    return;
}

void Database::insertPromoType(const PromoType& promoType, const std::string& cardId) {
    std::string sql = "INSERT OR IGNORE INTO PromoType(type) VALUES (?);";
    SQLite::Statement stmt(*connection, sql);
    stmt.bind(1, promoType.type);
    buildJunction("INSERT INTO Card_PromoType(card_id,promo_type) VALUES (?,?);", cardId, promoType.type);
    executeSQL(stmt);
    return;
}

void Database::insertRelatedCardObject(const RelatedCardObject& relatedCardObject, const std::string& cardId) {
    std::string sql = "INSERT OR IGNORE INTO RelatedCardObject(id,component,name,type_line) VALUES (?,?,?,?);";
    SQLite::Statement stmt(*connection, sql);
    bindOpt(stmt, 1, relatedCardObject.id);
    bindOpt(stmt, 2, relatedCardObject.component);
    bindOpt(stmt, 3, relatedCardObject.name);
    bindOpt(stmt, 4, relatedCardObject.type_line);
    buildJunction("INSERT INTO Card_RelatedCardObject(card_id,relatedcardobject_id) VALUES (?,?);", cardId, relatedCardObject.id.value());
    executeSQL(stmt);
    return;
}

void Database::insertRuling(const Legality& legality) {
    std::string sql = "INSERT OR IGNORE INTO Ruling(ruling) VALUES (?);";
    SQLite::Statement stmt(*connection, sql);
    stmt.bind(1, legality.ruling);
    executeSQL(stmt);
    return;
}

template <typename T, typename U>
void Database::buildJunction(const std::string& sql, const T& firstPrimary, const U& secondPrimary) {
    SQLite::Statement stmt(*connection, sql);
    stmt.bind(1, firstPrimary);
    stmt.bind(2, secondPrimary);
    executeSQL(stmt);
    return;
}

template <typename T, typename U, typename V>
void Database::buildJunction(const std::string& sql, const T& firstPrimary, const U& secondPrimary, const V& thirdPrimary) {
    SQLite::Statement stmt(*connection, sql);
    stmt.bind(1, firstPrimary);
    stmt.bind(2, secondPrimary);
    stmt.bind(3, thirdPrimary);
    executeSQL(stmt);
    return;
}
