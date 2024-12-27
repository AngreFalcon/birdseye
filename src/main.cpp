#include <spdlog/spdlog.h>
#include <wolfssl/openssl/ssl.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <ctime>
#include "download.hpp"
#include "database.hpp"
#include <filesystem>

int main(int argc, char** argv) {
	json j;
	std::vector<Set> sets;
	//std::string rawJSON = downloadData("https://api.scryfall.com/sets");
	//j = json::parse(rawJSON, nullptr, false);
	//for (const auto& i : j["data"].items()) {
	//	sets.emplace_back(jsonToSet(i.value()));
	//}
	if (std::filesystem::exists("./birdseye.db3"))
		std::filesystem::remove("./birdseye.db3");
	SQLite::Database db(Database::initialize());
	if (db.tableExists("Card") && db.tableExists("CardSet"))
		std::cout << "Successful?\n";
	return EXIT_SUCCESS;
}

//https://gatherer.wizards.com/Handlers/Image.ashx?multiverseid=!set:muid!&type=card
//https://gatherer.wizards.com/Handlers/Image.ashx?name=!name!&type=card