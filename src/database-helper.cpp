#include "database-helper.hpp"
#include "download.hpp"
#include "parse.hpp"
#include <spdlog/spdlog.h>
#include <BS_thread_pool.hpp>
#include <filesystem>

void verifyDBIntegrality(Database& db) {
	json j = json::parse(downloadData("https://api.scryfall.com/sets"));
	std::this_thread::sleep_for(std::chrono::milliseconds(100)); // ensure we respect scryfall's API guidelines by waiting at least 100 milliseconds before we can possibly initiate another connection
    uint32_t numOfCards = 0;
	uint32_t numOfSets = 0;
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
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // ensure we respect scryfall's API guidelines by waiting at least 100 milliseconds before we can possibly initiate another connection
    std::vector<CardSet> cardsets = parseSets(rawJSON);
    for (CardSet c : cardsets) {
        db.insertCardSet(c);
    }
    return;
}

void retrieveCards(Database& db) {
    std::string cacheFile = "./download-cache.json";
    getBulkDownload("https://api.scryfall.com/bulk-data/default_cards", cacheFile);
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // ensure we respect scryfall's API guidelines by waiting at least 100 milliseconds before we can possibly initiate another connection
    parseCards(cacheFile, [&db](Card& c) {
    	BS::thread_pool tp(std::thread::hardware_concurrency() - 1);
    	std::mutex dbMutex;
        tp.detach_task([&db, &c, &dbMutex]() {
            dbMutex.lock();
            db.insertCard(c);
            dbMutex.unlock();
        });
    });

    if (std::filesystem::exists(cacheFile)) {
		SPDLOG_TRACE("download cache removed");
        std::filesystem::remove(cacheFile);
    }
    return;
}
