#pragma once
#include <vector>

#include "Object.hpp" 
#include "Link.hpp"
#include "QuadTree.hpp"

class Solver {
public:
    std::vector<Object> objects;
    std::vector<Link> links;
    QuadTree tree;

    void initScene();
    void CreateNode(sf::RenderWindow& window);
    void PinTopRow();
    void UpdateObjects(float dt);
    void SolveConstraints();
    void solveCollisions();
    void update(float dt);
};