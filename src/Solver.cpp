#include "Solver.hpp"

void Solver::initScene() {
    for (int i = 0; i < 10; i++){
        for (int j = 0; j < 10; j++){
            float rand_offset = (std::rand() % 10) - 5;
            objects.emplace_back(500.0f + rand_offset + j * 100.0f, 100.0f + i * 100.0f, 10.0f);
        }
    }

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10 ; j++){
            float dist = 100.0f;
            if (i != 9){
                links.emplace_back(i + j * 10, i + 1 + j * 10, dist);
            }
            if ((j * 10 + i) + 10 < objects.size()){
                links.emplace_back(i + j * 10, (j * 10 + i) + 10, dist);
            }
        }
    }
}

void Solver::PinTopRow() {
    for (int i = 0; i < 10; i++) {
        objects[i].position = {500.0f + i * 100.0f, 100.0f};
        objects[i].last_position = objects[i].position;
    }
}

void Solver::solveCollisions() {
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

void Solver::SolveConstraints() {
    for (int i = 0; i < links.size(); i++){
        Link& link = links[i];
        link.apply(objects);
    }
    std::vector<Link> alive;
    alive.reserve(links.size());

    for (auto& link : links) {
        if (link.active) {
            alive.push_back(std::move(link));
        }
    }

    links = std::move(alive);
}

void Solver::UpdateObjects(float dt){
    for (auto& obj : objects){
        obj.update(dt);
    }
}

void Solver::update(float dt) {
    int sub_steps = 8; // Keep the stiffness high!
    float sub_dt = dt / (float)sub_steps;

    for (int s = 0; s < sub_steps; s++) {
        UpdateObjects(sub_dt);
        solveCollisions();
        SolveConstraints();
        PinTopRow();
    }
}