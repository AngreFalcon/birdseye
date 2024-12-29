#include <iostream>
#include "database.hpp"
#include "download.hpp"
#include <filesystem>
#include <spdlog/spdlog.h>

int main(int argc, char** argv) {
    spdlog::set_level(spdlog::level::trace);
	//Database db;
	//db.initialize();
	//if (db.validate("Artist") && db.validate("PromoType"))
	//	std::cout << "Successful\n";
	return EXIT_SUCCESS;
}