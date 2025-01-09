#pragma once
#include <cstdint>

struct MultiverseID {
    MultiverseID(uint32_t id) {
        this->id = id;
    };
    MultiverseID(void) = default;
    ~MultiverseID() = default;

    uint32_t id;
};