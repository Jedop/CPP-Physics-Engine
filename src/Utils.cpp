// Utils.cpp
#include "Utils.hpp"
#include <cmath>

float SPEED_OF_LIGHT = 5000.0f;

float length(sf::Vector2f v){
    return std::sqrt(v.x * v.x + v.y * v.y);
}

sf::Vector2f normalize(sf::Vector2f v) {
    float len = length(v);
    if (len == 0) return {0,0};
    return v / len;
}


