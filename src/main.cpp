#include <iostream>
#include "database.hpp"
#include "download.hpp"
#include <filesystem>
#include <spdlog/spdlog.h>

int main(int argc, char** argv) {
    spdlog::set_level(spdlog::level::trace);
	Database db;
	if (std::filesystem::exists("./birdseye.db3"))
		std::filesystem::remove("./birdseye.db3");
	db.initialize();
	//downloadDataToFile("https://api.scryfall.com/cards/named?fuzzy=Phyrexian+Metamorph", "./testing.json");
	if (db.validate("Artist") 
	&& db.validate("PromoType") 
	&& db.validate("CardSet") 
	&& db.validate("Card") 
	&& db.validate("Face") 
	&& db.validate("Color") 
	&& db.validate("Finish") 
	&& db.validate("FrameEffect") 
	&& db.validate("Legality") 
	&& db.validate("ImageURI") 
	&& db.validate("Keyword") 
	&& db.validate("MultiverseID") 
	&& db.validate("RelatedCardObject"))
		std::cout << "Successful\n";
	return EXIT_SUCCESS;
}