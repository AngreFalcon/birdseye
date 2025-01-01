#include "database.hpp"
#include "download.hpp"
#include <chrono>
#include <spdlog/spdlog.h>
#include <thread>

void Database::initialize() {
    std::string fileName = "./birdseye.db3";
    if (std::filesystem::exists(fileName))
        return;
    sqlite3_open(fileName.c_str(), &this->connection);
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
        "icon_svg_uri TEXT, "
		"scryfall_uri TEXT");
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
    createTable("Legality",
        "id INTEGER PRIMARY KEY, "
        "format TEXT, "
        "ruling TEXT");
    createTable("ImageURI",
        "id TEXT PRIMARY KEY, "
        "face_id TEXT REFERENCES Face(id), "
        "small_uri TEXT, "
        "normal_uri TEXT, "
        "large_uri TEXT, "
        "png_uri TEXT, "
        "art_crop_uri TEXT, "
        "border_crop_uri TEXT");
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
    createTable("Card_ColorIdentity",
        "card_id TEXT REFERENCES Card(id), "
        "color_id TEXT REFERENCES Color(id), "
		"PRIMARY KEY(card_id,color_id)");
    createTable("Card_Legality",
        "card_id TEXT REFERENCES Card(id), "
        "legality_id INTEGER REFERENCES Legality(id), "
		"PRIMARY KEY(card_id,legality_id)");
    createTable("Card_Finish",
        "card_id TEXT REFERENCES Card(id), "
        "type TEXT REFERENCES Finish(type), "
		"PRIMARY KEY(card_id,type)");
    createTable("Card_FrameEffect",//
        "card_id TEXT REFERENCES Card(id), "
        "type TEXT REFERENCES FrameEffect(type), "
		"PRIMARY KEY(card_id,type)");
    createTable("Card_Keyword",//
        "card_id TEXT REFERENCES Card(id), "
        "name TEXT REFERENCES Keyword(name), "
		"PRIMARY KEY(card_id,name)");
    createTable("Card_ProducedMana",
        "card_id TEXT REFERENCES Card(id), "
        "color_id TEXT REFERENCES Color(id), "
		"PRIMARY KEY(card_id,color_id)");
    createTable("Card_PromoType",//
        "card_id TEXT REFERENCES Card(id), "
        "promo_type TEXT REFERENCES PromoType(type), "
		"PRIMARY KEY(card_id,promo_type)");
    createTable("Card_RelatedCardObject",//
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
    populateDB();
    return;
}

bool Database::validate(const std::string& tableName) {
    sqlite3_open("./birdseye.db3", &this->connection);
    sqlite3_stmt* stmt;
    std::string sql = "SELECT 1 FROM sqlite_master where type='table' and name=?";
    bool result;
    int rc = sqlite3_prepare_v2(this->connection, sql.c_str(), static_cast<int>(sql.size()), &stmt, NULL);
    if (rc != SQLITE_OK) {
        SPDLOG_TRACE("error: {}\n", sqlite3_errmsg(this->connection));
        exit(EXIT_FAILURE);
    }
    sqlite3_bind_text(stmt, 1, tableName.c_str(), static_cast<int>(tableName.length()), SQLITE_STATIC);
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        result = true;
    }
    else if (rc == SQLITE_DONE) {
        result = false;
    }
    else {
        SPDLOG_TRACE("error: {}\n", sqlite3_errmsg(this->connection));
        exit(EXIT_FAILURE);
    }
    sqlite3_finalize(stmt);
    return result;
}

void Database::createTable(const std::string& tableName, const std::string& fields) {
    char* err;
    std::string sql = "CREATE TABLE " + tableName + " (" + fields + ");";
    int code = sqlite3_exec(this->connection, sql.c_str(), NULL, NULL, &err);
    if (code != SQLITE_OK) {
        SPDLOG_TRACE("Error creating table {}\nMessage: {}\nExiting program\n", tableName, err);
        sqlite3_free(err);
        exit(EXIT_FAILURE);
    }
    return;
}

time_t* Database::stringToTime(const std::string& date) {
    std::istringstream dateStream;
    dateStream.str() = date;
    tm tm = {};
    dateStream >> std::get_time(&tm, "%Y-%m-%d");
    time_t time = mktime(&tm);
    return &time;
}

std::vector<std::string> Database::separateArtistNames(const std::string& names) {
	std::vector<std::string> artistNames = { "" };
	for (int i = 0; i < names.size(); i++) {
		if (names[i] == '&') {
			artistNames.emplace_back();
		}
		else if (i > 0 && names[i] == ' ' && names[i-1] == '&') {
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

void Database::prepareSql(sqlite3_stmt* stmt, const std::string& sql) {
    sqlite3_open("./birdseye.db3", &this->connection);
	uint32_t rc = sqlite3_prepare_v2(this->connection, sql.c_str(), static_cast<int>(sql.size()), &stmt, NULL);
    if (rc != SQLITE_OK) {
        SPDLOG_TRACE("error: {}\n", sqlite3_errmsg(this->connection));
        exit(EXIT_FAILURE);
    }
    return;
}

void Database::bindSql(sqlite3_stmt* stmt, uint32_t i, const std::string& data) {
	SPDLOG_TRACE("bind text, index: {}", i);
    SPDLOG_TRACE("data: {}|", data.c_str(), i);
	uint32_t rc = sqlite3_bind_text(stmt, i, data.c_str(), -1, SQLITE_STATIC);
	if (rc != SQLITE_OK) {
        SPDLOG_TRACE("error: {}\n", sqlite3_errmsg(this->connection));
        exit(EXIT_FAILURE);
    }
    return;
}

void Database::bindSql(sqlite3_stmt* stmt, uint32_t i, const uint32_t data) {
	SPDLOG_TRACE("bind int, index: {}", i);
    SPDLOG_TRACE("data: {}|", data, i);
    uint32_t rc = sqlite3_bind_int(stmt, i, data);
    if (rc != SQLITE_OK) {
        SPDLOG_TRACE("error: {}\n", sqlite3_errmsg(this->connection));
        exit(EXIT_FAILURE);
    }
    return;
}

void Database::bindSql(sqlite3_stmt* stmt, uint32_t i, const void* data, uint32_t size) {
	SPDLOG_TRACE("bind blob, index: {}", i);
    SPDLOG_TRACE("data: {}|", data, i);
    uint32_t rc = sqlite3_bind_blob(stmt, 8, data, size, SQLITE_STATIC);
    if (rc != SQLITE_OK) {
        SPDLOG_TRACE("error: {}\n", sqlite3_errmsg(this->connection));
        exit(EXIT_FAILURE);
    }
    return;
}

void Database::bindSql(sqlite3_stmt* stmt, uint32_t i, const double data) {
	SPDLOG_TRACE("bind double, index: {}", i);
    SPDLOG_TRACE("data: {}|", data, i);
    uint32_t rc = sqlite3_bind_double(stmt, i, data);
    if (rc != SQLITE_OK) {
        SPDLOG_TRACE("error: {}\n", sqlite3_errmsg(this->connection));
        exit(EXIT_FAILURE);
    }
    return;
}

uint32_t Database::resetSql(sqlite3_stmt* stmt) {
	uint32_t rc;
    do {
        rc = sqlite3_step(stmt);
        if (rc == SQLITE_ERROR) {
            SPDLOG_TRACE("error: {}\n", sqlite3_errmsg(this->connection));
            exit(EXIT_FAILURE);
        }
    } while (rc == SQLITE_ROW);
	uint32_t faceId = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);
	return faceId;
}

void Database::finalizeSql(sqlite3_stmt* stmt) {
	uint32_t rc;
    do {
        rc = sqlite3_step(stmt);
        if (rc == SQLITE_ERROR) {
            SPDLOG_TRACE("error: {}\n", sqlite3_errmsg(this->connection));
            exit(EXIT_FAILURE);
        }
    } while (rc == SQLITE_ROW);
    sqlite3_finalize(stmt);
	return;
}

uint32_t Database::finalizeSqlRetInt(sqlite3_stmt* stmt, uint32_t col) {
	uint32_t rc;
    do {
        rc = sqlite3_step(stmt);
        if (rc == SQLITE_ERROR) {
            SPDLOG_TRACE("error: {}\n", sqlite3_errmsg(this->connection));
            exit(EXIT_FAILURE);
        }
    } while (rc == SQLITE_ROW);
	uint32_t faceId = sqlite3_column_int(stmt, col);
    sqlite3_finalize(stmt);
	return faceId;
}

void Database::populateDB() {
    retrieveSets();
    //retrieveCards();
    return;
}

void Database::retrieveSets() {
    std::string rawJSON = downloadData("https://api.scryfall.com/sets");
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // ensure we respect scryfall's API guidelines by waiting at least 100 milliseconds before we can possibly initiate another connection
    json j = json::parse(rawJSON, nullptr, false);
    uint64_t it = 0;
    for (const auto& i : j["data"].items()) {
        insertCardSet(i.value());
    }
    return;
}

void Database::retrieveCards() {
    //getBulkDownload("https://api.scryfall.com/bulk-data/oracle-cards", "./all-cards.json");
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // ensure we respect scryfall's API guidelines by waiting at least 100 milliseconds before we can possibly initiate another connection
    return;
}

void Database::insertArtistFromCard(const json& data, const std::string& faceId) {
    sqlite3_open("./birdseye.db3", &this->connection);
    sqlite3_stmt* stmt;
    std::string sql = "INSERT INTO Artist(id, name) VALUES (?,?);";
	size_t e = 0;
	std::vector<std::string> artistNames = separateArtistNames(data["artist"]);
	for (auto& i : data["artist_ids"].items()) {
    	prepareSql(stmt, sql);
    	bindSql(stmt, 1, static_cast<std::string>(i.value()));
    	bindSql(stmt, 2, artistNames[e++]);
		joinFace_Artist(stoi(faceId), i.value());
		resetSql(stmt);
	}
    sqlite3_finalize(stmt);
    return;
}

void Database::insertArtistFromFace(const json& data, const std::string& faceId) {
    sqlite3_open("./birdseye.db3", &this->connection);
    sqlite3_stmt* stmt;
    std::string sql = "INSERT INTO Artist(id, name) VALUES (?,?);";
    prepareSql(stmt, sql);
    bindSql(stmt, 1, static_cast<std::string>(data["artist_id"]));
    bindSql(stmt, 2, static_cast<std::string>(data["artist"]));
	joinFace_Artist(stoi(faceId), static_cast<std::string>(data["artist_id"]));
	finalizeSql(stmt);
    return;
}

void Database::insertCardSet(const json& data) {
    sqlite3_open("./birdseye.db3", &this->connection);
    sqlite3_stmt* stmt;
    std::string sql = "INSERT INTO CardSet(id,code,mtgo_code,arena_code,tcgplayer_id,name,set_type,released_at,block_code,block,parent_set_code,card_count,printed_size,digital,foil_only,nonfoil_only,scryfall_uri,icon_svg_uri) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?);";
    prepareSql(stmt, sql);
	bindSql(stmt, 1, static_cast<std::string>(data["id"]));
    bindSql(stmt, 2, static_cast<std::string>(data["code"]));
    bindSql(stmt, 3, data.value("mtgo_code", "null"));
    bindSql(stmt, 4, data.value("arena_code", "null"));
    bindSql(stmt, 5, static_cast<uint32_t>(data.value("tcgplayer_id", 0)));
    bindSql(stmt, 6, static_cast<std::string>(data["name"]));
    bindSql(stmt, 7, static_cast<std::string>(data["set_type"]));
    bindSql(stmt, 8, stringToTime(data["released_at"]), sizeof(time_t));
    bindSql(stmt, 9, data.value("block_code", "null"));
    bindSql(stmt, 10, data.value("block", "null"));
    bindSql(stmt, 11, data.value("parent_set_code", "null"));
    bindSql(stmt, 12, static_cast<uint32_t>(data["card_count"]));
    bindSql(stmt, 13, static_cast<uint32_t>(data.value("printed_size", 0)));
    bindSql(stmt, 14, static_cast<uint32_t>(data["digital"]));
    bindSql(stmt, 15, static_cast<uint32_t>(data["foil_only"]));
    bindSql(stmt, 16, static_cast<uint32_t>(data["nonfoil_only"]));
	bindSql(stmt, 17, static_cast<std::string>(data["scryfall_uri"]));
    bindSql(stmt, 18, static_cast<std::string>(data["icon_svg_uri"]));
	finalizeSql(stmt);
    return;
}

void Database::insertCard(const json& data) {
    sqlite3_open("./birdseye.db3", &this->connection);
    sqlite3_stmt* stmt;
    std::string sql = "INSERT INTO Card(arena_id,id,lang,mtgo_id,mtgo_foil_id,tcgplayer_id,tcgplayer_etched_id,cardmarket_id,layout,oracle_id,rulings_uri,scryfall_uri,cmc,defense,hand_modifier,life_modifier,loyalty,name,reserved,type_line,booster,border_color,card_back_id,collector_number,content_warning,digital,flavor_name,frame,full_art,highres_image,image_status,oversized,printed_name,printed_text,printed_type_line,promo,rarity,released_at,reprint,set_id,story_spotlight,textless,variation,variation_of,security_stamp) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?);";
    prepareSql(stmt, sql);
	std::string cardId = static_cast<std::string>(data["id"]);
	//core fields
	bindSql(stmt, 1, static_cast<uint32_t>(data.value("arena_id", 0)));
    bindSql(stmt, 2, cardId);
	bindSql(stmt, 3, static_cast<std::string>(data["lang"]));
	bindSql(stmt, 4, static_cast<uint32_t>(data.value("mtgo_id", 0)));
    bindSql(stmt, 5, static_cast<uint32_t>(data.value("mtgo_foil_id", 0)));
    bindSql(stmt, 6, static_cast<uint32_t>(data.value("tcgplayer_id", 0)));
    bindSql(stmt, 7, static_cast<uint32_t>(data.value("tcgplayer_etched_id", 0)));
    bindSql(stmt, 8, static_cast<uint32_t>(data.value("cardmarket_id", 0)));
    bindSql(stmt, 9, static_cast<std::string>(data["layout"]));
    bindSql(stmt, 10, data.value("oracle_id", "null"));
    bindSql(stmt, 11, static_cast<std::string>(data["rulings_uri"]));
    bindSql(stmt, 12, static_cast<std::string>(data["scryfall_uri"]));
	//gameplay fields
    bindSql(stmt, 13, static_cast<double>(data["cmc"]));
    bindSql(stmt, 14, data.value("defense", "null"));
    bindSql(stmt, 15, data.value("hand_modifier", "null"));
    bindSql(stmt, 16, data.value("life_modifier", "null"));
    bindSql(stmt, 17, data.value("loyalty", "null"));
    bindSql(stmt, 18, static_cast<std::string>(data["name"]));
    bindSql(stmt, 19, static_cast<uint32_t>(data["reserved"]));
    bindSql(stmt, 20, static_cast<std::string>(data["type_line"]));
	//print fields
    bindSql(stmt, 21, static_cast<uint32_t>(data["booster"]));
    bindSql(stmt, 22, static_cast<std::string>(data["border_color"]));
    bindSql(stmt, 23, static_cast<std::string>(data["card_back_id"]));
    bindSql(stmt, 24, static_cast<std::string>(data["collector_number"]));
    bindSql(stmt, 25, data.value("content_warning", "0"));
    bindSql(stmt, 26, static_cast<uint32_t>(data["digital"]));
    bindSql(stmt, 27, data.value("flavor_name", "null"));
    bindSql(stmt, 28, static_cast<std::string>(data["frame"]));
    bindSql(stmt, 29, static_cast<uint32_t>(data["full_art"]));
    bindSql(stmt, 30, static_cast<uint32_t>(data["highres_image"]));
    bindSql(stmt, 31, static_cast<std::string>(data["image_status"]));
    bindSql(stmt, 32, static_cast<uint32_t>(data["oversized"]));
    bindSql(stmt, 33, data.value("printed_name", "null"));
    bindSql(stmt, 34, data.value("printed_text", "null"));
    bindSql(stmt, 35, data.value("printed_type_line", "null"));
    bindSql(stmt, 36, static_cast<uint32_t>(data["promo"]));
    bindSql(stmt, 37, static_cast<std::string>(data["rarity"]));
    bindSql(stmt, 38, stringToTime(data["released_at"]), sizeof(time_t));
    bindSql(stmt, 39, static_cast<uint32_t>(data["reprint"]));
	bindSql(stmt, 40, static_cast<std::string>(data["set_id"]));
    bindSql(stmt, 41, static_cast<uint32_t>(data["story_spotlight"]));
    bindSql(stmt, 42, static_cast<uint32_t>(data["textless"]));
    bindSql(stmt, 43, static_cast<uint32_t>(data["variation"]));
    bindSql(stmt, 44, data.value("variation_of", "null"));
    bindSql(stmt, 45, data.value("security_stamp", "null"));
    finalizeSql(stmt);
	insertColor(data["color_identity"], cardId, 1);
	if (data["produced_mana"] != NULL) insertColor(data["produced_mana"], cardId, 2);
	if (data["card_faces"] == "null") {
		std::string faceId = insertFace(data, cardId);
		insertArtistFromCard(data, faceId);
	}
	else {
		for (auto& i : data["card_faces"].items()) {
			std::string faceId = insertFace(i.value(), cardId);
			insertArtistFromFace(i.value(), faceId);
		}
	}
	for (auto& i : data["finishes"].items()) {
		insertFinish(i.value(), cardId);
	}
	for (auto& i : data["legalities"].items()) {
		insertLegality(i.key(), i.value(), cardId);
	}
	if (data["frame_effects"] != NULL) {
		for (auto& i : data["frame_effects"].items()) {
			insertFrameEffect(i.value(), cardId);
		}
	}
	for (auto& i : data["keywords"].items()) {
		insertKeyword(i.value(), cardId);
	}
	if (data["multiverse_ids"] != NULL) {
		for (auto& i : data["multiverse_ids"].items()) {
			insertMultiverseID(i.value(), cardId);
		}
	}
	if (data["promo_types"] != NULL) {
		for (auto& i : data["promo_types"].items()) {
			insertPromoType(i.value(), cardId);
		}
	}
	if (data["all_parts"] != NULL) {
		for (auto& i : data["all_parts"].items()) {
			insertRelatedCardObject(i.value(), cardId);
		}
	}
    return;
}

void Database::colorJoin(const std::string& baseId, const std::string& colorId, const uint8_t funcInd) {
	switch (funcInd) {
	case 1:
		joinCard_ColorIdentity(baseId, colorId);
		break;
	case 2:
		joinCard_ProducedMana(baseId, colorId);
		break;
	case 3:
		joinFace_ColorIndicator(stoi(baseId), colorId);
		break;
	case 4:
		joinFace_Colors(stoi(baseId), colorId);
		break;
	}
	return;
}

void Database::insertColor(const json& dataArray, const std::string& id, const uint8_t funcInd) {
    sqlite3_open("./birdseye.db3", &this->connection);
    sqlite3_stmt* stmt;
    std::string sql = "INSERT INTO Color(id) VALUES (?);";
	for (auto& i : dataArray.items()) {
    	prepareSql(stmt, sql);
		bindSql(stmt, 1, static_cast<std::string>(i.value()));
		colorJoin(id, static_cast<std::string>(i.value()), funcInd);
    	resetSql(stmt);
	}
    sqlite3_finalize(stmt);
    return;
}

std::string Database::insertFace(const json& data, const std::string& cardId) {
    sqlite3_open("./birdseye.db3", &this->connection);
    sqlite3_stmt* stmt;
    std::string sql = "INSERT INTO Face(card_id,cmc,defense,flavor_text,illustration_id,layout,loyalty,mana_cost,name,oracle_id,oracle_text,power,printed_name,printed_type_line,toughness,type_line,watermark) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?);";
    prepareSql(stmt, sql);
	bindSql(stmt, 1, cardId);
	bindSql(stmt, 2, static_cast<double>(data.value("cmc", 0.0)));
    bindSql(stmt, 3, data.value("defense", "null"));
    bindSql(stmt, 4, data.value("flavor_text", "null"));
    bindSql(stmt, 5, data.value("illustration_id", "null"));
    bindSql(stmt, 6, data.value("layout", "null"));
    bindSql(stmt, 7, data.value("loyalty", "null"));
    bindSql(stmt, 8, static_cast<std::string>(data["mana_cost"]));
    bindSql(stmt, 9, static_cast<std::string>(data["name"]));
    bindSql(stmt, 10, data.value("oracle_id", "null"));
    bindSql(stmt, 11, data.value("oracle_text", "null"));
    bindSql(stmt, 12, data.value("power", "null"));
    bindSql(stmt, 13, data.value("printed_name", "null"));
    bindSql(stmt, 14, data.value("printed_type_line", "null"));
    bindSql(stmt, 15, data.value("toughness", "null"));
    bindSql(stmt, 16, data.value("type_line", "value"));
    bindSql(stmt, 17, data.value("watermark", "null"));
	std::string id = std::to_string(finalizeSqlRetInt(stmt, 0));
	if (data["color_indicator"] != NULL) insertColor(data["color_indicator"], id, 3);
	if (data["colors"] != NULL) insertColor(data["colors"], id, 4);
	if (data["image_uris"] != NULL) insertImageURI(data["image_uris"], id);
	return id;
}

void Database::insertFinish(const std::string& data, const std::string& cardId) {
    sqlite3_open("./birdseye.db3", &this->connection);
    sqlite3_stmt* stmt;
    std::string sql = "INSERT INTO Finish(type) VALUES (?);";
    prepareSql(stmt, sql);
	bindSql(stmt, 1, data);
	joinCard_Finish(cardId, data);
    finalizeSql(stmt);
    return;
}

void Database::insertFrameEffect(const std::string& data, const std::string& cardId) {
    sqlite3_open("./birdseye.db3", &this->connection);
    sqlite3_stmt* stmt;
    std::string sql = "INSERT INTO FrameEffect(type) VALUES (?);";
    prepareSql(stmt, sql);
	bindSql(stmt, 1, data);
	joinCard_FrameEffect(cardId, data);
    finalizeSql(stmt);
    return;
}

void Database::insertLegality(const std::string& format, const std::string& ruling, const std::string& cardId) {
    sqlite3_open("./birdseye.db3", &this->connection);
    sqlite3_stmt* stmt;
    std::string sql = "INSERT INTO Legality(format,ruling) VALUES (?,?);";
    prepareSql(stmt, sql);
    bindSql(stmt, 1, format);
    bindSql(stmt, 2, ruling);
	uint32_t id = finalizeSqlRetInt(stmt, 0);
	joinCard_Legality(cardId, id);
    return;
}

void Database::insertImageURI(const json& data, const std::string& faceId) {
    sqlite3_open("./birdseye.db3", &this->connection);
    sqlite3_stmt* stmt;
    std::string sql = "INSERT INTO ImageURI(face_id,small_uri,normal_uri,large_uri,png_uri,art_crop_uri,border_crop_uri) VALUES (?,?,?,?,?,?,?);";
    prepareSql(stmt, sql);
	bindSql(stmt, 1, faceId);
    bindSql(stmt, 2, static_cast<std::string>(data["small_uri"]));
    bindSql(stmt, 3, static_cast<std::string>(data["normal_uri"]));
    bindSql(stmt, 4, static_cast<std::string>(data["large_uri"]));
    bindSql(stmt, 5, static_cast<std::string>(data["png_uri"]));
    bindSql(stmt, 6, static_cast<std::string>(data["art_crop_uri"]));
    bindSql(stmt, 7, static_cast<std::string>(data["border_crop_uri"]));
    finalizeSql(stmt);
    return;
}

void Database::insertKeyword(const std::string& data, const std::string& cardId) {
    sqlite3_open("./birdseye.db3", &this->connection);
    sqlite3_stmt* stmt;
    std::string sql = "INSERT INTO Keyword(name) VALUES (?);";
    prepareSql(stmt, sql);
	bindSql(stmt, 1, data);
	joinCard_Keyword(cardId, data);
    finalizeSql(stmt);
    return;
}

void Database::insertMultiverseID(const std::string& data, const std::string& cardId) {
    sqlite3_open("./birdseye.db3", &this->connection);
    sqlite3_stmt* stmt;
    std::string sql = "INSERT INTO MultiverseID(id,card_id) VALUES (?,?);";
    prepareSql(stmt, sql);
	bindSql(stmt, 1, data);
	bindSql(stmt, 1, cardId);
    finalizeSql(stmt);
    return;
}

void Database::insertPromoType(const std::string& data, const std::string& cardId) {
    sqlite3_open("./birdseye.db3", &this->connection);
    sqlite3_stmt* stmt;
    std::string sql = "INSERT INTO PromoType(type) VALUES (?);";
    prepareSql(stmt, sql);
	bindSql(stmt, 1, data);
	joinCard_PromoType(cardId, data);
    finalizeSql(stmt);
    return;
}

void Database::insertRelatedCardObject(const json& data, const std::string& cardId) {
    sqlite3_open("./birdseye.db3", &this->connection);
    sqlite3_stmt* stmt;
    std::string sql = "INSERT INTO RelatedCardObject(id,component,name,type_line) VALUES (?,?,?,?);";
    prepareSql(stmt, sql);
	bindSql(stmt, 1, static_cast<std::string>(data["id"]));
    bindSql(stmt, 2, static_cast<std::string>(data["component"]));
    bindSql(stmt, 2, static_cast<std::string>(data["name"]));
    bindSql(stmt, 2, static_cast<std::string>(data["type_line"]));
	joinCard_RelatedCardObject(cardId, static_cast<std::string>(data["id"]));
    finalizeSql(stmt);
    return;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Database::joinCard_ColorIdentity(const std::string& cardId, const std::string& colorIdentityId) {
    sqlite3_open("./birdseye.db3", &this->connection);
    sqlite3_stmt* stmt;
    std::string sql = "INSERT INTO Card_ColorIdentity(card_id,color_id) VALUES (?,?);";
    prepareSql(stmt, sql);
	bindSql(stmt, 1, cardId);
	bindSql(stmt, 1, colorIdentityId);
    finalizeSql(stmt);
    return;
}

void Database::joinCard_Legality(const std::string& cardId, const uint32_t legalityId) {
    sqlite3_open("./birdseye.db3", &this->connection);
    sqlite3_stmt* stmt;
    std::string sql = "INSERT INTO Card_Legality(card_id,legality_id) VALUES (?,?);";
    prepareSql(stmt, sql);
	bindSql(stmt, 1, cardId);
	bindSql(stmt, 1, legalityId);
    finalizeSql(stmt);
    return;
}

void Database::joinCard_Finish(const std::string& cardId, const std::string& finishType) {
    sqlite3_open("./birdseye.db3", &this->connection);
    sqlite3_stmt* stmt;
    std::string sql = "INSERT INTO Card_Finish(card_id,type) VALUES (?,?);";
    prepareSql(stmt, sql);
	bindSql(stmt, 1, cardId);
	bindSql(stmt, 1, finishType);
    finalizeSql(stmt);
    return;
}

void Database::joinCard_FrameEffect(const std::string& cardId, const std::string& frameEffectType) {
    sqlite3_open("./birdseye.db3", &this->connection);
    sqlite3_stmt* stmt;
    std::string sql = "INSERT INTO Card_FrameEffect(card_id,type) VALUES (?,?);";
    prepareSql(stmt, sql);
	bindSql(stmt, 1, cardId);
	bindSql(stmt, 1, frameEffectType);
    finalizeSql(stmt);
    return;
}

void Database::joinCard_Keyword(const std::string& cardId, const std::string& keywordName) {
    sqlite3_open("./birdseye.db3", &this->connection);
    sqlite3_stmt* stmt;
    std::string sql = "INSERT INTO Card_Keyword(card_id,name) VALUES (?,?);";
    prepareSql(stmt, sql);
	bindSql(stmt, 1, cardId);
	bindSql(stmt, 1, keywordName);
    finalizeSql(stmt);
    return;
}

void Database::joinCard_ProducedMana(const std::string& cardId, const std::string& producedManaId) {
    sqlite3_open("./birdseye.db3", &this->connection);
    sqlite3_stmt* stmt;
    std::string sql = "INSERT INTO Card_ProducedMana(card_id,color_id) VALUES (?,?);";
    prepareSql(stmt, sql);
	bindSql(stmt, 1, cardId);
	bindSql(stmt, 1, producedManaId);
    finalizeSql(stmt);
    return;
}

void Database::joinCard_PromoType(const std::string& cardId, const std::string& promoType) {
    sqlite3_open("./birdseye.db3", &this->connection);
    sqlite3_stmt* stmt;
    std::string sql = "INSERT INTO Card_PromoType(card_id,promo_type) VALUES (?,?);";
    prepareSql(stmt, sql);
	bindSql(stmt, 1, cardId);
	bindSql(stmt, 1, promoType);
    finalizeSql(stmt);
    return;
}

void Database::joinCard_RelatedCardObject(const std::string& cardId, const std::string& relatedCardObjectId) {
    sqlite3_open("./birdseye.db3", &this->connection);
    sqlite3_stmt* stmt;
    std::string sql = "INSERT INTO Card_RelatedCardObject(card_id,relatedcardobject_id) VALUES (?,?);";
    prepareSql(stmt, sql);
	bindSql(stmt, 1, cardId);
	bindSql(stmt, 1, relatedCardObjectId);
    finalizeSql(stmt);
    return;
}

void Database::joinFace_Artist(const uint32_t faceId, const std::string& artistId) {
    sqlite3_open("./birdseye.db3", &this->connection);
    sqlite3_stmt* stmt;
    std::string sql = "INSERT INTO Face_Artist(face_id,artist_id) VALUES (?,?);";
    prepareSql(stmt, sql);
	bindSql(stmt, 1, faceId);
	bindSql(stmt, 1, artistId);
    finalizeSql(stmt);
    return;
}

void Database::joinFace_ColorIndicator(const uint32_t faceId, const std::string& colorIndicatorId) {
    sqlite3_open("./birdseye.db3", &this->connection);
    sqlite3_stmt* stmt;
    std::string sql = "INSERT INTO Face_ColorIndicator(face_id,color_id) VALUES (?,?);";
    prepareSql(stmt, sql);
	bindSql(stmt, 1, faceId);
	bindSql(stmt, 1, colorIndicatorId);
    finalizeSql(stmt);
    return;
}

void Database::joinFace_Colors(const uint32_t faceId, const std::string& colorsId) {
    sqlite3_open("./birdseye.db3", &this->connection);
    sqlite3_stmt* stmt;
    std::string sql = "INSERT INTO Face_Colors(face_id,color_id) VALUES (?,?);";
    prepareSql(stmt, sql);
	bindSql(stmt, 1, faceId);
	bindSql(stmt, 1, colorsId);
    finalizeSql(stmt);
    return;
}
