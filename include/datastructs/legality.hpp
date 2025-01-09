#pragma once
#include <string>

struct Legality {
    Legality(const std::string& format, const std::string& ruling) {
        this->format = format;
        this->ruling = ruling;
    }
    Legality(void) = default;
    ~Legality() = default;

    std::string format;
    std::string ruling;
};