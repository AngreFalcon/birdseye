#include "database.hpp"

void Database::getTableCol(const std::string& tableName, const std::string& col) {
    try {
        SQLite::Statement stmt(*connection, "SELECT " + col + " FROM " + tableName);
        uint32_t colNum = stmt.getColumnIndex(col.c_str());
        while (stmt.executeStep()) {
            SPDLOG_TRACE("{}: {}", col, stmt.getColumn(colNum).getString());
        }
    } catch (std::exception& e) {
        SPDLOG_TRACE("{}", e.what());
        exit(EXIT_FAILURE);
    }
    return;
}

void Database::getTable(const std::string& tableName) {
    try {
        SQLite::Statement stmt(*connection, "SELECT * FROM " + tableName);
        uint32_t numCols = stmt.getColumnCount();
        std::vector<std::string> colNames;
        for (uint32_t i = 0; i < numCols; i++) {
            colNames.emplace_back(stmt.getColumnName(i));
        }
        while (stmt.executeStep()) {
            std::string row;
            for (uint32_t i = 0; i < numCols; i++) {
                SQLite::Column colVal = stmt.getColumn(i);
                if (colVal.isText() && colVal.getString() != "") {
                    row += colNames[i] + ": " + colVal.getString() + "| ";
                }
                else if (colVal.isInteger() && colVal.getInt() != 0) {
                    row += colNames[i] + ": " + std::to_string(colVal.getInt()) + "| ";
                }
                else if (colVal.isFloat() && colVal.getDouble() != 0.0) {
                    row += colNames[i] + ": " + std::to_string(colVal.getDouble()) + "| ";
                }
            }
            SPDLOG_TRACE("{}", row);
        }
    } catch (std::exception& e) {
        SPDLOG_TRACE("{}", e.what());
        exit(EXIT_FAILURE);
    }
    return;
}

uint32_t Database::getNumRows(const std::string& tableName) {
    try {
		std::string sql = "SELECT COUNT(1) FROM " + tableName;
		SQLite::Statement stmt(*connection, sql);
		uint32_t numOfRecords = executeSQLGetInt(stmt, 0);
		SPDLOG_TRACE("{}: {}", sql, numOfRecords);
		return numOfRecords;
    } catch (std::exception& e) {
        SPDLOG_TRACE("{}", e.what());
        exit(EXIT_FAILURE);
    }
}

template <typename T>
std::optional<Card> getCardByCol(const std::string& colName, const T& colVal) {
	
}