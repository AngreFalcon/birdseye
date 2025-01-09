#pragma once
#include "database.hpp"

void verifyDBIntegrality(Database& db);
void populateDB(Database& db);
void retrieveSets(Database& db);
void retrieveCards(Database& db);