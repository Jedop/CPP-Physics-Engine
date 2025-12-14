#pragma once
#include <SFML/Graphics.hpp>
#include "Object.hpp"
#include "Utils.hpp"

struct Link {
    int obj1_idx; 
    int obj2_idx;
    float target_dist;
    bool active = true;

    Link(int id1, int id2, float dist)
        : obj1_idx(id1), obj2_idx(id2), target_dist(dist) {}

    void apply(std::vector<Object>& objects) {
        Object& obj1 = objects[obj1_idx];
        Object& obj2 = objects[obj2_idx];

        sf::Vector2f axis = obj1.position - obj2.position;
        float dist = length(axis);

        if (dist < 0.0001f) return; 
        if (dist > target_dist * 2.0f) active = false;

        sf::Vector2f n = axis / dist;
        float delta = target_dist - dist;

        obj1.position += 0.5f * delta * n;
        obj2.position -= 0.5f * delta * n;
    }
};