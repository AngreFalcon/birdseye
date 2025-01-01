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
	std::vector<std::string> separateArtistNames(const std::string& names);

	void prepareSql(sqlite3_stmt* stmt, const std::string& sql);
	void bindSql(sqlite3_stmt* stmt, uint32_t i, const std::string& data);
	void bindSql(sqlite3_stmt* stmt, uint32_t i, const uint32_t data);
	void bindSql(sqlite3_stmt* stmt, uint32_t i, const void* data, uint32_t size);
	void bindSql(sqlite3_stmt* stmt, uint32_t i, const double data);
	uint32_t resetSql(sqlite3_stmt* stmt);
	void finalizeSql(sqlite3_stmt* stmt);
	uint32_t finalizeSqlRetInt(sqlite3_stmt* stmt, uint32_t col);

	void populateDB();
	void retrieveSets();
	void retrieveCards();
	void insertArtistFromCard(const json& data, const std::string& faceId);
	void insertArtistFromFace(const json& data, const std::string& faceId);
	void insertCardSet(const json& data);
	void insertCard(const json& data);
	void colorJoin(const std::string& baseId, const std::string& colorId, const uint8_t funcInd);
	void insertColor(const json& dataArray, const std::string& id, const uint8_t funcInd);
	std::string insertFace(const json& data, const std::string& cardId);
	void insertFinish(const std::string& data, const std::string& cardId);
	void insertFrameEffect(const std::string& data, const std::string& cardId);
	void insertLegality(const std::string& format, const std::string& ruling, const std::string& cardId);
	void insertImageURI(const json& data, const std::string& faceId);
	void insertKeyword(const std::string& data, const std::string& cardId);
	void insertMultiverseID(const std::string& data, const std::string& cardId);
	void insertPromoType(const std::string& data, const std::string& cardId);
	void insertRelatedCardObject(const json& data, const std::string& cardId);

	/*junction table functions*/
	void joinCard_ColorIdentity(const std::string& cardId, const std::string& colorIdentityId);
	void joinCard_Legality(const std::string& cardId, const uint32_t legalityId);
	void joinCard_Finish(const std::string& cardId, const std::string& finishType);
	void joinCard_FrameEffect(const std::string& cardId, const std::string& frameEffectType);
	void joinCard_Keyword(const std::string& cardId, const std::string& keywordName);
	void joinCard_ProducedMana(const std::string& cardId, const std::string& producedManaId);
	void joinCard_PromoType(const std::string& cardId, const std::string& promoType);
	void joinCard_RelatedCardObject(const std::string& cardId, const std::string& relatedCardObjectId);
	void joinFace_Artist(const uint32_t faceId, const std::string& artistId);
	void joinFace_ColorIndicator(const uint32_t faceId, const std::string& colorIndicatorId);
	void joinFace_Colors(const uint32_t faceId, const std::string& colorsId);
};