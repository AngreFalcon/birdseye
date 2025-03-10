#include "database.hpp"
#include <spdlog/spdlog.h>

Database::Database(const std::string& dbLoc) {
    connection = std::unique_ptr<SQLite::Database>(new SQLite::Database(dbLoc, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE));
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
    std::array tables = { "Artist", "PromoType", "CardSet", "Card", "Face", "Color", "Finish", "FrameEffect", "ImageURI", "Keyword", "MultiverseID", "RelatedCardObject" };
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
	setsOutOfDate = getNumRows("CardSet") != numOfSets;
	return;
}

void Database::verifyNumOfCards(uint32_t numOfCards) {
	cardsOutOfDate = getNumRows("Card") != numOfCards;
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

uint32_t Database::getNumRows(const std::string& tableName) {
    try {
		std::string sql = "SELECT COUNT(1) FROM " + tableName;
		SQLite::Statement stmt(*connection, sql);
		uint32_t numOfRecords = executeSQLGetInt(stmt, 0);
		SPDLOG_TRACE("{}: {}", sql, numOfRecords);
		return numOfRecords;
    } catch (std::exception& e) {
        SPDLOG_TRACE("{}", e.what());
        exit(EXIT_FAILURE);
    }
}void Database::bindOpt(SQLite::Statement& stmt, uint32_t ind, std::optional<std::string> val) {
    val.has_value() ? stmt.bind(ind, val.value()) : stmt.bind(ind);
    return;
}

void Database::bindOpt(SQLite::Statement& stmt, uint32_t ind, std::optional<uint32_t> val) {
    val.has_value() ? stmt.bind(ind, val.value()) : stmt.bind(ind);
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

void Database::executeSQL(SQLite::Statement& stmt) {
	try {
		stmt.exec();
    } catch (std::exception& e) {
        SPDLOG_TRACE("{}", e.what());
        exit(EXIT_FAILURE);
    }
    return;
}

uint32_t Database::executeSQLGetInt(SQLite::Statement& stmt, uint32_t col) {
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

template <typename T>
std::optional<Card> getCardByCol(const std::string& colName, const T& colVal) {
}
