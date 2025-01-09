#include "database.hpp"
#include <spdlog/spdlog.h>

Database::Database(const std::string& dbLoc) {
    this->connection = std::unique_ptr<SQLite::Database>(new SQLite::Database(dbLoc, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE));
}

void Database::initialize() {
    SPDLOG_TRACE("initialize");
    if (validateAll())
        return;
    createTable("Artist",
        "id TEXT PRIMARY KEY, "
        "name TEXT");
    createTable("CardSet",
        "id TEXT PRIMARY KEY, "
        "code TEXT, "
        "mtgo_code TEXT, "
        "arena_code TEXT, "
        "tcgplayer_id INTEGER, "
        "name TEXT, "
        "set_type TEXT, "
        "released_at BLOB, "
        "block_code TEXT, "
        "block TEXT, "
        "parent_set_code TEXT, "
        "card_count INTEGER, "
        "printed_size INTEGER, "
        "digital INTEGER, "
        "foil_only INTEGER, "
        "nonfoil_only INTEGER, "
        "icon_svg_uri TEXT");
    createTable("Card",
        "id TEXT PRIMARY KEY, "
        "set_id TEXT REFERENCES CardSet(id), "
        "arena_id INTEGER, "
        "lang TEXT, "
        "mtgo_id INTEGER, "
        "mtgo_foil_id INTEGER, "
        "tcgplayer_id INTEGER, "
        "tcgplayer_etched_id INTEGER, "
        "cardmarket_id INTEGER, "
        "layout TEXT, "
        "oracle_id TEXT, "
        "rulings_uri TEXT, "
        "cmc REAL, "
        "defense TEXT, "
        "hand_modifier TEXT, "
        "life_modifier TEXT, "
        "loyalty TEXT, "
        "name TEXT, "
        "reserved INTEGER, "
        "type_line TEXT, "
        "booster INTEGER, "
        "border_color TEXT, "
        "card_back_id TEXT, "
        "collector_number TEXT, "
        "content_warning INTEGER, "
        "digital INTEGER, "
        "flavor_name TEXT, "
        "frame TEXT, "
        "full_art INTEGER, "
        "highres_image INTEGER, "
        "image_status TEXT, "
        "oversized INTEGER, "
        "printed_name TEXT, "
        "printed_text TEXT, "
        "printed_type_line TEXT, "
        "promo INTEGER, "
        "rarity TEXT, "
        "released_at BLOB, "
        "reprint INTEGER, "
        "story_spotlight INTEGER, "
        "textless INTEGER, "
        "variation INTEGER, "
        "variation_of TEXT, "
        "security_stamp TEXT, "
        "scryfall_uri TEXT");
    createTable("Color",
        "id TEXT PRIMARY KEY");
    createTable("Face",
        "id INTEGER PRIMARY KEY, "
        "card_id TEXT REFERENCES Card(id), "
        "cmc REAL, "
        "defense TEXT, "
        "flavor_text TEXT, "
        "illustration_id TEXT, "
        "image_uris INTEGER, "
        "layout TEXT, "
        "loyalty TEXT, "
        "mana_cost TEXT, "
        "name TEXT, "
        "oracle_id TEXT, "
        "oracle_text TEXT, "
        "power TEXT, "
        "printed_name TEXT, "
        "printed_type_line TEXT, "
        "toughness TEXT, "
        "type_line TEXT, "
        "watermark TEXT");
    createTable("Finish",
        "type TEXT PRIMARY KEY");
    createTable("FrameEffect",
        "type TEXT PRIMARY KEY");
    createTable("ImageURI",
        "id INTEGER PRIMARY KEY, "
        "small TEXT, "
        "normal TEXT, "
        "large TEXT, "
        "png TEXT, "
        "art_crop TEXT, "
        "border_crop TEXT");
    createTable("Keyword",
        "name TEXT PRIMARY KEY");
    createTable("MultiverseID",
        "id TEXT PRIMARY KEY, "
        "card_id TEXT REFERENCES Card(id)");
    createTable("PromoType",
        "type TEXT PRIMARY KEY");
    createTable("RelatedCardObject",
        "id TEXT PRIMARY KEY, "
        "component TEXT, "
        "name TEXT, "
        "type_line TEXT");
    createTable("Format",
        "name TEXT PRIMARY KEY");
    createTable("Ruling",
        "ruling TEXT PRIMARY KEY");
    createTable("Card_ColorIdentity",
        "card_id TEXT REFERENCES Card(id), "
        "color_id TEXT REFERENCES Color(id), "
        "PRIMARY KEY(card_id,color_id)");
    createTable("Card_Finish",
        "card_id TEXT REFERENCES Card(id), "
        "type TEXT REFERENCES Finish(type), "
        "PRIMARY KEY(card_id,type)");
    createTable("Card_Format_Ruling",
        "card_id TEXT REFERENCES Card(id), "
        "format_id INTEGER REFERENCES Format(name), "
        "ruling_id TEXT REFERENCES Ruling(ruling), "
        "PRIMARY KEY(card_id, format_id, ruling_id)");
    createTable("Card_FrameEffect",
        "card_id TEXT REFERENCES Card(id), "
        "type TEXT REFERENCES FrameEffect(type), "
        "PRIMARY KEY(card_id,type)");
    createTable("Card_Keyword",
        "card_id TEXT REFERENCES Card(id), "
        "name TEXT REFERENCES Keyword(name), "
        "PRIMARY KEY(card_id,name)");
    createTable("Card_ProducedMana",
        "card_id TEXT REFERENCES Card(id), "
        "color_id TEXT REFERENCES Color(id), "
        "PRIMARY KEY(card_id,color_id)");
    createTable("Card_PromoType",
        "card_id TEXT REFERENCES Card(id), "
        "promo_type TEXT REFERENCES PromoType(type), "
        "PRIMARY KEY(card_id,promo_type)");
    createTable("Card_RelatedCardObject",
        "card_id TEXT REFERENCES Card(id), "
        "relatedcardobject_id TEXT REFERENCES RelatedCardObject(id), "
        "PRIMARY KEY(card_id,relatedcardobject_id)");
    createTable("Face_Artist",
        "face_id INTEGER REFERENCES Face(id), "
        "artist_id TEXT REFERENCES Artist(id), "
        "PRIMARY KEY(face_id,artist_id)");
    createTable("Face_ColorIndicator",
        "face_id INTEGER REFERENCES Face(id), "
        "color_id TEXT REFERENCES Color(id), "
        "PRIMARY KEY(face_id,color_id)");
    createTable("Face_Colors",
        "face_id INTEGER REFERENCES Face(id), "
        "color_id TEXT REFERENCES Color(id), "
        "PRIMARY KEY(face_id,color_id)");
    return;
}

bool Database::validateAll(void) {
    std::array tables = { "Artist", "PromoType", "CardSet", "Card", "Face", "Color", "Finish", "FrameEffect", "Legality", "ImageURI", "Keyword", "MultiverseID", "RelatedCardObject" };
    for (std::string i : tables) {
        if (!validate(i))
            return false;
    }
    return true;
}

bool Database::validate(const std::string& tableName) {
    try {
        return connection->tableExists(tableName);
    } catch (std::exception& e) {
        SPDLOG_TRACE("{}", e.what());
        exit(EXIT_FAILURE);
    }
}
	
void Database::verifyNumOfSets(uint32_t numOfSets) {
	setsOutOfDate = numOfSets != 0;
	return;
}

void Database::verifyNumOfCards(uint32_t numOfCards) {
	cardsOutOfDate = numOfCards != 0;
	return;
}

bool Database::doSetsNeedUpdate() {
	return setsOutOfDate;
}

bool Database::doCardsNeedUpdate() {
	return cardsOutOfDate;
}

void Database::getTableCol(const std::string& tableName, const std::string& col) {
    try {
        SQLite::Statement stmt(*connection, "SELECT " + col + " FROM " + tableName);
        uint32_t colNum = stmt.getColumnIndex(col.c_str());
        while (stmt.executeStep()) {
            SPDLOG_TRACE("{}: {}", col, stmt.getColumn(colNum).getString());
        }
    } catch (std::exception& e) {
        SPDLOG_TRACE("{}", e.what());
        exit(EXIT_FAILURE);
    }
    return;
}

void Database::getTable(const std::string& tableName) {
    try {
        SQLite::Statement stmt(*connection, "SELECT * FROM " + tableName);
        uint32_t numCols = stmt.getColumnCount();
        std::vector<std::string> colNames;
        for (uint32_t i = 0; i < numCols; i++) {
            colNames.emplace_back(stmt.getColumnName(i));
        }
        while (stmt.executeStep()) {
            std::string row;
            for (uint32_t i = 0; i < numCols; i++) {
                SQLite::Column colVal = stmt.getColumn(i);
                if (colVal.isText() && colVal.getString() != "") {
                    row += colNames[i] + ": " + colVal.getString() + "| ";
                }
                else if (colVal.isInteger() && colVal.getInt() != 0) {
                    row += colNames[i] + ": " + std::to_string(colVal.getInt()) + "| ";
                }
                else if (colVal.isFloat() && colVal.getDouble() != 0.0) {
                    row += colNames[i] + ": " + std::to_string(colVal.getDouble()) + "| ";
                }
            }
            SPDLOG_TRACE("{}", row);
        }
    } catch (std::exception& e) {
        SPDLOG_TRACE("{}", e.what());
        exit(EXIT_FAILURE);
    }
    return;
}

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

void Database::bindOpt(SQLite::Statement& stmt, uint32_t ind, std::optional<std::string> val) {
    val.has_value() ? stmt.bind(ind, val.value()) : stmt.bind(ind);
    return;
}

void Database::bindOpt(SQLite::Statement& stmt, uint32_t ind, std::optional<uint32_t> val) {
	try {
    val.has_value() ? stmt.bind(ind, val.value()) : stmt.bind(ind);
    } catch (std::exception& e) {
        SPDLOG_TRACE("{}", e.what());
        exit(EXIT_FAILURE);
    }
    return;
}

void Database::bindOpt(SQLite::Statement& stmt, uint32_t ind, std::optional<bool> val) {
    val.has_value() ? stmt.bind(ind, val.value()) : stmt.bind(ind);
    return;
}

void Database::bindOpt(SQLite::Statement& stmt, uint32_t ind, std::optional<double> val) {
    val.has_value() ? stmt.bind(ind, val.value()) : stmt.bind(ind);
    return;
}

void Database::resetSql(SQLite::Statement& stmt) {
    try {
        stmt.reset();
        stmt.clearBindings();
    } catch (std::exception& e) {
        SPDLOG_TRACE("{}", e.what());
        exit(EXIT_FAILURE);
    }
    return;
}

void Database::executeSql(SQLite::Statement& stmt) {
    try {
        while (stmt.executeStep());
    } catch (std::exception& e) {
        SPDLOG_TRACE("{}", e.what());
        exit(EXIT_FAILURE);
    }
    return;
}

uint32_t Database::executeInsertGetInt(SQLite::Statement& stmt, uint32_t col) {
    uint32_t result;
    try {
        stmt.executeStep();
    	result = stmt.getColumn(col);
    } catch (std::exception& e) {
        SPDLOG_TRACE("{}", e.what());
        exit(EXIT_FAILURE);
    }
    return result;
}

void Database::insertArtist(const Artist& artist, const uint32_t faceId) {
    std::string sql = "INSERT OR IGNORE INTO Artist(id, name) VALUES (?,?);";
    SQLite::Statement stmt(*connection, sql);
    stmt.bind(1, artist.id);
    stmt.bind(2, artist.name);
    buildJunction("INSERT INTO Face_Artist(face_id,artist_id) VALUES (?,?);", faceId, artist.id);
    executeSql(stmt);
    return;
}

void Database::insertCardSet(const CardSet& cardSet) {
    std::string sql = "INSERT INTO CardSet(id,code,name,set_type,card_count,digital,foil_only,nonfoil_only,icon_svg_uri,mtgo_code,arena_code,tcgplayer_id,released_at,block_code,block,parent_set_code,printed_size) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?);";
    SQLite::Statement stmt(*connection, sql);
	stmt.bind(1, cardSet.id);
    stmt.bind(2, cardSet.code);
    stmt.bind(3, cardSet.name);
    stmt.bind(4, cardSet.set_type);
    stmt.bind(5, cardSet.card_count);
    stmt.bind(6, cardSet.digital);
    stmt.bind(7, cardSet.foil_only);
    stmt.bind(8, cardSet.nonfoil_only);
    stmt.bind(9, cardSet.icon_svg_uri);
    bindOpt(stmt, 10, cardSet.mtgo_code);
    bindOpt(stmt, 11, cardSet.arena_code);
    bindOpt(stmt, 12, cardSet.tcgplayer_id);
    bindOpt(stmt, 13, cardSet.released_at);
    bindOpt(stmt, 14, cardSet.block_code);
    bindOpt(stmt, 15, cardSet.block);
    bindOpt(stmt, 16, cardSet.parent_set_code);
    bindOpt(stmt, 17, cardSet.printed_size);
    executeSql(stmt);
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
    stmt.bind(1, card.id);
    stmt.bind(2, card.name);
    stmt.bind(3, card.lang);
    stmt.bind(4, card.layout);
    stmt.bind(5, card.rulings_uri);
    stmt.bind(6, card.scryfall_uri);
    stmt.bind(7, card.cmc);
    stmt.bind(8, card.reserved);
    stmt.bind(9, card.type_line);
    stmt.bind(10, card.booster);
    stmt.bind(11, card.border_color);
    stmt.bind(12, card.collector_number);
    stmt.bind(13, card.digital);
    stmt.bind(14, card.frame);
    stmt.bind(15, card.full_art);
    stmt.bind(16, card.highres_image);
    stmt.bind(17, card.image_status);
    stmt.bind(18, card.oversized);
    stmt.bind(19, card.promo);
    stmt.bind(20, card.rarity);
    stmt.bind(21, card.released_at);
    stmt.bind(22, card.reprint);
    stmt.bind(23, card.set_id);
    stmt.bind(24, card.story_spotlight);
    stmt.bind(25, card.textless);
    stmt.bind(26, card.variation);
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
    executeSql(stmt);
    for (const Color& c : card.color_identity) {
        insertColor(c);
        buildJunction("INSERT INTO Card_ColorIdentity(card_id,color_id) VALUES (?,?);", card.id, c.color);
    }
    for (const Color& c : card.produced_mana) {
        insertColor(c);
        buildJunction("INSERT INTO Card_ProducedMana(card_id,color_id) VALUES (?,?);", card.id, c.color);
    }
    for (const Face& face : card.card_faces) {
        insertFace(face, card.id);
    }
    for (const Finish& finish : card.finishes) {
        insertFinish(finish, card.id);
    }
    for (const Legality& legality : card.legalities) {
        insertLegality(legality, card.id);
    }
    for (const FrameEffect& frameEffect : card.frame_effects) {
        insertFrameEffect(frameEffect, card.id);
    }
    for (const Keyword& keyword : card.keywords) {
        insertKeyword(keyword, card.id);
    }
    for (const MultiverseID& multiverseId : card.multiverse_ids) {
        insertMultiverseID(multiverseId, card.id);
    }
    for (const PromoType& promoType : card.promo_types) {
        insertPromoType(promoType, card.id);
    }
    for (const RelatedCardObject& relatedCardObject : card.all_parts) {
        insertRelatedCardObject(relatedCardObject, card.id);
    }
	transaction.commit();
    return;
}

void Database::insertColor(const Color& color) {
    std::string sql = "INSERT OR IGNORE INTO Color(id) VALUES (?);";
    SQLite::Statement stmt(*connection, sql);
    stmt.bind(1, color.color);
    executeSql(stmt);
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
    stmt.bind(1, cardId);
    stmt.bind(2, face.mana_cost);
    stmt.bind(3, face.name);
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
    uint32_t id = executeInsertGetInt(stmt, 0);
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
    executeSql(stmt);
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
    executeSql(stmt);
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
    stmt.bind(1, imageUri.small);
    stmt.bind(2, imageUri.normal);
    stmt.bind(3, imageUri.large);
    stmt.bind(4, imageUri.png);
    stmt.bind(5, imageUri.art_crop);
    stmt.bind(6, imageUri.border_crop);
    return executeInsertGetInt(stmt, 0);
}

void Database::insertKeyword(const Keyword& keyword, const std::string& cardId) {
    std::string sql = "INSERT OR IGNORE INTO Keyword(name) VALUES (?);";
    SQLite::Statement stmt(*connection, sql);
    stmt.bind(1, keyword.name);
    buildJunction("INSERT INTO Card_Keyword(card_id,name) VALUES (?,?);", cardId, keyword.name);
    executeSql(stmt);
    return;
}

void Database::insertMultiverseID(const MultiverseID& multiverseId, const std::string& cardId) {
    std::string sql = "INSERT INTO MultiverseID(id,card_id) VALUES (?,?);";
    SQLite::Statement stmt(*connection, sql);
    stmt.bind(1, multiverseId.id);
    stmt.bind(2, cardId);
    executeSql(stmt);
    return;
}

void Database::insertPromoType(const PromoType& promoType, const std::string& cardId) {
    std::string sql = "INSERT OR IGNORE INTO PromoType(type) VALUES (?);";
    SQLite::Statement stmt(*connection, sql);
    stmt.bind(1, promoType.type);
    buildJunction("INSERT INTO Card_PromoType(card_id,promo_type) VALUES (?,?);", cardId, promoType.type);
    executeSql(stmt);
    return;
}

void Database::insertRelatedCardObject(const RelatedCardObject& relatedCardObject, const std::string& cardId) {
    std::string sql = "INSERT OR IGNORE INTO RelatedCardObject(id,component,name,type_line) VALUES (?,?,?,?);";
    SQLite::Statement stmt(*connection, sql);
    stmt.bind(1, relatedCardObject.id);
    stmt.bind(2, relatedCardObject.component);
    stmt.bind(3, relatedCardObject.name);
    stmt.bind(4, relatedCardObject.type_line);
    buildJunction("INSERT INTO Card_RelatedCardObject(card_id,relatedcardobject_id) VALUES (?,?);", cardId, relatedCardObject.id);
    executeSql(stmt);
    return;
}

void Database::insertRuling(const Legality& legality) {
    std::string sql = "INSERT OR IGNORE INTO Ruling(ruling) VALUES (?);";
    SQLite::Statement stmt(*connection, sql);
    stmt.bind(1, legality.ruling);
    executeSql(stmt);
    return;
}

template <typename T, typename U>
void Database::buildJunction(const std::string& sql, const T& firstPrimary, const U& secondPrimary) {
    SQLite::Statement stmt(*connection, sql);
    stmt.bind(1, firstPrimary);
    stmt.bind(2, secondPrimary);
    executeSql(stmt);
    return;
}

template <typename T, typename U, typename V>
void Database::buildJunction(const std::string& sql, const T& firstPrimary, const U& secondPrimary, const V& thirdPrimary) {
    SQLite::Statement stmt(*connection, sql);
    stmt.bind(1, firstPrimary);
    stmt.bind(2, secondPrimary);
    stmt.bind(3, thirdPrimary);
    executeSql(stmt);
    return;
}

// void Database::buildJunction(const std::string& sql, const std::string& firstPrimary, const std::string& secondPrimary) {
//     SQLite::Statement stmt(*this->connection, sql);
// 	stmt.bind(1, firstPrimary);
// 	stmt.bind(2, secondPrimary);
//     executeSql(stmt);
//     return;
// }

// void Database::buildJunction(const std::string& sql, const uint32_t firstPrimary, const std::string& secondPrimary) {
//     SQLite::Statement stmt(*this->connection, sql);
// 	stmt.bind(1, firstPrimary);
// 	stmt.bind(2, secondPrimary);
//     executeSql(stmt);
//     return;
// }

// void Database::buildJunction(const std::string& sql, const std::string& firstPrimary, const uint32_t secondPrimary) {
//     SQLite::Statement stmt(*this->connection, sql);
// 	stmt.bind(1, firstPrimary);
// 	stmt.bind(2, secondPrimary);
//     executeSql(stmt);
//     return;
// }

template <typename T>
std::optional<Card> getCardByCol(const std::string& colName, const T& colVal) {
}

/*
void Database::joinCard_ColorIdentity(const std::string& cardId, const std::string& colorIdentityId) {
    std::string sql = "INSERT INTO Card_ColorIdentity(card_id,color_id) VALUES (?,?);";
    SQLite::Statement stmt(*this->connection, sql);
        stmt.bind(1, cardId);
        stmt.bind(1, colorIdentityId);
    executeSql(stmt);
    return;
}

void Database::joinCard_Legality(const std::string& cardId, const uint32_t legalityId) {
    std::string sql = "INSERT INTO Card_Legality(card_id,legality_id) VALUES (?,?);";
    SQLite::Statement stmt(*this->connection, sql);
        stmt.bind(1, cardId);
        stmt.bind(1, legalityId);
    executeSql(stmt);
    return;
}

void Database::joinCard_Finish(const std::string& cardId, const std::string& finishType) {
    std::string sql = "INSERT INTO Card_Finish(card_id,type) VALUES (?,?);";
    SQLite::Statement stmt(*this->connection, sql);
        stmt.bind(1, cardId);
        stmt.bind(1, finishType);
    executeSql(stmt);
    return;
}

void Database::joinCard_FrameEffect(const std::string& cardId, const std::string& frameEffectType) {
    std::string sql = "INSERT INTO Card_FrameEffect(card_id,type) VALUES (?,?);";
    SQLite::Statement stmt(*this->connection, sql);
        stmt.bind(1, cardId);
        stmt.bind(1, frameEffectType);
    executeSql(stmt);
    return;
}

void Database::joinCard_Keyword(const std::string& cardId, const std::string& keywordName) {
    std::string sql = "INSERT INTO Card_Keyword(card_id,name) VALUES (?,?);";
    SQLite::Statement stmt(*this->connection, sql);
        stmt.bind(1, cardId);
        stmt.bind(1, keywordName);
    executeSql(stmt);
    return;
}

void Database::joinCard_ProducedMana(const std::string& cardId, const std::string& producedManaId) {
    std::string sql = "INSERT INTO Card_ProducedMana(card_id,color_id) VALUES (?,?);";
    SQLite::Statement stmt(*this->connection, sql);
        stmt.bind(1, cardId);
        stmt.bind(1, producedManaId);
    executeSql(stmt);
    return;
}

void Database::joinCard_PromoType(const std::string& cardId, const std::string& promoType) {
    std::string sql = "INSERT INTO Card_PromoType(card_id,promo_type) VALUES (?,?);";
    SQLite::Statement stmt(*this->connection, sql);
        stmt.bind(1, cardId);
        stmt.bind(1, promoType);
    executeSql(stmt);
    return;
}

void Database::joinCard_RelatedCardObject(const std::string& cardId, const std::string& relatedCardObjectId) {
    std::string sql = "INSERT INTO Card_RelatedCardObject(card_id,relatedcardobject_id) VALUES (?,?);";
    SQLite::Statement stmt(*this->connection, sql);
        stmt.bind(1, cardId);
        stmt.bind(1, relatedCardObjectId);
    executeSql(stmt);
    return;
}

void Database::joinFace_Artist(const uint32_t faceId, const std::string& artistId) {
    std::string sql = "INSERT INTO Face_Artist(face_id,artist_id) VALUES (?,?);";
    SQLite::Statement stmt(*this->connection, sql);
        stmt.bind(1, faceId);
        stmt.bind(1, artistId);
    executeSql(stmt);
    return;
}

void Database::joinFace_ColorIndicator(const uint32_t faceId, const std::string& colorIndicatorId) {
    std::string sql = "INSERT INTO Face_ColorIndicator(face_id,color_id) VALUES (?,?);";
    SQLite::Statement stmt(*this->connection, sql);
        stmt.bind(1, faceId);
        stmt.bind(1, colorIndicatorId);
    executeSql(stmt);
    return;
}

void Database::joinFace_Colors(const uint32_t faceId, const std::string& colorsId) {
    std::string sql = "INSERT INTO Face_Colors(face_id,color_id) VALUES (?,?);";
    SQLite::Statement stmt(*this->connection, sql);
        stmt.bind(1, faceId);
        stmt.bind(1, colorsId);
    executeSql(stmt);
    return;
}
*/