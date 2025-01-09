#pragma once
#include <string>

struct FrameEffect {
    FrameEffect(const std::string& type) {
        this->type = type;
    }
    FrameEffect(void) = default;
    ~FrameEffect() = default;

    std::string type;
};