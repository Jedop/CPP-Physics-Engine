#include "Solver.hpp"
#include <iostream>
void Solver::initScene(SceneType type) {
    currentScene = type;
    objects.clear();
    links.clear();
    objects.reserve(5000);
    switch (type) {
        case SceneType::FLUID:    setupFluid();    break;
        case SceneType::CLOTH:    setupCloth();    break;
        case SceneType::COLLIDER: setupCollider(); break;
        case SceneType::PENDULUM: setupPendulum(); break;
    }
}

void Solver::setupCloth() {
    const int width  = 10;
    const int height = 15;
    const float distx = 100.0f;
    const float disty = 25.0f;

    // create particles
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            float rand_offset = 0;
            objects.emplace_back(
                400.0f + rand_offset + i * distx,
                500.0f + j * disty,
                4.0f
            );
        }
    }

    // pin top corners
    objects[0].pinned = true;                 // top-left
    objects[width - 1].pinned = true;         // top-right

    // create links
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            int idx = i + j * width;

            // horizontal link
            if (i + 1 < width)
                links.emplace_back(idx, idx + 1, distx);

            // vertical link
            if (j + 1 < height)
                links.emplace_back(idx, idx + width, disty);
        }
    }

    objects.emplace_back(960.0f, 200.0f, 150.0f);
}


void Solver::setupFluid() {
    for (int i = 0; i < 10; i++){
        float rand_offset = (std::rand() % 10) - 5;
        objects.emplace_back(500.0f + rand_offset, 100.0f, 4.0f);
    }
}

void Solver::setupCollider() {
    for (int i = 0; i < 100; i++){
        float rand_offset = (std::rand() % 10) - 5;
        objects.emplace_back(500.0f + rand_offset, 100.0f + rand_offset, 50.0f);
    }
    for (Object& obj: objects){
        obj.last_position = {obj.position.x - (std::rand() % 101) - 50, obj.position.y - (std::rand() % 101) - 50};
        obj.acceleration = {0.0f, 0.0f};
    }
}

void Solver::setupPendulum() {
    objects.emplace_back(960.0f, 100.0f, 10.0f);
    objects.emplace_back(980.0f, 550.0f, 10.0f);
    objects.emplace_back(940.0f, 1000.0f, 10.0f);
    links.emplace_back(0, 1, 450.0f);
    links.emplace_back(1, 2, 450.0f);
    objects[0].pinned = true;
}

void Solver::solveCollisions(float dt) {
    std::vector<int> neighbors;
    sf::Vector2f center(1920/2.0f, 1080/2.0f);
    float width = 1920.0f;
    float height = 1080.0f;

    for (int i = 0; i < objects.size(); i++) {
        tree.insert(0, i);
    }

    for (int i = 0; i < objects.size(); i++) {
        std::vector<int> found;
        float r = objects[i].radius;
        sf::FloatRect range({objects[i].position.x - r, objects[i].position.y - r} , {r*2, r*2});

        tree.query(0, range, neighbors);
        for (int j : neighbors) {
            checkCollision(i, j, dt);
        }
        neighbors.clear();
    }

}

void Solver::checkCollision(int i, int j, float dt){
    if (i == j) return;
    Object& obj1 = objects[i];
    Object& obj2 = objects[j];

    sf::Vector2f collision_axis = obj1.position - obj2.position; // Axis along which collision happens
    float dist = length(collision_axis); // distance between the centres of mass of the two objects
    float min_dist = obj1.radius + obj2.radius; // minimum distance required between the objects assuming they are incompressible

    if (dist < min_dist) {
        
        sf::Vector2f v1 = (obj1.position - obj1.last_position) / dt;
        sf::Vector2f v2 = (obj2.position - obj2.last_position) / dt;

        float s1 = length(obj1.position - obj1.last_position) / dt;
        float s2 = length(obj2.position - obj2.last_position) / dt;

        s1 = std::min(s1, SPEED_OF_LIGHT * 0.99f);
        s2 = std::min(s2, SPEED_OF_LIGHT * 0.99f);

        float gamma1 = 1 / sqrt(1 - (s1*s1)/(SPEED_OF_LIGHT*SPEED_OF_LIGHT));
        float gamma2 = 1 / sqrt(1 - (s2*s2)/(SPEED_OF_LIGHT*SPEED_OF_LIGHT));

        float m1 = gamma1 * obj1.radius;
        float m2 = gamma2 * obj2.radius;
        float total_mass = m1 + m2;

        float ratio1 = m2 / total_mass;
        float ratio2 = m1 / total_mass;

        sf::Vector2f n1(0.f, 0.f);
        if (s1 > 0.0001f) {
            n1 = (obj1.position - obj1.last_position) / (dt * s1);
        }

        sf::Vector2f n2(0.f, 0.f);
        if (s2 > 0.0001f) {
            n2 = (obj2.position - obj2.last_position) / (dt * s2);
        }

        if (dist < 0.0001f) {
            collision_axis = sf::Vector2f(1.0f, 0.0f); 
            dist = 1.0f;
        }
        
        sf::Vector2f n = collision_axis / dist; // unit vector in direction of collision axis
        float delta = min_dist - dist; // overlap as a scalar

        float beta1 = s1 / SPEED_OF_LIGHT;
        float beta2 = s2 / SPEED_OF_LIGHT;

        float dot1 = n1.dot(n);
        float dot2 = n2.dot(n);
        dot1 = std::max(-1.0f, std::min(dot1, 1.0f));
        dot2 = std::max(-1.0f, std::min(dot2, 1.0f));
        float r1_eff = obj1.radius * std::sqrt(1.0f - (beta1 * beta1) * (dot1 * dot1));
        float r2_eff = obj2.radius * std::sqrt(1.0f - (beta2 * beta2) * (dot2 * dot2));
        
        float real_min_dist = r1_eff + r2_eff;
        if (dist >= real_min_dist) return;

        delta = real_min_dist - dist;
        
        obj1.position += ratio1 * delta * n; // moving the two objects away from each other
        obj2.position -= ratio2 * delta * n;
    }
}

void Solver::SolveConstraints(float dt) {
    for (int i = 0; i < links.size(); i++){
        Link& link = links[i];
        link.apply(objects, dt);
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
    if (objects.size() < 3000 && currentScene == SceneType::FLUID) {
        float rand_offset = (std::rand() % 10) - 5;
        objects.emplace_back(500.0f + rand_offset, 100.0f + rand_offset, 4.0f);
    }

    int sub_steps = 1; 
    float sub_dt = dt / (float)sub_steps;
    tree.build(objects);
    for (int s = 0; s < sub_steps; s++) {
        UpdateObjects(sub_dt);
        solveCollisions(sub_dt);
        SolveConstraints(sub_dt);
    }
    tree.clear();
}