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
	void createTable(const std::string& tableName, const std::string& fields);
	time_t* stringToTime(const std::string& date);
	void bindSql(sqlite3_stmt* stmt, uint32_t i, const std::string& data);
	void bindSql(sqlite3_stmt* stmt, uint32_t i, const uint32_t data);
	void bindSql(sqlite3_stmt* stmt, uint32_t i, const void* data, uint32_t size);
	void bindSql(sqlite3_stmt* stmt, uint32_t i, const double data);
	bool doesRowExist(const std::string& table, const std::string& key);

	void populateDB();
	void retrieveSets();
	void retrieveCards();
	void insertArtistFromCard(const json& data);
	void insertArtistFromFace(const json& data);
	void insertCard(const json& data);
	std::list<std::string> insertColor(const json& data);
	void insertFace(const json& data);
	void insertFinish(const json& data);
	void insertFrameEffect(const json& data);
	void insertLegality(const json& data);
	void insertImageURI(const json& data);
	void insertKeyword(const json& data);
	void insertPromoType(const json& data);
	void insertRelatedCardObject(const json& data);
	void insertCardSet(const json& data, size_t i);
};