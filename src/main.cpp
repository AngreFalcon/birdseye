#include "database.hpp"
#include "database-helper.hpp"
#include <spdlog/spdlog.h>

void initializeProgram(Database& db);
void runProgram(Database& db);

int main(int argc, char** argv) {
    spdlog::set_level(spdlog::level::trace);
    std::string dbLoc = "./birdseye.db3";
    Database db(dbLoc);
	initializeProgram(db);
    return EXIT_SUCCESS;
}

void initializeProgram(Database& db) {
    db.initialize();
	verifyDBIntegrality(db);
    populateDB(db);
    return;
}

void runProgram(Database& db) {
    while (true) {
        // db.getTable("CardSet");
    }
    return;
}