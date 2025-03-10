#include "database-helper.hpp"
#include "download.hpp"
#include "parse.hpp"
#include <spdlog/spdlog.h>
#include <BS_thread_pool.hpp>
#include <filesystem>

void verifyDBIntegrality(Database& db) {
	json j = json::parse(downloadData("https://api.scryfall.com/sets"));
	uint32_t numOfCards = 0, numOfSets = 0;
	for (auto& i : j["data"].items()) {
		numOfCards += i.value()["card_count"];
		numOfSets++;
	}
	SPDLOG_TRACE("total num of cards: {}, total num of sets: {}", numOfCards, numOfSets);
	db.verifyNumOfSets(numOfSets);
	db.verifyNumOfCards(numOfCards);
	return;
}

void populateDB(Database& db) {
    if (db.doSetsNeedUpdate()) retrieveSets(db);
    if (db.doCardsNeedUpdate()) retrieveCards(db);
    return;
}

void retrieveSets(Database& db) {
    std::string rawJSON = downloadData("https://api.scryfall.com/sets");
    std::vector<CardSet> cardsets = parseSets(rawJSON);
    for (CardSet c : cardsets) {
        db.insertCardSet(c);
    }
    return;
}

void retrieveCards(Database& db) {
	std::string cacheFileLoc = "./download-cache.json";
    getBulkDownload("https://api.scryfall.com/bulk-data/oracle_cards", cacheFileLoc);
    getBulkDownload("https://api.scryfall.com/bulk-data/default_cards", cacheFileLoc);
    
	BS::thread_pool tp(std::thread::hardware_concurrency() - 1);
    std::mutex dbMutex;
    parseCards(cacheFileLoc, [&tp, &dbMutex, &db](Card& c) {
        tp.detach_task([&dbMutex, &db, c]() {
            dbMutex.lock();
            db.insertCard(c);
            dbMutex.unlock();
        });
    });
	tp.wait();
	std::filesystem::remove(cacheFileLoc);
    return;
}
