#pragma once
#include <SFML/Graphics.hpp>
#include "Utils.hpp"

struct Object {
    sf::Vector2f position;
    sf::Vector2f last_position;
    sf::Vector2f acceleration;
    float radius;
    sf::Color color;
    bool pinned = false;

    // Object Initialization
    Object(float x, float y, float r){
        position = {x, y};
        last_position = {x, y};    
        acceleration = {0.0f, 1000.0f}; 
        radius = r;

        int red = std::rand() % 255;
        int green = std::rand() % 255;
        int blue = std::rand() % 255;
        color = sf::Color(red, green, blue);
    }

    // How each ball updates every time frame
    // Done using Verlet Integration
    void update(float dt){
        if (pinned) {
            last_position = position;
            return;
        }

        sf::Vector2f velocity = position - last_position; 
        last_position = position;
        position = position + velocity + acceleration * (dt * dt);

        // Boundaries
        float ground = 1030.f;
        float ceiling = 0.f;
        float left_wall = 0.f;
        float right_wall = 1920.f;
        float dist_moved = length(velocity);
        float max_dist = SPEED_OF_LIGHT * dt; 

        if (dist_moved > max_dist) {
            sf::Vector2f dir = velocity / dist_moved;
            position = last_position + dir * max_dist;
        }

        // Checking boundary condition
        if (position.y + radius > ground){

            position.y = ground - radius; // Ensures object does not go out of bounds

            float restitution = 0.7f; 
            velocity.y = -velocity.y * restitution;

            last_position.y = position.y - velocity.y; // We can imagine this as the object hitting an imaginary ground and the velocity from that collision is transferred over
        }
        // Checking boundary condition
        if (position.y - radius < ceiling) {
            position.y = ceiling + radius;

            float restitution = 0.7f; 
            velocity.y = -velocity.y * restitution;

            last_position.y = position.y - velocity.y;
        }
        // Checking boundary condition
        if (position.x - radius < left_wall) {
            position.x = left_wall + radius;

            float restitution = 0.7f; 
            velocity.x = -velocity.x * restitution;

            last_position.x = position.x - velocity.x;
        }
        // Checking boundary condition
        if (position.x + radius > right_wall) {
            position.x = right_wall - radius;

            float restitution = 0.7f; 
            velocity.x = -velocity.x * restitution;

            last_position.x = position.x - velocity.x;
        }
    }
};
