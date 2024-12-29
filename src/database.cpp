#include "database.hpp"
#include "download.hpp"
#include <spdlog/spdlog.h>
#include <chrono>
#include <thread>

void Database::initialize() {
	std::string fileName = "./birdseye.db3";
	if (std::filesystem::exists(fileName))
		return;
    sqlite3_open(fileName.c_str(), &this->connection);
    createTable( "id TEXT PRIMARY KEY, "
                    "name TEXT",
        "Artist");
    createTable("id TEXT PRIMARY KEY, "
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
                    "icon_svg_uri TEXT",
        "CardSet");
    createTable("id TEXT PRIMARY KEY, "
                    "set_id TEXT REFERENCES CardSet(id), "
                    "arena_id INTEGER, "
                    "lang TEXT, "
                    "mtgo_id INTEGER, "
                    "mtgo_foil_id INTEGER, "
                    "tcgplayer_id INTEGER, "
                    "tcgplayer_etched_id INTEGER, "
                    "cardmarket_id INTEGER, "
                    "object TEXT, "
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
                    "scryfall_uri TEXT",
        "Card");
    createTable("id TEXT PRIMARY KEY, "
                    "value TEXT",
        "Color");
    createTable("id TEXT PRIMARY KEY, "
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
                    "watermark TEXT",
        "Face");
    createTable("type TEXT PRIMARY KEY",
        "Finish");
    createTable("type TEXT PRIMARY KEY",
        "FrameEffect");
    createTable("id TEXT PRIMARY KEY, "
                    "format TEXT, "
                    "ruling TEXT",
        "Legality");
    createTable("id TEXT PRIMARY KEY, "
                    "face_id TEXT REFERENCES Face(id), "
                    "small_uri TEXT, "
                    "normal_uri TEXT, "
                    "large_uri TEXT, "
                    "png_uri TEXT, "
                    "art_crop_uri TEXT, "
                    "border_crop_uri TEXT",
        "ImageURI");
    createTable("name TEXT PRIMARY KEY",
        "Keyword");
    createTable("id TEXT PRIMARY KEY, "
                    "card_id TEXT REFERENCES Card(id)",
        "MultiverseID");
    createTable("type TEXT PRIMARY KEY",
        "PromoType");
    createTable("id TEXT PRIMARY KEY, "
                    "object TEXT, "
                    "component TEXT, "
                    "name TEXT, "
                    "type_line TEXT",
        "RelatedCardObject");
    createTable("card_id TEXT REFERENCES Card(id), "
                    "color_id TEXT REFERENCES Color(id)",
        "Card_ColorIdentity");
    createTable("card_id TEXT REFERENCES Card(id), "
                    "color_id TEXT REFERENCES Color(id)",
        "Card_ColorIndicator");
    createTable("card_id TEXT REFERENCES Card(id), "
                    "legality_id TEXT REFERENCES Legality(id)",
        "Card_Legality");
    createTable("card_id TEXT REFERENCES Card(id), "
                    "color_id TEXT REFERENCES Color(id)",
        "Card_ProducedMana");
    createTable("card_id TEXT REFERENCES Card(id), "
                    "type TEXT REFERENCES Finish(type)",
        "Card_Finish");
    createTable("card_id TEXT REFERENCES Card(id), "
                    "type TEXT REFERENCES FrameEffect(type)",
        "Card_FrameEffect");
    createTable("card_id TEXT REFERENCES Card(id), "
                    "name TEXT REFERENCES Keyword(name)",
        "Card_Keyword");
    createTable("card_id TEXT REFERENCES Card(id), "
                    "promo_type TEXT REFERENCES PromoType(type)",
        "Card_PromoType");
    createTable("card_id TEXT REFERENCES Card(id), "
                    "relatedcardobject_id TEXT REFERENCES RelatedCardObject(id)",
        "Card_RelatedCardObject");
    createTable("face_id TEXT REFERENCES Face(id), "
                    "artist_id TEXT REFERENCES Artist(id)",
        "Face_Artist");
    createTable("face_id TEXT REFERENCES Face(id), "
                    "color_id TEXT REFERENCES Color(id)",
        "Face_ColorIndicator");
    createTable("face_id TEXT REFERENCES Face(id), "
                    "color_id TEXT REFERENCES Color(id)",
        "Face_Colors");
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

void Database::createTable(const std::string& fields, const std::string& tableName) {
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

time_t Database::stringToTime(const std::string& date) {
    std::istringstream dateStream;
    dateStream.str() = date;
    tm tm = {};
    dateStream >> std::get_time(&tm, "%Y-%m-%d");
    return mktime(&tm);
}

void Database::populateDB() {
    retrieveSets();
	retrieveCards();
    return;
}

void Database::retrieveSets() {
    std::string rawJSON = downloadData("https://api.scryfall.com/sets");
	std::this_thread::sleep_for(std::chrono::milliseconds(100)); // ensure we respect scryfall's API guidelines by waiting at least 100 milliseconds before we can possibly initiate another connection
    json j = json::parse(rawJSON, nullptr, false);
    for (const auto& i : j["data"].items()) {
        insertCardSet(i);
    }
    return;
}

void Database::retrieveCards() {
	//getBulkDownload("https://api.scryfall.com/bulk-data/oracle-cards", "./all-cards.json");
	std::this_thread::sleep_for(std::chrono::milliseconds(100)); // ensure we respect scryfall's API guidelines by waiting at least 100 milliseconds before we can possibly initiate another connection
    return;
}

void Database::insertArtist(const json& data) {

    return;
}

void Database::insertCard(const json& data) {

    return;
}

void Database::insertFace(const json& data) {

    return;
}

void Database::insertFinish(const json& data) {

    return;
}

void Database::insertFrameEffect(const json& data) {

    return;
}

void Database::insertLegality(const json& data) {

    return;
}

void Database::insertImageURI(const json& data) {

    return;
}

void Database::insertKeyword(const json& data) {

    return;
}

void Database::insertPromoType(const json& data) {

    return;
}

void Database::insertRelatedCardObjects(const json& data) {

    return;
}

void Database::insertCardSet(const json& data) {
    std::string uuid = data["id"];
    std::string setCode = data["code"];
    std::string mtgoCode = data.value("mtgo_code", "null");
    std::string arenaCode = data.value("arena_code", "null");
    uint32_t tcgplayerId = data.value("tcgplayer_id", 0);
    std::string name = data["name"];
    std::string type = data["set_type"];
    std::time_t releaseDate = stringToTime(data["released_at"]);
    std::string blockCode = data.value("block_code", "null");
    std::string block = data.value("block", "null");
    std::string parentCode = data.value("parent_set_code", "null");
    uint16_t cardCount = data["card_count"];
    uint16_t printedSize = data.value("printed_size", 0);
    bool digital = data["digital"];
    bool foilOnly = data["foil_only"];
    bool nonfoilOnly = data["nonfoil_only"];
    std::string IconURI = data["icon_svg_uri"];

    sqlite3_open("./birdseye.db3", &this->connection);
    sqlite3_stmt* stmt;
    std::string sql = "INSERT INTO CardSet(id,code,mtgo_code,arena_code,tcgplayer_id,name,set_type,released_at,block_code,block,parent_set_code,card_count,printed_size,digital,foil_only,nonfoil_only,icon_svg_uri) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?);";
    int rc = sqlite3_prepare_v2(this->connection, sql.c_str(), static_cast<int>(sql.size()), &stmt, NULL);
    if (rc != SQLITE_OK) {
		SPDLOG_TRACE("error: {}\n", sqlite3_errmsg(this->connection));
        exit(EXIT_FAILURE);
    }
    sqlite3_bind_text(stmt, 1, uuid.c_str(), static_cast<int>(uuid.length()), SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, setCode.c_str(), static_cast<int>(setCode.length()), SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, mtgoCode.c_str(), static_cast<int>(mtgoCode.length()), SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, arenaCode.c_str(), static_cast<int>(arenaCode.length()), SQLITE_STATIC);
	sqlite3_bind_int(stmt, 5, tcgplayerId);
    sqlite3_bind_text(stmt, 6, name.c_str(), static_cast<int>(name.length()), SQLITE_STATIC);
    sqlite3_bind_text(stmt, 7, type.c_str(), static_cast<int>(type.length()), SQLITE_STATIC);
	sqlite3_bind_blob(stmt, 8, &releaseDate, sizeof(time_t), SQLITE_STATIC);
    sqlite3_bind_text(stmt, 9, blockCode.c_str(), static_cast<int>(blockCode.length()), SQLITE_STATIC);
    sqlite3_bind_text(stmt, 10, block.c_str(), static_cast<int>(block.length()), SQLITE_STATIC);
    sqlite3_bind_text(stmt, 11, parentCode.c_str(), static_cast<int>(parentCode.length()), SQLITE_STATIC);
	sqlite3_bind_int(stmt, 12, cardCount);
	sqlite3_bind_int(stmt, 13, printedSize);
	sqlite3_bind_int(stmt, 14, digital);
	sqlite3_bind_int(stmt, 15, foilOnly);
	sqlite3_bind_int(stmt, 16, nonfoilOnly);
    sqlite3_bind_text(stmt, 17, IconURI.c_str(), static_cast<int>(IconURI.length()), SQLITE_STATIC);
    
	rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return;
}
