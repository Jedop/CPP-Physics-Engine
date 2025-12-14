#pragma once
#include <SFML/Graphics.hpp>

struct Object {
    sf::Vector2f position;
    sf::Vector2f last_position;
    sf::Vector2f acceleration;
    float radius;
    sf::Color color;

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
        sf::Vector2f velocity = position - last_position; 
        last_position = position;
        position = position + velocity + acceleration * (dt * dt);

        // Boundaries
        float ground = 1030.f;
        float ceiling = 0.f;
        float left_wall = 0.f;
        float right_wall = 1920.f;

        // Checking boundary condition
        if (position.y > ground){

            position.y = ground; // Ensures object does not go out of bounds

            float restitution = 0.7f; 
            velocity.y = -velocity.y * restitution;

            last_position.y = position.y - velocity.y; // We can imagine this as the object hitting an imaginary ground and the velocity from that collision is transferred over
        }
        // Checking boundary condition
        if (position.y < ceiling) {
            position.y = ceiling;

            float restitution = 0.7f; 
            velocity.y = -velocity.y * restitution;

            last_position.y = position.y - velocity.y;
        }
        // Checking boundary condition
        if (position.x < left_wall) {
            position.x = left_wall;

            float restitution = 0.7f; 
            velocity.x = -velocity.x * restitution;

            last_position.x = position.x - velocity.x;
        }
        // Checking boundary condition
        if (position.x > right_wall) {
            position.x = right_wall;

            float restitution = 0.7f; 
            velocity.x = -velocity.x * restitution;

            last_position.x = position.x - velocity.x;
        }
    }
};
