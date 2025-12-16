#pragma once
#include <SFML/Graphics.hpp>
#include "Object.hpp"
#include "Link.hpp"
#include <vector>
#include "Solver.hpp"
#include "Utils.hpp"

class Renderer {
public:
    void DrawSpeed(sf::RenderWindow& window);
    void DrawObjects(sf::RenderWindow& window, const std::vector<Object>& objects, float dt);
    void DrawLinks(sf::RenderWindow& window, const std::vector<Object>& objects, const std::vector<Link>& links, float dt);
    void DrawGrid(sf::RenderWindow& window, Solver& solver);
    void Draw(sf::RenderWindow& window, Solver& solver, float dt);
};
