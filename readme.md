# Birdseye

Birdseye is a WIP card browser/search engine for Magic The Gathering. Birdseye is designed for offline use and uses the Scryfall API to build a local database powered by SQLite, enabling fast search queries without the need for an internet connection.

## Description

Birdseye is intended to be a simple, fast, and convenient tool to enable browsing and searching through the entire catalog of Magic The Gathering cards. As of version 0.1a, Birdseye is unfinished and no search functionality has been implemented. Birdseye is currently capable of interfacing with the Scryfall API and populating its own SQLite database. 

## Getting Started

### Dependencies

* Birdseye is built using libcurl, sqlitecpp, nlohmann_json, and threadpool. In addition to these, spdlog is used for debugging purposes.
* Birdseye is intended to be cross-platform, but has not been built or tested on any platforms other than Windows 10.

### Installing

* Any releases will be hosted alongside the github repository at https://github.com/AngreFalcon/birdseye/releases
* Birdseye is intended to be a portable application. It will never require elevated permissions and will not prompt for an install location. It will run within the directory it's contained in, and as such, will need to be located in a folder that does not have restricted read/write permissions.

### Executing program

* Birdseye does not have any execution instructions as of 0.1a

## Authors

* Michael Kukulka - [AngreFalcon](https://github.com/AngreFalcon)

## Version History

* 0.1a
    * Github repo set to public from private

## License

This project is licensed under the [GPL-2.0] License - see the LICENSE.md file for details

## Acknowledgments

Inspiration, code snippets, etc.
* [DomPizzie/README-Template.md](https://gist.github.com/DomPizzie/7a5ff55ffa9081f2de27c315f5018afc)
* [Scryfall API](https://scryfall.com/docs/api)