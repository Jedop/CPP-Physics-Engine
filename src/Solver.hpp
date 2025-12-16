#pragma once
#include <vector>

#include "Object.hpp" 
#include "Link.hpp"
#include "QuadTree.hpp"
#include "Utils.hpp"

enum class SceneType {
    FLUID,
    CLOTH,
    COLLIDER,
    PENDULUM
};

class Solver {
public:
    std::vector<Object> objects;
    std::vector<Link> links;
    QuadTree tree;
    SceneType currentScene;

    void initScene(SceneType type); 
    void CreateNode(sf::RenderWindow& window);
    void checkCollision(int i, int j, float dt);
    void UpdateObjects(float dt);
    void SolveConstraints(float dt);
    void solveCollisions(float dt);
    void update(float dt);
private:
    void setupFluid();
    void setupCloth();
    void setupCollider();
    void setupPendulum();
};