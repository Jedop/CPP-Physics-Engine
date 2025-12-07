#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <iostream>
#include <ctime>

// Calculate length of a given vector
float length(sf::Vector2f v){
    return std::sqrt(v.x * v.x + v.y * v.y);
}

// Normalize a given vector(for future use)
sf::Vector2f normalize(sf::Vector2f v) {
    float len = length(v);
    if (len == 0) return {0, 0};
    return v / len;
}

// The balls that we see in the simulation are these objects
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

void solveCollisions(std::vector<Object>& objects) {
    // O(n^2) collision detection
    for (int i = 0; i < objects.size(); i++) {
        for (int j = i + 1; j < objects.size(); j++) {
            Object& obj1 = objects[i];
            Object& obj2 = objects[j];

            sf::Vector2f collision_axis = obj1.position - obj2.position; // Axis along which collision happens
            float dist = length(collision_axis); // distance between the centres of mass of the two objects
            float min_dist = obj1.radius + obj2.radius; // minimum distance required between the objects assuming they are incompressible

            if (dist < min_dist) {
                // this is to avoid division by zero when there are too many objects occupying the same space
                if (dist < 0.0001f) {
                    collision_axis = sf::Vector2f(1.0f, 0.0f); 
                    dist = 1.0f;
                }
                
                sf::Vector2f n = collision_axis / dist; // unit vector in direction of collision axis
                float delta = min_dist - dist; // overlap as a scalar

                obj1.position += 0.5f * delta * n; // moving the two objects away from each other
                obj2.position -= 0.5f * delta * n;
            }
        }
    }
}

int main()
{
    std::srand(std::time(nullptr)); // seeding the rng based on current time
    auto window = sf::RenderWindow(sf::VideoMode({1920u, 1080u}), "CMake SFML Project");
    window.setFramerateLimit(144);
    
    std::vector<Object> objects; // list of balls

    float dt = 1.0f / 144.0f; // time frame, good to match with fps

    int frame_count = 0; 

    while (window.isOpen())
    {
        frame_count++;    
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }
        //adds a ball every 20 frames
        if (frame_count % 20 == 0) {
            float rand_offset = (std::rand() % 10) - 5;
            objects.emplace_back(500.0f + rand_offset, 100.0f, 10.0f); 
        }
        window.clear();
        // updates the position of the balls every frame
        for (auto& obj : objects){
            obj.update(dt);
        }
        // resolves collision issues
        solveCollisions(objects);
        // actually renders the balls with colors
        for (auto& obj : objects){
            sf::CircleShape shape(obj.radius);
            shape.setFillColor(obj.color);
            shape.setOrigin({obj.radius, obj.radius}); 
            shape.setPosition(obj.position);
            window.draw(shape);
        }

        window.display();
    }
}
