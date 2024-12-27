#include "database.hpp"
#include <iostream>

namespace Database {
    /**
     * @brief
     *
     * @param db
     * @return true
     * @return false
     */
    bool validate(SQLite::Database& db);

    // Definitions
    SQLite::Database initialize() {	
        // Open a database file
        SQLite::Database db("birdseye.db3", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
        if (db.tableExists("Card")) {
            return db;
        }
        db.exec("CREATE TABLE Artist (id TEXT PRIMARY KEY, "
                "name TEXT);");
        db.exec("CREATE TABLE Card (id TEXT PRIMARY KEY, "
                "arena_id INTEGER, "
                "lang TEXT, "
                "mtgo_id INTEGER, "
                "mtgo_foil_id INTEGER, "
                "multiverse_ids BLOB, "
                "tcgplayer_id INTEGER, "
                "tcgplayer_etched_id INTEGER, "
                "cardmarket_id INTEGER, "
                "object TEXT, layout TEXT, "
                "oracle_id TEXT, "
                "rulings_uri TEXT, "
				"cmc REAL, "
                "color_identity TEXT, "
                "color_indicator TEXT, "
                "colors TEXT, "
                "defense TEXT, "
                "edhrec_rank INTEGER, "
                "hand_modifier TEXT, "
                "life_modifier TEXT, "
                "loyalty TEXT, "
                "mana_cost TEXT, "
                "name TEXT, "
                "oracle_text TEXT, "
                "penny_rank INTEGER, "
                "power TEXT, "
                "produced_mana TEXT, "
                "reserved INTEGER, "
                "toughness TEXT, "
                "type_line TEXT, "
                "booster INTEGER, "
                "border_color TEXT, "
                "card_back_id TEXT, "
                "collector_number TEXT, "
                "content_warning INTEGER, "
                "digital INTEGER, "
                "flavor_name TEXT, "
                "flavor_text TEXT, "
                "frame TEXT, "
                "full_art INTEGER, "
                "highres_image INTEGER, "
                "illustration_id TEXT, "
                "image_status TEXT, "
                "oversized INTEGER, "
                "printed_name TEXT, "
                "printed_text TEXT, "
                "printed_type_line TEXT, "
                "promo INTEGER, "
                "rarity TEXT, "
                "released_at BLOB, "
                "reprint INTEGER, "
                "set_id TEXT, "
                "story_spotlight INTEGER, "
                "textless INTEGER, "
                "variation INTEGER, "
                "variation_of TEXT, "
                "security_stamp TEXT, "
                "watermark TEXT, "
                "preview_previewed_at BLOB, "
                "preview_source_uri TEXT, "
                "preview_source TEXT);");
        db.exec("CREATE TABLE Face (id TEXT PRIMARY KEY, "
				"artist_id TEXT REFERENCES Artist(id), "
                "cmc REAL, "
                "color_indicator TEXT, "
                "colors TEXT, "
                "defense TEXT, "
                "flavor_text TEXT, "
                "illustration_id TEXT, "
                "layout TEXT, "
                "loyalty TEXT, "
                "mana_cost TEXT, "
                "name TEXT, "
                "object TEXT, "
                "oracle_id TEXT, "
                "oracle_text TEXT, "
                "power TEXT, "
                "printed_name TEXT, "
                "printed_type_line TEXT, "
                "toughness TEXT, "
                "type_line TEXT, "
                "watermark TEXT);");
        db.exec("CREATE TABLE Finish (type TEXT PRIMARY KEY);");
        db.exec("CREATE TABLE FrameEffect (type TEXT PRIMARY KEY);");
        db.exec("CREATE TABLE Legality (id TEXT PRIMARY KEY, "
				"card_id TEXT REFERENCES Card(id), "
                "format TEXT, "
                "ruling TEXT);");
        db.exec("CREATE TABLE ImageURI (uri TEXT PRIMARY KEY);");
        db.exec("CREATE TABLE Keyword (name TEXT PRIMARY KEY);");
        db.exec("CREATE TABLE PromoType (type TEXT PRIMARY KEY);");
        db.exec("CREATE TABLE RelatedCardObject (id TEXT PRIMARY KEY, "
                "object TEXT, "
                "component TEXT, "
                "name TEXT, "
                "type_line TEXT, "
                "uri TEXT);");
        db.exec("CREATE TABLE CardSet (id TEXT PRIMARY KEY, "
                "code TEXT, "
                "mtgo_code TEXT, "
                "arena_code TEXT, "
                "tcgplayer_id INTEGER, "
                "name TEXT, "
                "set_type TEXT, "
                "released_at BLOB, "
                "block_code TEXT, "
                "block TEXT, "
                "parent_set_code TEXT, "
                "card_count INTEGER, "
                "printed_size INTEGER, "
                "digital INTEGER, "
                "foil_only INTEGER, "
                "nonfoil_only INTEGER, "
                "icon_svg_uri TEXT);");
        db.exec("CREATE TABLE Card_Artist (card_id TEXT REFERENCES Card(id), "
                "artist_id TEXT REFERENCES Artist(id));");
        db.exec("CREATE TABLE Card_Face (card_id TEXT REFERENCES Card(id), "
                "face_id TEXT REFERENCES Face(id));");
        db.exec("CREATE TABLE Card_Finish (card_id TEXT REFERENCES Card(id), "
                "type TEXT REFERENCES Finish(type));");
        db.exec("CREATE TABLE Card_FrameEffect (card_id TEXT REFERENCES Card(id), "
                "type TEXT REFERENCES FrameEffect(type));");
        db.exec("CREATE TABLE Card_ImageURI (card_id TEXT REFERENCES Card(id), "
                "uri TEXT REFERENCES ImageURI(uri));");
        db.exec("CREATE TABLE Card_Keyword (card_id TEXT REFERENCES Card(id), "
                "name TEXT REFERENCES Keyword(name));");
        db.exec("CREATE TABLE Card_PromoType (card_id TEXT REFERENCES Card(id), "
                "promo_type TEXT REFERENCES PromoType(type));");
        db.exec("CREATE TABLE Card_RelatedCardObject (card_id TEXT REFERENCES Card(id), "
                "relatedcardobject_id TEXT REFERENCES RelatedCardObject(id));");
        db.exec("CREATE TABLE Face_Artist (face_id TEXT REFERENCES Face(id), "
                "artist_id TEXT REFERENCES Artist(id));");
        db.exec("CREATE TABLE Face_ImageURI (face_id TEXT REFERENCES Face(id), "
                "uri TEXT REFERENCES ImageURI(uri));");
        db.exec("CREATE TABLE CardSet_Card (set_id TEXT REFERENCES CardSet(id), "
                "card_id TEXT REFERENCES Card(id));");
        return db;
    }

    bool validate(SQLite::Database& db) {
        // TODO: database validation
        return true;
    }
}
