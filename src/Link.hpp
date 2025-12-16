#pragma once
#include <SFML/Graphics.hpp>
#include "Object.hpp"
#include "Utils.hpp"
#include <iostream>
struct Link {
    int obj1_idx; 
    int obj2_idx;
    float target_dist;
    bool active = true;

    Link(int id1, int id2, float dist)
        : obj1_idx(id1), obj2_idx(id2), target_dist(dist) {}

    void apply(std::vector<Object>& objects, float dt) {
        Object& obj1 = objects[obj1_idx];
        Object& obj2 = objects[obj2_idx];

        sf::Vector2f axis = obj1.position - obj2.position;
        sf::Vector2f v1 = (obj1.position - obj1.last_position) / dt;
        sf::Vector2f v2 = (obj2.position - obj2.last_position) / dt;
        sf::Vector2f velocity = (v1 + v2) / 2.0f;
        sf::Vector2f n_vel = velocity / (length(velocity) + 0.00001f);

        float dist = length(axis);
        float current_speed = length(velocity);
        float safe_speed = std::min(current_speed, SPEED_OF_LIGHT * 0.99f);
        float beta = safe_speed / SPEED_OF_LIGHT;
        if (beta > 0.99f) beta = 0.99f; 
        
        if (dist < 0.0001f) return; 

        sf::Vector2f n = axis / dist;

        float dot = n_vel.dot(n);
        dot = std::max(-1.0f, std::min(dot, 1.0f));

        float gamma = 1 / sqrt(1 - ((beta*beta) * (dot*dot)));
        float eff_target_dist = target_dist / gamma;
        float physical_limit = objects[obj1_idx].radius + objects[obj2_idx].radius;
        float delta = eff_target_dist - dist;

        if (eff_target_dist < physical_limit * 1.01f) eff_target_dist = physical_limit * 1.01f;
        if (dist > eff_target_dist * 10.0f) active = false;

        if (obj1.pinned){
            obj2.position -= delta * n; // pinned object does not move
        }
        else if (obj2.pinned){
            obj1.position += delta * n; // pinned object does not move
        }
        else {
            obj1.position += 0.5f * delta * n; // push each object back by half
            obj2.position -= 0.5f * delta * n;
        } 
    }
};