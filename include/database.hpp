#pragma once
#include <sqlitecpp/sqlitecpp.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Database {
friend class sax_event_consumer;
public:
	Database(void) = default;
	~Database() = default;
	SQLite::Database* connection;
    void initialize(const std::string& fileName);
	bool validateAll(void);
	bool validate(const std::string& tableName);
	//std::vector<CardPreview> getCardsByName(const std::string& name);
	//std::vector<CardPreview> getCardsByArtist(...);
	void getTableCol(const std::string& tableName, const std::string& col);
	void getTable(const std::string& tableName);

protected:
	void insertCard(const json& data);

private:
	void createTable(const std::string& tableName, const std::string& fields);
	void dropTable(const std::string& tableName);
	std::vector<std::string> separateArtistNames(const std::string& names);

	void resetSql(SQLite::Statement& stmt);
	void executeSql(SQLite::Statement& stmt);
	uint32_t executeSqlGetInt(SQLite::Statement& stmt, uint32_t col);

	void populateDB(void);
	void retrieveSets(void);
	void retrieveCards(void);
	void insertArtistFromCard(const json& data, const std::string& faceId);
	void insertArtistFromFace(const json& data, const std::string& faceId);
	void insertCardSet(const json& data);
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
	void joinTables(const std::string& sql, const std::string& firstPrimary, const std::string& secondPrimary);
	void joinTables(const std::string& sql, const uint32_t firstPrimary, const std::string& secondPrimary);
	void joinTables(const std::string& sql, const std::string& firstPrimary, const uint32_t secondPrimary);
	/*
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
	*/
};