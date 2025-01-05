#include "database.hpp"
#include "download.hpp"
#include <chrono>
#include <spdlog/spdlog.h>
#include <thread>

void Database::initialize(const std::string& fileName) {
	SPDLOG_TRACE("initialize");
	this->connection = new SQLite::Database(fileName, SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE);
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
    populateDB();
    return;
}

bool Database::validateAll(void) {
	std::array tables = {"Artist","PromoType","CardSet","Card","Face","Color","Finish","FrameEffect","Legality","ImageURI","Keyword","MultiverseID","RelatedCardObject"};
	for (std::string i : tables) {
		if (!this->validate(i))
			return false;
	}
	return true;
}

bool Database::validate(const std::string& tableName) {
	try {
		return this->connection->tableExists(tableName);
	}
	catch (std::exception& e){
		SPDLOG_TRACE("{}", e.what());
		exit(EXIT_FAILURE);
	}
}

void Database::getTableCol(const std::string& tableName, const std::string& col) {
	try {
		SQLite::Statement stmt(*this->connection, "SELECT " + col + " FROM " + tableName);
		uint32_t colNum = stmt.getColumnIndex(col.c_str());
		while (stmt.executeStep()) {
			SPDLOG_TRACE("{}: {}", col, stmt.getColumn(colNum).getString());
		}
	}
	catch (std::exception& e) {
		SPDLOG_TRACE("{}", e.what());
		exit(EXIT_FAILURE);
	}
	return;
}

void Database::getTable(const std::string& tableName) {
	try {
		SQLite::Statement stmt(*this->connection, "SELECT * FROM " + tableName);
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
	}
	catch (std::exception& e) {
		SPDLOG_TRACE("{}", e.what());
		exit(EXIT_FAILURE);
	}
	return;
}

void Database::createTable(const std::string& tableName, const std::string& fields) {
	dropTable(tableName);
	std::string sql = "CREATE TABLE " + tableName + " (" + fields + ");";
	try {
		SQLite::Transaction transaction(*this->connection);
		this->connection->exec(sql);
		transaction.commit();
	}
	catch (std::exception& e) {
		SPDLOG_TRACE("{}", e.what());
		exit(EXIT_FAILURE);
	}
    return;
}

void Database::dropTable(const std::string& tableName) {
	std::string sql = "DROP TABLE IF EXISTS " + tableName;
	try {
		SQLite::Transaction transaction(*this->connection);
		this->connection->exec(sql);
		transaction.commit();
	}
	catch (std::exception& e) {
		SPDLOG_TRACE("{}", e.what());
		exit(EXIT_FAILURE);
	}
	return;
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

void Database::resetSql(SQLite::Statement& stmt) {
	try {
		stmt.reset();
		stmt.clearBindings();
	}
	catch (std::exception& e) {
		SPDLOG_TRACE("{}", e.what());
		exit(EXIT_FAILURE);
	}
	return;
}

void Database::executeSql(SQLite::Statement& stmt) {
	try {
		while(stmt.executeStep());
	}
	catch (std::exception& e) {
		SPDLOG_TRACE("{}", e.what());
		exit(EXIT_FAILURE);
	}
	return;
}

uint32_t Database::executeSqlGetInt(SQLite::Statement& stmt, uint32_t col) {
	uint32_t result;
	try {
		while(stmt.executeStep());
		result = stmt.getColumn(col);
	}
	catch (std::exception& e) {
		SPDLOG_TRACE("{}", e.what());
		exit(EXIT_FAILURE);
	}
	return result;
}

void Database::populateDB(void) {
    retrieveSets();
    //retrieveCards();
    return;
}

void Database::retrieveSets(void) {
    std::string rawJSON = downloadData("https://api.scryfall.com/sets");
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // ensure we respect scryfall's API guidelines by waiting at least 100 milliseconds before we can possibly initiate another connection
    json j = json::parse(rawJSON, nullptr, false);
    uint64_t it = 0;
    for (const auto& i : j["data"].items()) {
        insertCardSet(i.value());
    }
    return;
}

void Database::retrieveCards(void) {
	std::string cacheFile = "./download-cache.json";
    getBulkDownload("https://api.scryfall.com/bulk-data/oracle-cards", cacheFile);
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // ensure we respect scryfall's API guidelines by waiting at least 100 milliseconds before we can possibly initiate another connection
    
    sax_event_consumer sec;
	sec.db = this;
	FILE* file;
	fopen_s(&file, cacheFile.c_str(), "r");
	bool result = json::sax_parse(file, &sec);
	if (!result) {
		// json file structure turned out invalid
	}
	if (std::filesystem::exists(cacheFile)) {
		std::filesystem::remove(cacheFile);
	}
	return;
}

void Database::insertArtistFromCard(const json& data, const std::string& faceId) {
    std::string sql = "INSERT INTO Artist(id, name) VALUES (?,?);";
    SQLite::Statement stmt(*this->connection, sql);
	size_t e = 0;
	std::vector<std::string> artistNames = separateArtistNames(data["artist"]);
	for (auto& i : data["artist_ids"].items()) {
    	stmt.bind(1, static_cast<std::string>(i.value()));
    	stmt.bind(2, artistNames[e++]);
		joinTables("INSERT INTO Face_Artist(face_id,artist_id) VALUES (?,?);", stoi(faceId), i.value());
		resetSql(stmt);
	}
    executeSql(stmt);
    return;
}

void Database::insertArtistFromFace(const json& data, const std::string& faceId) {
    std::string sql = "INSERT INTO Artist(id, name) VALUES (?,?);";
    SQLite::Statement stmt(*this->connection, sql);
    stmt.bind(1, static_cast<std::string>(data["artist_id"]));
    stmt.bind(2, static_cast<std::string>(data["artist"]));
	joinTables("INSERT INTO Face_Artist(face_id,artist_id) VALUES (?,?);", stoi(faceId), static_cast<std::string>(data["artist_id"]));
	executeSql(stmt);
    return;
}

void Database::insertCardSet(const json& data) {
    std::string sql = "INSERT INTO CardSet(id,code,mtgo_code,arena_code,tcgplayer_id,name,set_type,released_at,block_code,block,parent_set_code,card_count,printed_size,digital,foil_only,nonfoil_only,icon_svg_uri) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?);";
    SQLite::Statement stmt(*this->connection, sql);
	stmt.bind(1, static_cast<std::string>(data["id"]));
    stmt.bind(2, static_cast<std::string>(data["code"]));
	data.contains("mtgo_code") ? stmt.bind(3, static_cast<std::string>(data["mtgo_code"])) : stmt.bind(3);
    data.contains("arena_code") ? stmt.bind(4, static_cast<std::string>(data["arena_code"])) : stmt.bind(4);
    data.contains("tcgplayer_id") ? stmt.bind(5, static_cast<uint32_t>(data["tcgplayer_id"])) : stmt.bind(5);
    stmt.bind(6, static_cast<std::string>(data["name"]));
    stmt.bind(7, static_cast<std::string>(data["set_type"]));
    data.contains("released_at") ? stmt.bind(8, static_cast<std::string>(data["released_at"])) : stmt.bind(8);
    data.contains("block_code") ? stmt.bind(9, static_cast<std::string>(data["block_code"])) : stmt.bind(9);
    data.contains("block") ? stmt.bind(10, static_cast<std::string>(data["block"])) : stmt.bind(10);
    data.contains("parent_set_code") ? stmt.bind(11, static_cast<std::string>(data["parent_set_code"])) : stmt.bind(11);
    stmt.bind(12, static_cast<uint32_t>(data["card_count"]));
    data.contains("printed_size") ? stmt.bind(13, static_cast<uint32_t>(data["printed_size"])) : stmt.bind(13);
    stmt.bind(14, static_cast<uint32_t>(data["digital"]));
    stmt.bind(15, static_cast<uint32_t>(data["foil_only"]));
    stmt.bind(16, static_cast<uint32_t>(data["nonfoil_only"]));
    stmt.bind(17, static_cast<std::string>(data["icon_svg_uri"]));
	executeSql(stmt);
    return;
}

void Database::insertCard(const json& data) {
    std::string sql = "INSERT INTO Card(arena_id,id,lang,mtgo_id,mtgo_foil_id,tcgplayer_id,tcgplayer_etched_id,cardmarket_id,layout,oracle_id,rulings_uri,scryfall_uri,cmc,defense,hand_modifier,life_modifier,loyalty,name,reserved,type_line,booster,border_color,card_back_id,collector_number,content_warning,digital,flavor_name,frame,full_art,highres_image,image_status,oversized,printed_name,printed_text,printed_type_line,promo,rarity,released_at,reprint,set_id,story_spotlight,textless,variation,variation_of,security_stamp) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?);";
    SQLite::Statement stmt(*this->connection, sql);
	SPDLOG_TRACE("entered insert card");
	std::string cardId = static_cast<std::string>(data["id"]);
	//core fields
	data.contains("arena_id") ? stmt.bind(1, static_cast<uint32_t>(data["arena_id"])) : stmt.bind(1);
    stmt.bind(2, cardId);
	stmt.bind(3, static_cast<std::string>(data["lang"]));
	data.contains("mtgo_id") ? stmt.bind(4, static_cast<uint32_t>(data["mtgo_id"])) : stmt.bind(4);
    data.contains("mtgo_foil_id") ? stmt.bind(5, static_cast<uint32_t>(data["mtgo_foil_id"])) : stmt.bind(5);
    data.contains("tcgplayer_id") ? stmt.bind(6, static_cast<uint32_t>(data["tcgplayer_id"])) : stmt.bind(6);
    data.contains("tcgplayer_etched_id") ? stmt.bind(7, static_cast<uint32_t>(data["tcgplayer_etched_id"])) : stmt.bind(7);
    data.contains("cardmarket_id") ? stmt.bind(8, static_cast<uint32_t>(data["cardmarket_id"])) : stmt.bind(8);
    stmt.bind(9, static_cast<std::string>(data["layout"]));
    data.contains("oracle_id") ? stmt.bind(10, static_cast<std::string>(data["oracle_id"])) : stmt.bind(10);
    stmt.bind(11, static_cast<std::string>(data["rulings_uri"]));
    stmt.bind(12, static_cast<std::string>(data["scryfall_uri"]));
	//gameplay fields
    stmt.bind(13, static_cast<double>(data["cmc"]));
    data.contains("defense") ? stmt.bind(14, static_cast<std::string>(data["defense"])) : stmt.bind(14);
    data.contains("hand_modifier") ? stmt.bind(15, static_cast<std::string>(data["hand_modifier"])) : stmt.bind(15);
    data.contains("life_modifier") ? stmt.bind(16, static_cast<std::string>(data["life_modifier"])) : stmt.bind(16);
    data.contains("loyalty") ? stmt.bind(17, static_cast<std::string>(data["loyalty"])) : stmt.bind(17);
    stmt.bind(18, static_cast<std::string>(data["name"]));
    stmt.bind(19, static_cast<uint32_t>(data["reserved"]));
    stmt.bind(20, static_cast<std::string>(data["type_line"]));
	//print fields
    stmt.bind(21, static_cast<uint32_t>(data["booster"]));
    stmt.bind(22, static_cast<std::string>(data["border_color"]));
    stmt.bind(23, static_cast<std::string>(data["card_back_id"]));
    stmt.bind(24, static_cast<std::string>(data["collector_number"]));
    data.contains("content_warning") ? stmt.bind(25, static_cast<std::string>(data["content_warning"])) : stmt.bind(25);
    stmt.bind(26, static_cast<uint32_t>(data["digital"]));
    data.contains("flavor_name") ? stmt.bind(27, static_cast<std::string>(data["flavor_name"])) : stmt.bind(27);
    stmt.bind(28, static_cast<std::string>(data["frame"]));
    stmt.bind(29, static_cast<uint32_t>(data["full_art"]));
    stmt.bind(30, static_cast<uint32_t>(data["highres_image"]));
    stmt.bind(31, static_cast<std::string>(data["image_status"]));
    stmt.bind(32, static_cast<uint32_t>(data["oversized"]));
    data.contains("printed_name") ? stmt.bind(33, static_cast<std::string>(data["printed_name"])) : stmt.bind(33);
    data.contains("printed_text") ? stmt.bind(34, static_cast<std::string>(data["printed_text"])) : stmt.bind(34);
    data.contains("printed_type_line") ? stmt.bind(35, static_cast<std::string>(data["printed_type_line"])) : stmt.bind(35);
    stmt.bind(36, static_cast<uint32_t>(data["promo"]));
    stmt.bind(37, static_cast<std::string>(data["rarity"]));
    stmt.bind(38, static_cast<std::string>(data["released_at"]));
    stmt.bind(39, static_cast<uint32_t>(data["reprint"]));
	stmt.bind(40, static_cast<std::string>(data["set_id"]));
    stmt.bind(41, static_cast<uint32_t>(data["story_spotlight"]));
    stmt.bind(42, static_cast<uint32_t>(data["textless"]));
    stmt.bind(43, static_cast<uint32_t>(data["variation"]));
    data.contains("variation_of") ? stmt.bind(44, static_cast<std::string>(data["variation_of"])) : stmt.bind(44);
    data.contains("security_stamp") ? stmt.bind(45, static_cast<std::string>(data["security_stamp"])) : stmt.bind(45);
    executeSql(stmt);
	insertColor(data["color_identity"], cardId, 1);
	if (data.contains("produced_mana")) insertColor(data["produced_mana"], cardId, 2);
	if (data.contains("card_faces")) {
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
	if (data.contains("frame_effects")) {
		for (auto& i : data["frame_effects"].items()) {
			insertFrameEffect(i.value(), cardId);
		}
	}
	for (auto& i : data["keywords"].items()) {
		insertKeyword(i.value(), cardId);
	}
	if (data.contains("multiverse_ids")) {
		for (auto& i : data["multiverse_ids"].items()) {
			insertMultiverseID(i.value(), cardId);
		}
	}
	if (data.contains("promo_types")) {
		for (auto& i : data["promo_types"].items()) {
			insertPromoType(i.value(), cardId);
		}
	}
	if (data.contains("all_parts")) {
		for (auto& i : data["all_parts"].items()) {
			insertRelatedCardObject(i.value(), cardId);
		}
	}
    return;
}

void Database::colorJoin(const std::string& baseId, const std::string& colorId, const uint8_t funcInd) {
	switch (funcInd) {
	case 1:
		joinTables("INSERT INTO Card_ColorIdentity(card_id,color_id) VALUES (?,?);", baseId, colorId);
		break;
	case 2:
		joinTables("INSERT INTO Card_ProducedMana(card_id,color_id) VALUES (?,?);", baseId, colorId);
		break;
	case 3:
		joinTables("INSERT INTO Face_ColorIndicator(face_id,color_id) VALUES (?,?);", stoi(baseId), colorId);
		break;
	case 4:
		joinTables("INSERT INTO Face_Colors(face_id,color_id) VALUES (?,?);", stoi(baseId), colorId);
		break;
	}
	return;
}

void Database::insertColor(const json& dataArray, const std::string& id, const uint8_t funcInd) {
    std::string sql = "INSERT INTO Color(id) VALUES (?);";
    SQLite::Statement stmt(*this->connection, sql);
	for (auto& i : dataArray.items()) {
		stmt.bind(1, static_cast<std::string>(i.value()));
		colorJoin(id, static_cast<std::string>(i.value()), funcInd);
    	resetSql(stmt);
	}
    executeSql(stmt);
    return;
}

std::string Database::insertFace(const json& data, const std::string& cardId) {
    std::string sql = "INSERT INTO Face(card_id,cmc,defense,flavor_text,illustration_id,layout,loyalty,mana_cost,name,oracle_id,oracle_text,power,printed_name,printed_type_line,toughness,type_line,watermark) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?);";
    SQLite::Statement stmt(*this->connection, sql);
	stmt.bind(1, cardId);
	data.contains("cmc") ? stmt.bind(2, static_cast<double>(data["cmc"])) : stmt.bind(2);
    data.contains("defense") ? stmt.bind(3, static_cast<std::string>(data["defense"])) : stmt.bind(3);
    data.contains("flavor_text") ? stmt.bind(4, static_cast<std::string>(data["flavor_text"])) : stmt.bind(4);
    data.contains("illustration_id") ? stmt.bind(5, static_cast<std::string>(data["illustration_id"])) : stmt.bind(5);
    data.contains("layout") ? stmt.bind(6, static_cast<std::string>(data["layout"])) : stmt.bind(6);
    data.contains("loyalty") ? stmt.bind(7, static_cast<std::string>(data["loyalty"])) : stmt.bind(7);
    stmt.bind(8, static_cast<std::string>(data["mana_cost"]));
    stmt.bind(9, static_cast<std::string>(data["name"]));
    data.contains("oracle_id") ? stmt.bind(10, static_cast<std::string>(data["oracle_id"])) : stmt.bind(10);
    data.contains("oracle_text") ? stmt.bind(11, static_cast<std::string>(data["oracle_text"])) : stmt.bind(11);
    data.contains("power") ? stmt.bind(12, static_cast<std::string>(data["power"])) : stmt.bind(12);
    data.contains("printed_name") ? stmt.bind(13, static_cast<std::string>(data["printed_name"])) : stmt.bind(13);
    data.contains("printed_type_line") ? stmt.bind(14, static_cast<std::string>(data["printed_type_line"])) : stmt.bind(14);
    data.contains("toughness") ? stmt.bind(15, static_cast<std::string>(data["toughness"])) : stmt.bind(15);
    data.contains("type_line") ? stmt.bind(16, static_cast<std::string>(data["type_line"])) : stmt.bind(16);
    data.contains("watermark") ? stmt.bind(17, static_cast<std::string>(data["watermark"])) : stmt.bind(17);

	std::string id = std::to_string(executeSqlGetInt(stmt, 0));
	if (data.contains("color_indicator")) insertColor(data["color_indicator"], id, 3);
	if (data.contains("colors")) insertColor(data["colors"], id, 4);
	if (data.contains("image_uris")) insertImageURI(data["image_uris"], id);
	return id;
}

void Database::insertFinish(const std::string& data, const std::string& cardId) {
    std::string sql = "INSERT INTO Finish(type) VALUES (?);";
    SQLite::Statement stmt(*this->connection, sql);
	stmt.bind(1, data);
	joinTables("INSERT INTO Card_Finish(card_id,type) VALUES (?,?);", cardId, data);
    executeSql(stmt);
    return;
}

void Database::insertFrameEffect(const std::string& data, const std::string& cardId) {
    std::string sql = "INSERT INTO FrameEffect(type) VALUES (?);";
    SQLite::Statement stmt(*this->connection, sql);
	stmt.bind(1, data);
	joinTables("INSERT INTO Card_FrameEffect(card_id,type) VALUES (?,?);", cardId, data);
    executeSql(stmt);
    return;
}

void Database::insertLegality(const std::string& format, const std::string& ruling, const std::string& cardId) {
    std::string sql = "INSERT INTO Legality(format,ruling) VALUES (?,?);";
    SQLite::Statement stmt(*this->connection, sql);
    stmt.bind(1, format);
    stmt.bind(2, ruling);
	uint32_t id = executeSqlGetInt(stmt, 0);
	joinTables("INSERT INTO Card_Legality(card_id,legality_id) VALUES (?,?);", cardId, id);
    return;
}

void Database::insertImageURI(const json& data, const std::string& faceId) {
    std::string sql = "INSERT INTO ImageURI(face_id,small_uri,normal_uri,large_uri,png_uri,art_crop_uri,border_crop_uri) VALUES (?,?,?,?,?,?,?);";
    SQLite::Statement stmt(*this->connection, sql);
	stmt.bind(1, faceId);
    stmt.bind(2, static_cast<std::string>(data["small_uri"]));
    stmt.bind(3, static_cast<std::string>(data["normal_uri"]));
    stmt.bind(4, static_cast<std::string>(data["large_uri"]));
    stmt.bind(5, static_cast<std::string>(data["png_uri"]));
    stmt.bind(6, static_cast<std::string>(data["art_crop_uri"]));
    stmt.bind(7, static_cast<std::string>(data["border_crop_uri"]));
    executeSql(stmt);
    return;
}

void Database::insertKeyword(const std::string& data, const std::string& cardId) {
    std::string sql = "INSERT INTO Keyword(name) VALUES (?);";
    SQLite::Statement stmt(*this->connection, sql);
	stmt.bind(1, data);
	joinTables("INSERT INTO Card_Keyword(card_id,name) VALUES (?,?);", cardId, data);
    executeSql(stmt);
    return;
}

void Database::insertMultiverseID(const std::string& data, const std::string& cardId) {
    std::string sql = "INSERT INTO MultiverseID(id,card_id) VALUES (?,?);";
    SQLite::Statement stmt(*this->connection, sql);
	stmt.bind(1, data);
	stmt.bind(2, cardId);
    executeSql(stmt);
    return;
}

void Database::insertPromoType(const std::string& data, const std::string& cardId) {
    std::string sql = "INSERT INTO PromoType(type) VALUES (?);";
    SQLite::Statement stmt(*this->connection, sql);
	stmt.bind(1, data);
	joinTables("INSERT INTO Card_PromoType(card_id,promo_type) VALUES (?,?);", cardId, data);
    executeSql(stmt);
    return;
}

void Database::insertRelatedCardObject(const json& data, const std::string& cardId) {
    std::string sql = "INSERT INTO RelatedCardObject(id,component,name,type_line) VALUES (?,?,?,?);";
    SQLite::Statement stmt(*this->connection, sql);
	stmt.bind(1, static_cast<std::string>(data["id"]));
    stmt.bind(2, static_cast<std::string>(data["component"]));
    stmt.bind(3, static_cast<std::string>(data["name"]));
    stmt.bind(4, static_cast<std::string>(data["type_line"]));
	joinTables("INSERT INTO Card_RelatedCardObject(card_id,relatedcardobject_id) VALUES (?,?);", cardId, static_cast<std::string>(data["id"]));
    executeSql(stmt);
    return;
}

void Database::joinTables(const std::string& sql, const std::string& firstPrimary, const std::string& secondPrimary) {
    SQLite::Statement stmt(*this->connection, sql);
	stmt.bind(1, firstPrimary);
	stmt.bind(2, secondPrimary);
    executeSql(stmt);
    return;
}

void Database::joinTables(const std::string& sql, const uint32_t firstPrimary, const std::string& secondPrimary) {
    SQLite::Statement stmt(*this->connection, sql);
	stmt.bind(1, firstPrimary);
	stmt.bind(2, secondPrimary);
    executeSql(stmt);
    return;
}

void Database::joinTables(const std::string& sql, const std::string& firstPrimary, const uint32_t secondPrimary) {
    SQLite::Statement stmt(*this->connection, sql);
	stmt.bind(1, firstPrimary);
	stmt.bind(2, secondPrimary);
    executeSql(stmt);
    return;
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