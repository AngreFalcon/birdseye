#pragma once
#include <sqlite3.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Database {
public:
	sqlite3* connection;
    void initialize();
	bool validate(const std::string& tableName);
	//std::vector<CardPreview> getCardsByName(const std::string& name);
	//std::vector<CardPreview> getCardsByArtist(...);

private:
	void createTable(const std::string& sql, const std::string& tableName);
	time_t stringToTime(const std::string& date);

	void populateDB();
	void retrieveSets();
	void retrieveCards();
	void insertArtist(const json& data);
	void insertCard(const json& data);
	void insertFace(const json& data);
	void insertFinish(const json& data);
	void insertFrameEffect(const json& data);
	void insertLegality(const json& data);
	void insertImageURI(const json& data);
	void insertKeyword(const json& data);
	void insertPromoType(const json& data);
	void insertRelatedCardObjects(const json& data);
	void insertCardSet(const json& data);
};