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
        "edhrec_rank INTEGER, "
        "hand_modifier TEXT, "
        "life_modifier TEXT, "
        "loyalty TEXT, "
        "name TEXT, "
        "penny_rank INTEGER, "
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
        "id TEXT PRIMARY KEY, "
        "card_id TEXT REFERENCES Card(id), "
        "artist_id TEXT REFERENCES Artist(id), "
        "cmc REAL, "
        "color_indicator TEXT, "
        "colors TEXT, "
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
        "id TEXT PRIMARY KEY, "
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
        "object TEXT, "
        "component TEXT, "
        "name TEXT, "
        "type_line TEXT");
    createTable("Card_ColorIdentity",
        "card_id TEXT REFERENCES Card(id), "
        "color_id TEXT REFERENCES Color(id), "
		"PRIMARY KEY(card_id,color_id)");
    createTable("Card_ColorIndicator",
        "card_id TEXT REFERENCES Card(id), "
        "color_id TEXT REFERENCES Color(id), "
		"PRIMARY KEY(card_id,color_id)");
    createTable("Card_Legality",
        "card_id TEXT REFERENCES Card(id), "
        "legality_id TEXT REFERENCES Legality(id), "
		"PRIMARY KEY(card_id,legality_id)");
    createTable("Card_ProducedMana",
        "card_id TEXT REFERENCES Card(id), "
        "color_id TEXT REFERENCES Color(id), "
		"PRIMARY KEY(card_id,color_id)");
    createTable("Card_Finish",
        "card_id TEXT REFERENCES Card(id), "
        "type TEXT REFERENCES Finish(type), "
		"PRIMARY KEY(card_id,type)");
    createTable("Card_FrameEffect",
        "card_id TEXT REFERENCES Card(id), "
        "type TEXT REFERENCES FrameEffect(type), "
		"PRIMARY KEY(card_id,type)");
    createTable("Card_Keyword",
        "card_id TEXT REFERENCES Card(id), "
        "name TEXT REFERENCES Keyword(name), "
		"PRIMARY KEY(card_id,name)");
    createTable("Card_PromoType",
        "card_id TEXT REFERENCES Card(id), "
        "promo_type TEXT REFERENCES PromoType(type), "
		"PRIMARY KEY(card_id,promo_type)");
    createTable("Card_RelatedCardObject",
        "card_id TEXT REFERENCES Card(id), "
        "relatedcardobject_id TEXT REFERENCES RelatedCardObject(id), "
		"PRIMARY KEY(card_id,relatedcardobject_id)");
    createTable("Face_Artist",
        "face_id TEXT REFERENCES Face(id), "
        "artist_id TEXT REFERENCES Artist(id), "
		"PRIMARY KEY(face_id,artist_id)");
    createTable("Face_ColorIndicator",
        "face_id TEXT REFERENCES Face(id), "
        "color_id TEXT REFERENCES Color(id), "
		"PRIMARY KEY(face_id,color_id)");
    createTable("Face_Colors",
        "face_id TEXT REFERENCES Face(id), "
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

void Database::bindSql(sqlite3_stmt* stmt, uint32_t i, const std::string& data) {
    uint32_t rc = sqlite3_bind_text(stmt, i, data.c_str(), static_cast<int>(data.length()), SQLITE_STATIC);
    if (rc != SQLITE_OK) {
        SPDLOG_TRACE("error: {}\n", sqlite3_errmsg(this->connection));
        exit(EXIT_FAILURE);
    }
    return;
}

void Database::bindSql(sqlite3_stmt* stmt, uint32_t i, const uint32_t data) {
    uint32_t rc = sqlite3_bind_int(stmt, i, data);
    if (rc != SQLITE_OK) {
        SPDLOG_TRACE("error: {}\n", sqlite3_errmsg(this->connection));
        exit(EXIT_FAILURE);
    }
    return;
}

void Database::bindSql(sqlite3_stmt* stmt, uint32_t i, const void* data, uint32_t size) {
    uint32_t rc = sqlite3_bind_blob(stmt, 8, data, size, SQLITE_STATIC);
    if (rc != SQLITE_OK) {
        SPDLOG_TRACE("error: {}\n", sqlite3_errmsg(this->connection));
        exit(EXIT_FAILURE);
    }
    return;
}

void Database::bindSql(sqlite3_stmt* stmt, uint32_t i, const double data) {
    uint32_t rc = sqlite3_bind_double(stmt, i, data);
    if (rc != SQLITE_OK) {
        SPDLOG_TRACE("error: {}\n", sqlite3_errmsg(this->connection));
        exit(EXIT_FAILURE);
    }
    return;
}

void Database::populateDB() {
    retrieveSets();
    // retrieveCards();
    return;
}

void Database::retrieveSets() {
    std::string rawJSON = downloadData("https://api.scryfall.com/sets");
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // ensure we respect scryfall's API guidelines by waiting at least 100 milliseconds before we can possibly initiate another connection
    json j = json::parse(rawJSON, nullptr, false);
    uint64_t it = 0;
    for (const auto& i : j["data"].items()) {
        insertCardSet(i.value(), it++);
    }
    return;
}

void Database::retrieveCards() {
    // getBulkDownload("https://api.scryfall.com/bulk-data/oracle-cards", "./all-cards.json");
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // ensure we respect scryfall's API guidelines by waiting at least 100 milliseconds before we can possibly initiate another connection
    return;
}

/***********************************************************************************************************/
/***********************************************************************************************************/
/***********************************************************************************************************/
/***********************************************************************************************************/
/***********************************************************************************************************/
/***********************************************************************************************************/
/***********************************************************************************************************/
/***********************************************************************************************************/
/***********************************************************************************************************/
/***********************************************************************************************************/
/***********************************************************************************************************/
/***********************************************************************************************************/
/***********************************************************************************************************/

void Database::insertArtistFromCard(const json& data) {
    sqlite3_open("./birdseye.db3", &this->connection);
    sqlite3_stmt* stmt;
    std::string sql = "INSERT INTO Artist(id, name) VALUES (?,?);";
	for (auto& i : data["artist_ids"].items()) {
    	uint32_t rc = sqlite3_prepare_v2(this->connection, sql.c_str(), static_cast<int>(sql.size()), &stmt, NULL);
    	if (rc != SQLITE_OK) {
        	SPDLOG_TRACE("error: {}\n", sqlite3_errmsg(this->connection));
        	exit(EXIT_FAILURE);
    	}
    	bindSql(stmt, 1, static_cast<std::string>(i.value()));
    	//bindSql(stmt, 2, static_cast<std::string>(data["artist"])); // problematic. need to implement a string parser to seperate into substrings using & as a delimeter. multiple artists are currently represented as "Jana Schirmer & Johannes Voss"
    	do {
        	rc = sqlite3_step(stmt);
        	if (rc == SQLITE_ERROR) {
            	SPDLOG_TRACE("error: {}\n", sqlite3_errmsg(this->connection));
            	exit(EXIT_FAILURE);
        	}
    	} while (rc == SQLITE_ROW);
		sqlite3_reset(stmt);
	}
    sqlite3_finalize(stmt);
    return;
}

void Database::insertArtistFromFace(const json& data) {
    sqlite3_open("./birdseye.db3", &this->connection);
    sqlite3_stmt* stmt;
    std::string sql = "INSERT INTO Artist(id, name) VALUES (?,?);";
    uint32_t rc = sqlite3_prepare_v2(this->connection, sql.c_str(), static_cast<int>(sql.size()), &stmt, NULL);
    if (rc != SQLITE_OK) {
        SPDLOG_TRACE("error: {}\n", sqlite3_errmsg(this->connection));
        exit(EXIT_FAILURE);
    }
    bindSql(stmt, 1, static_cast<std::string>(data["artist_id"]));
    bindSql(stmt, 2, static_cast<std::string>(data["artist"]));
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
//functions above here need work
void Database::insertCardSet(const json& data, size_t i) {
    sqlite3_open("./birdseye.db3", &this->connection);
    sqlite3_stmt* stmt;
    std::string sql = "INSERT INTO CardSet(id,code,mtgo_code,arena_code,tcgplayer_id,name,set_type,released_at,block_code,block,parent_set_code,card_count,printed_size,digital,foil_only,nonfoil_only,icon_svg_uri) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?);";
    uint32_t rc = sqlite3_prepare_v2(this->connection, sql.c_str(), static_cast<int>(sql.size()), &stmt, NULL);
    if (rc != SQLITE_OK) {
        SPDLOG_TRACE("error: {}\n", sqlite3_errmsg(this->connection));
        exit(EXIT_FAILURE);
    }
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
    bindSql(stmt, 17, static_cast<std::string>(data["icon_svg_uri"]));
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

void Database::insertCard(const json& data) {
    sqlite3_open("./birdseye.db3", &this->connection);
    sqlite3_stmt* stmt;
    std::string sql = "INSERT INTO Card(id,set_id,arena_id,lang,mtgo_id_mtgo_foil_id,tcgplayer_id,tcgplayer_etched_id,cardmarket_id,layout,oracle_id,rulings_uri,cmc,defense,edhrec_rank,hand_modifier,life_modifier,loyalty,name,penny_rank,reserved,type_line,booster,border_color,card_back_id,collector_number,content_warning,digital,flavor_name,frame,full_art,highres_image,image_status,oversized,printed_name,printed_text,printed_type_line,promo,rarity,released_at,reprint,story_spotlight,textless,variation,variation_of,security_stamp,scryfall_uri) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?);";
    uint32_t rc = sqlite3_prepare_v2(this->connection, sql.c_str(), static_cast<int>(sql.size()), &stmt, NULL);
    if (rc != SQLITE_OK) {
        SPDLOG_TRACE("error: {}\n", sqlite3_errmsg(this->connection));
        exit(EXIT_FAILURE);
    }
    bindSql(stmt, 1, static_cast<std::string>(data["id"]));
	bindSql(stmt, 2, static_cast<std::string>(data["set_id"]));
	bindSql(stmt, 3, static_cast<uint32_t>(data.value("arena_id", 0)));
	bindSql(stmt, 4, static_cast<std::string>(data["lang"]));
	bindSql(stmt, 5, static_cast<uint32_t>(data.value("mtgo_id", 0)));
    bindSql(stmt, 6, static_cast<uint32_t>(data.value("mtgo_foil_id", 0)));
    bindSql(stmt, 7, static_cast<uint32_t>(data.value("tcgplayer_id", 0)));
    bindSql(stmt, 8, static_cast<uint32_t>(data.value("tcgplayer_etched_id", 0)));
    bindSql(stmt, 9, static_cast<uint32_t>(data.value("cardmarket_id", 0)));
    bindSql(stmt, 10, static_cast<std::string>(data["layout"]));
    bindSql(stmt, 11, data.value("oracle_id", "null"));
    bindSql(stmt, 12, static_cast<std::string>(data["rulings_uri"]));
    bindSql(stmt, 13, static_cast<double>(data["cmc"]));
    bindSql(stmt, 14, data.value("defense", "null"));
    bindSql(stmt, 15, static_cast<uint32_t>(data.value("edhrec_rank", 0)));
    bindSql(stmt, 16, data.value("hand_modifier", "null"));
    bindSql(stmt, 17, data.value("life_modifier", "null"));
    bindSql(stmt, 18, data.value("loyalty", "null"));
    bindSql(stmt, 19, static_cast<std::string>(data["name"]));
    bindSql(stmt, 20, static_cast<uint32_t>(data.value("penny_rank", 0)));
    bindSql(stmt, 21, static_cast<uint32_t>(data["reserved"]));
    bindSql(stmt, 22, static_cast<std::string>(data["type_line"]));
    bindSql(stmt, 23, static_cast<uint32_t>(data["booster"]));
    bindSql(stmt, 24, static_cast<std::string>(data["border_color"]));
    bindSql(stmt, 25, static_cast<std::string>(data["card_back_id"]));
    bindSql(stmt, 26, static_cast<std::string>(data["collector_number"]));
    bindSql(stmt, 27, data.value("content_warning", "0"));
    bindSql(stmt, 28, static_cast<uint32_t>(data["digital"]));
    bindSql(stmt, 29, data.value("flavor_name", "null"));
    bindSql(stmt, 30, static_cast<std::string>(data["frame"]));
    bindSql(stmt, 31, static_cast<uint32_t>(data["full_art"]));
    bindSql(stmt, 32, static_cast<uint32_t>(data["highres_image"]));
    bindSql(stmt, 33, static_cast<std::string>(data["image_status"]));
    bindSql(stmt, 34, static_cast<uint32_t>(data["oversized"]));
    bindSql(stmt, 35, data.value("printed_name", "null"));
    bindSql(stmt, 36, data.value("printed_text", "null"));
    bindSql(stmt, 37, data.value("printed_type_line", "null"));
    bindSql(stmt, 38, static_cast<uint32_t>(data["promo"]));
    bindSql(stmt, 39, static_cast<std::string>(data["rarity"]));
    bindSql(stmt, 40, stringToTime(data["released_at"]), sizeof(time_t));
    bindSql(stmt, 41, static_cast<uint32_t>(data["reprint"]));
    bindSql(stmt, 42, static_cast<uint32_t>(data["story_spotlight"]));
    bindSql(stmt, 43, static_cast<uint32_t>(data["textless"]));
    bindSql(stmt, 44, static_cast<uint32_t>(data["variation"]));
    bindSql(stmt, 45, data.value("variation_of", "null"));
    bindSql(stmt, 46, data.value("security_stamp", "null"));
    bindSql(stmt, 47, static_cast<std::string>(data["scryfall_uri"]));
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

// functions below here need work
std::list<std::string> Database::insertColor(const json& dataArray) {
    sqlite3_open("./birdseye.db3", &this->connection);
    sqlite3_stmt* stmt;
    std::string sql = "INSERT INTO Color(id) VALUES (?);";
	std::list<std::string> colors;
	for (auto& i : dataArray.items()) {
    	uint32_t rc = sqlite3_prepare_v2(this->connection, sql.c_str(), static_cast<int>(sql.size()), &stmt, NULL);
    	if (rc != SQLITE_OK) {
        	SPDLOG_TRACE("error: {}\n", sqlite3_errmsg(this->connection));
        	exit(EXIT_FAILURE);
    	}
    	bindSql(stmt, 1, static_cast<std::string>(dataArray["id"]));
		colors.emplace_back(dataArray["id"]);
    	do {
        	rc = sqlite3_step(stmt);
        	if (rc == SQLITE_ERROR) {
        	    SPDLOG_TRACE("error: {}\n", sqlite3_errmsg(this->connection));
        	    exit(EXIT_FAILURE);
        	}
    	} while (rc == SQLITE_ROW);
		sqlite3_reset(stmt);
	}
    sqlite3_finalize(stmt);
    return colors;
}

void Database::insertFace(const json& data) {
    sqlite3_open("./birdseye.db3", &this->connection);
    sqlite3_stmt* stmt;
    std::string sql = "INSERT INTO Face(id,code,mtgo_code,arena_code,tcgplayer_id,name,set_type,released_at,block_code,block,parent_set_code,card_count,printed_size,digital,foil_only,nonfoil_only,icon_svg_uri) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?);";
    uint32_t rc = sqlite3_prepare_v2(this->connection, sql.c_str(), static_cast<int>(sql.size()), &stmt, NULL);
    if (rc != SQLITE_OK) {
        SPDLOG_TRACE("error: {}\n", sqlite3_errmsg(this->connection));
        exit(EXIT_FAILURE);
    }
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
    bindSql(stmt, 17, static_cast<std::string>(data["icon_svg_uri"]));
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

void Database::insertFinish(const json& data) {
    sqlite3_open("./birdseye.db3", &this->connection);
    sqlite3_stmt* stmt;
    std::string sql = "INSERT INTO Finish(id,code,mtgo_code,arena_code,tcgplayer_id,name,set_type,released_at,block_code,block,parent_set_code,card_count,printed_size,digital,foil_only,nonfoil_only,icon_svg_uri) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?);";
    uint32_t rc = sqlite3_prepare_v2(this->connection, sql.c_str(), static_cast<int>(sql.size()), &stmt, NULL);
    if (rc != SQLITE_OK) {
        SPDLOG_TRACE("error: {}\n", sqlite3_errmsg(this->connection));
        exit(EXIT_FAILURE);
    }
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
    bindSql(stmt, 17, static_cast<std::string>(data["icon_svg_uri"]));
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

void Database::insertFrameEffect(const json& data) {
    sqlite3_open("./birdseye.db3", &this->connection);
    sqlite3_stmt* stmt;
    std::string sql = "INSERT INTO FrameEffect(id,code,mtgo_code,arena_code,tcgplayer_id,name,set_type,released_at,block_code,block,parent_set_code,card_count,printed_size,digital,foil_only,nonfoil_only,icon_svg_uri) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?);";
    uint32_t rc = sqlite3_prepare_v2(this->connection, sql.c_str(), static_cast<int>(sql.size()), &stmt, NULL);
    if (rc != SQLITE_OK) {
        SPDLOG_TRACE("error: {}\n", sqlite3_errmsg(this->connection));
        exit(EXIT_FAILURE);
    }
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
    bindSql(stmt, 17, static_cast<std::string>(data["icon_svg_uri"]));
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

void Database::insertLegality(const json& data) {
    sqlite3_open("./birdseye.db3", &this->connection);
    sqlite3_stmt* stmt;
    std::string sql = "INSERT INTO Legality(id,code,mtgo_code,arena_code,tcgplayer_id,name,set_type,released_at,block_code,block,parent_set_code,card_count,printed_size,digital,foil_only,nonfoil_only,icon_svg_uri) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?);";
    uint32_t rc = sqlite3_prepare_v2(this->connection, sql.c_str(), static_cast<int>(sql.size()), &stmt, NULL);
    if (rc != SQLITE_OK) {
        SPDLOG_TRACE("error: {}\n", sqlite3_errmsg(this->connection));
        exit(EXIT_FAILURE);
    }
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
    bindSql(stmt, 17, static_cast<std::string>(data["icon_svg_uri"]));
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

void Database::insertImageURI(const json& data) {
    sqlite3_open("./birdseye.db3", &this->connection);
    sqlite3_stmt* stmt;
    std::string sql = "INSERT INTO ImageURI(id,code,mtgo_code,arena_code,tcgplayer_id,name,set_type,released_at,block_code,block,parent_set_code,card_count,printed_size,digital,foil_only,nonfoil_only,icon_svg_uri) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?);";
    uint32_t rc = sqlite3_prepare_v2(this->connection, sql.c_str(), static_cast<int>(sql.size()), &stmt, NULL);
    if (rc != SQLITE_OK) {
        SPDLOG_TRACE("error: {}\n", sqlite3_errmsg(this->connection));
        exit(EXIT_FAILURE);
    }
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
    bindSql(stmt, 17, static_cast<std::string>(data["icon_svg_uri"]));
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

void Database::insertKeyword(const json& data) {
    sqlite3_open("./birdseye.db3", &this->connection);
    sqlite3_stmt* stmt;
    std::string sql = "INSERT INTO Keyword(id,code,mtgo_code,arena_code,tcgplayer_id,name,set_type,released_at,block_code,block,parent_set_code,card_count,printed_size,digital,foil_only,nonfoil_only,icon_svg_uri) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?);";
    uint32_t rc = sqlite3_prepare_v2(this->connection, sql.c_str(), static_cast<int>(sql.size()), &stmt, NULL);
    if (rc != SQLITE_OK) {
        SPDLOG_TRACE("error: {}\n", sqlite3_errmsg(this->connection));
        exit(EXIT_FAILURE);
    }
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
    bindSql(stmt, 17, static_cast<std::string>(data["icon_svg_uri"]));
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

void Database::insertPromoType(const json& data) {
    sqlite3_open("./birdseye.db3", &this->connection);
    sqlite3_stmt* stmt;
    std::string sql = "INSERT INTO PromoType(id,code,mtgo_code,arena_code,tcgplayer_id,name,set_type,released_at,block_code,block,parent_set_code,card_count,printed_size,digital,foil_only,nonfoil_only,icon_svg_uri) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?);";
    uint32_t rc = sqlite3_prepare_v2(this->connection, sql.c_str(), static_cast<int>(sql.size()), &stmt, NULL);
    if (rc != SQLITE_OK) {
        SPDLOG_TRACE("error: {}\n", sqlite3_errmsg(this->connection));
        exit(EXIT_FAILURE);
    }
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
    bindSql(stmt, 17, static_cast<std::string>(data["icon_svg_uri"]));
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

void Database::insertRelatedCardObject(const json& data) {
    sqlite3_open("./birdseye.db3", &this->connection);
    sqlite3_stmt* stmt;
    std::string sql = "INSERT INTO RelatedCardObject(id,code,mtgo_code,arena_code,tcgplayer_id,name,set_type,released_at,block_code,block,parent_set_code,card_count,printed_size,digital,foil_only,nonfoil_only,icon_svg_uri) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?);";
    uint32_t rc = sqlite3_prepare_v2(this->connection, sql.c_str(), static_cast<int>(sql.size()), &stmt, NULL);
    if (rc != SQLITE_OK) {
        SPDLOG_TRACE("error: {}\n", sqlite3_errmsg(this->connection));
        exit(EXIT_FAILURE);
    }
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
    bindSql(stmt, 17, static_cast<std::string>(data["icon_svg_uri"]));
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