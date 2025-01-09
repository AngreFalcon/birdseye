#pragma once
#include "datastructs/card.hpp"
#include "datastructs/cardset.hpp"
#include <sqlitecpp/sqlitecpp.h>

using json = nlohmann::json;

class Database {
    friend class sax_event_consumer;

public:
	Database(const std::string& dbLoc);
    Database(void) = default;
    ~Database() = default;
    std::unique_ptr<SQLite::Database> connection;
    void initialize();
    bool validateAll(void);
    bool validate(const std::string& tableName);
    void getTableCol(const std::string& tableName, const std::string& col);
    void getTable(const std::string& tableName);
    void insertCardSet(const CardSet& cardSet);
    void insertCard(const Card& card);

private:
    void createTable(const std::string& tableName, const std::string& fields);
    void dropTable(const std::string& tableName);

    void bindOpt(SQLite::Statement& stmt, uint32_t ind, std::optional<std::string>);
    void bindOpt(SQLite::Statement& stmt, uint32_t ind, std::optional<uint32_t>);
    void bindOpt(SQLite::Statement& stmt, uint32_t ind, std::optional<bool>);
    void bindOpt(SQLite::Statement& stmt, uint32_t ind, std::optional<double>);
    void resetSql(SQLite::Statement& stmt);
    void executeSql(SQLite::Statement& stmt);
    uint32_t executeInsertGetInt(SQLite::Statement& stmt, uint32_t col);

    // void insertArtistFromCard(const json& data, const std::string& faceId);
    // void insertArtistFromFace(const json& data, const std::string& faceId);
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
    template <typename T, typename U, typename Y>
    void buildJunction(const std::string& sql, const T& firstPrimary, const U& secondPrimary, const Y& thirdPrimary);
    // void buildJunction(const std::string& sql, const std::string& firstPrimary, const std::string& secondPrimary);
    // void buildJunction(const std::string& sql, const uint32_t firstPrimary, const std::string& secondPrimary);
    // void buildJunction(const std::string& sql, const std::string& firstPrimary, const uint32_t secondPrimary);

    template <typename T>
    std::optional<Card> getCardByCol(const std::string& colName, const T& colVal);
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