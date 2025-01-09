#include "database.hpp"
#include "download.hpp"
#include "parse.hpp"
#include <filesystem>
#include <spdlog/spdlog.h>

void initializeProgram(Database& db);
void runProgram(Database& db);

void populateDB(Database& db);
void retrieveSets(Database& db);
void retrieveCards(Database& db);

int main(int argc, char** argv) {
    spdlog::set_level(spdlog::level::trace);
    std::string dbLoc = "./birdseye.db3";
    Database db(dbLoc);
    initializeProgram(db);
    return EXIT_SUCCESS;
}

void initializeProgram(Database& db) {
    db.initialize();
    populateDB(db);
    return;
}

void runProgram(Database& db) {
    while (true) {
        // db.getTable("CardSet");
    }
    return;
}

void populateDB(Database& db) {
    retrieveSets(db);
    retrieveCards(db);
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
    std::string cacheFile = "./all-cards.json"; // std::string cacheFile = "./download-cache.json";
    // getBulkDownload("https://api.scryfall.com/bulk-data/oracle-cards", cacheFile);
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
        //std::filesystem::remove(cacheFile);
    }
    return;
}
