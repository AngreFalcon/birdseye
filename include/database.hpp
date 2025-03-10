#pragma once
#include "datastructs/card.hpp"
#include "datastructs/cardset.hpp"
#include <sqlitecpp/sqlitecpp.h>

using json = nlohmann::json;

class Database {
    friend class sax_event_consumer;

public:
    std::unique_ptr<SQLite::Database> connection;

	Database(const std::string& dbLoc);
    Database(void) = default;
    ~Database() = default;

	// these methods are defined in database.cpp
    void initialize();
    bool validateAll(void);
    bool validate(const std::string& tableName);
	void verifyNumOfSets(uint32_t numOfSets);
	void verifyNumOfCards(uint32_t numOfCards);
	bool doSetsNeedUpdate();
	bool doCardsNeedUpdate();
    void getTableCol(const std::string& tableName, const std::string& col);
    void getTable(const std::string& tableName);
	uint32_t getNumRows(const std::string& tableName);

	// these methods are potentially destructive and are defined in database-update.cpp
    void insertCardSet(const CardSet& cardSet);
    void insertCard(const Card& card);

private:
	bool setsOutOfDate = true;
	bool cardsOutOfDate = true;

	// these methods are defined in database.cpp
	void bindOpt(SQLite::Statement& stmt, uint32_t ind, std::optional<std::string>);
    void bindOpt(SQLite::Statement& stmt, uint32_t ind, std::optional<uint32_t>);
    void bindOpt(SQLite::Statement& stmt, uint32_t ind, std::optional<bool>);
    void bindOpt(SQLite::Statement& stmt, uint32_t ind, std::optional<double>);
    void resetSql(SQLite::Statement& stmt);
    void executeSQL(SQLite::Statement& stmt);
    uint32_t executeSQLGetInt(SQLite::Statement& stmt, uint32_t col);
    template <typename T>
    std::optional<Card> getCardByCol(const std::string& colName, const T& colVal);

	// these methods are potentially destructive and are defined in database-update.cpp
    void createTable(const std::string& tableName, const std::string& fields);
    void dropTable(const std::string& tableName);
    void insertArtist(const Artist& artist, const uint32_t faceId);
    void insertColor(const Color& color);
    void insertFace(const Face& face, const std::string& cardId);
    void insertFinish(const Finish& finish, const std::string& cardId);
    void insertFormat(const Legality& legality);
    void insertFrameEffect(const FrameEffect& frameEffect, const std::string& cardId);
    uint32_t insertImageURI(const ImageURI& imageUri);
    void insertKeyword(const Keyword& keyword, const std::string& cardId);
    void insertLegality(const Legality& legality, const std::string& cardId);
    void insertMultiverseID(const MultiverseID& multiverseId, const std::string& cardId);
    void insertPromoType(const PromoType& promoType, const std::string& cardId);
    void insertRelatedCardObject(const RelatedCardObject& relatedCardObject, const std::string& cardId);
    void insertRuling(const Legality& legality);
    template <typename T, typename U>
    void buildJunction(const std::string& sql, const T& firstPrimary, const U& secondPrimary);
    template <typename T, typename U, typename V>
    void buildJunction(const std::string& sql, const T& firstPrimary, const U& secondPrimary, const V& thirdPrimary);

};