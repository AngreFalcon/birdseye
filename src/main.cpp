#include <iostream>
#include "database.hpp"
#include "download.hpp"
#include <filesystem>
#include <spdlog/spdlog.h>

void initializeProgram(const std::string& fileName, Database& db);
void runProgram(Database& db);

int main(int argc, char** argv) {
    spdlog::set_level(spdlog::level::trace);
	//std::string fileName = "./birdseye.db3";
	//Database db;
	//initializeProgram(fileName, db);
	FILE* file;
	fopen_s(&file, "./all-cards.json", "r");
    sax_event_consumer sec;
    bool result = json::sax_parse(file, &sec);
	return EXIT_SUCCESS;
}

void initializeProgram(const std::string& fileName, Database& db) {
	db.initialize(fileName);

	return;
}

void runProgram(Database& db) {
	while (true) {
		db.getTable("CardSet");

	}
	return;
}