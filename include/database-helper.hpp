#pragma once
#include "database.hpp"

/**
 * @brief Performs a check to ensure the database has a valid number of cards and a valid number of cardsets.
 * 
 * @param db A reference to the currently open database.
 */
void verifyDBIntegrality(Database& db);

/**
 * @brief Checks if the cardset table and the card table needs to be populated/updated, then calls the respective retrieval method if flagged as true.
 * 
 * @param db A reference to the currently open database.
 */
void populateDB(Database& db);

/**
 * @brief Performs a bulk download for cardsets, then inserts the sets into the cardset table.
 * 
 * @param db A reference to the currently open database.
 */
void retrieveSets(Database& db);

/**
 * @brief Performs a bulk download for cards, then inserts the cards into the card table.
 * 
 * @param db A reference to the currently open database.
 */
void retrieveCards(Database& db);