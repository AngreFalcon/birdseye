#pragma once
#include <string>
#include <vector>

struct Artist {
    Artist(std::string id, std::string name) {
        this->id = id;
        this->name = name;
    }
    Artist(void) = default;
    ~Artist() = default;

    std::string id;
    std::string name;
};