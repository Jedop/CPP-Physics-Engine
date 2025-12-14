#pragma once
#include <vector>

#include "Object.hpp" 
#include "Link.hpp"

class Solver {
public:
    std::vector<Object> objects;
    std::vector<Link> links;

    void initScene();
    void PinTopRow();
    void UpdateObjects(float dt);
    void SolveConstraints();
    void solveCollisions();
    void update(float dt);
};