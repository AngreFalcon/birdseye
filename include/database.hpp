#pragma once
#include <SQLiteCpp/SQLiteCpp.h>

namespace Database {
	/**
	 * @brief 
	 * 
	 * @return SQLite::Database 
	 */
    SQLite::Database initialize();
}