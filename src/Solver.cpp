#include "Solver.hpp"

void Solver::initScene() {
    // for (int i = 0; i < 10; i++){
    //     for (int j = 0; j < 10; j++){
    //         float rand_offset = (std::rand() % 10) - 5;
    //         objects.emplace_back(500.0f + rand_offset + j * 100.0f, 100.0f + i * 100.0f, 10.0f);
    //     }
    // }

    // for (int i = 0; i < 10; i++) {
    //     for (int j = 0; j < 10 ; j++){
    //         float dist = 100.0f;
    //         if (i != 9){
    //             links.emplace_back(i + j * 10, i + 1 + j * 10, dist);
    //         }
    //         if ((j * 10 + i) + 10 < objects.size()){
    //             links.emplace_back(i + j * 10, (j * 10 + i) + 10, dist);
    //         }
    //     }
    // }
    objects.reserve(3000);
    for (int i = 0; i < 10; i++){
        float rand_offset = (std::rand() % 10) - 5;
        objects.emplace_back(500.0f + rand_offset, 100.0f, 3.0f);
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
    std::vector<int> neighbors;
    sf::Vector2f center(1920/2.0f, 1080/2.0f);
    float width = 1920.0f;
    float height = 1080.0f;

    // Create a new root node on the heap
    

    for (int i = 0; i < objects.size(); i++) {
        tree.insert(0, i);
    }

    for (int i = 0; i < objects.size(); i++) {

        
        std::vector<int> found;
        float r = objects[i].radius;
        sf::FloatRect range({objects[i].position.x - r, objects[i].position.y - r} , {r*2, r*2});

        tree.query(0, range, neighbors);
        for (int j : neighbors) {
            if (i == j) continue;
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
        neighbors.clear();
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
    if (objects.size() < 5000) {
        // Spawn 2 at a time for a faster stream?
        float rand_offset = (std::rand() % 10) - 5;
        objects.emplace_back(500.0f + rand_offset, 100.0f, 3.0f);
    }

    int sub_steps = 1; // Keep the stiffness high!
    float sub_dt = dt / (float)sub_steps;
    tree.build(objects);
    for (int s = 0; s < sub_steps; s++) {
        UpdateObjects(sub_dt);
        solveCollisions();
        SolveConstraints();
    }
    tree.clear();
}