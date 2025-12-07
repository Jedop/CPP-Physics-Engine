#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <iostream>
#include <ctime>


float length(sf::Vector2f v){
    return std::sqrt(v.x * v.x + v.y * v.y);
}

sf::Vector2f normalize(sf::Vector2f v) {
    float len = length(v);
    if (len == 0) return {0, 0};
    return v / len;
}


struct Object {
    sf::Vector2f position;
    sf::Vector2f last_position;
    sf::Vector2f acceleration;
    float radius;
    sf::Color color;

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

    void update(float dt){
        sf::Vector2f velocity = position - last_position;
        last_position = position;
        position = position + velocity + acceleration * (dt * dt);

        float ground = 1030.f;
        float ceiling = 0.f;
        float left_wall = 0.f;
        float right_wall = 1920.f;
        if (position.y > ground){

            position.y = ground;

            float restitution = 0.7f; 
            velocity.y = -velocity.y * restitution;

            last_position.y = position.y - velocity.y;
        }

        if (position.y < ceiling) {
            position.y = ceiling;

            float restitution = 0.7f; 
            velocity.y = -velocity.y * restitution;

            last_position.y = position.y - velocity.y;
        }

        if (position.x < left_wall) {
            position.x = left_wall;

            float restitution = 0.7f; 
            velocity.x = -velocity.x * restitution;

            last_position.x = position.x - velocity.x;
        }

        if (position.x > right_wall) {
            position.x = right_wall;

            float restitution = 0.7f; 
            velocity.x = -velocity.x * restitution;

            last_position.x = position.x - velocity.x;
        }
    }
};

void solveCollisions(std::vector<Object>& objects) {
    for (int i = 0; i < objects.size(); i++) {
        for (int j = i + 1; j < objects.size(); j++) {
            Object& obj1 = objects[i];
            Object& obj2 = objects[j];

            sf::Vector2f collision_axis = obj1.position - obj2.position;
            float dist = length(collision_axis);
            float min_dist = obj1.radius + obj2.radius;

            if (dist < min_dist) {
                if (dist < 0.0001f) {
                    collision_axis = sf::Vector2f(1.0f, 0.0f); // Arbitrary push X
                    dist = 1.0f;
                }
                sf::Vector2f n = collision_axis / dist;
                float delta = min_dist - dist;

                obj1.position += 0.5f * delta * n;
                obj2.position -= 0.5f * delta * n;
            }
        }
    }
}

int main()
{
    std::srand(std::time(nullptr));
    auto window = sf::RenderWindow(sf::VideoMode({1920u, 1080u}), "CMake SFML Project");
    window.setFramerateLimit(144);
    
    // Object ball(960.0f, 200.0f, 50.0f);
    std::vector<Object> objects;

    

    float dt = 1.0f / 144.0f;

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
        if (frame_count % 20 == 0) {
            float rand_offset = (std::rand() % 10) - 5;
            objects.emplace_back(500.0f + rand_offset, 100.0f, 10.0f); // x, y, radius
        }
        window.clear();
        for (auto& obj : objects){
            obj.update(dt);
        }

        solveCollisions(objects);
        
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
