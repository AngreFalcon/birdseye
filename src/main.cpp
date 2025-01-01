#include <iostream>
#include "database.hpp"
#include "download.hpp"
#include <filesystem>
#include <spdlog/spdlog.h>

void initializeProgram(const std::string& fileName, Database& db);
void runProgram(Database& db);

int main(int argc, char** argv) {
	std::string fileName = "./birdseye.db3";
	Database db;
	initializeProgram(fileName, db);
	return EXIT_SUCCESS;
}

void initializeProgram(const std::string& fileName, Database& db) {
    spdlog::set_level(spdlog::level::trace);
	db.initialize(fileName);

	return;
}

void runProgram(Database& db) {
	while (true) {
		db.getTable("CardSet");

	}
	return;
}