#pragma once
#include <SFML/Graphics.hpp>
#include "Object.hpp"
#include "Link.hpp"
#include <vector>
#include "Solver.hpp"

class Renderer {
public:
    void DrawObjects(sf::RenderWindow& window, const std::vector<Object>& objects);
    void DrawLinks(sf::RenderWindow& window, const std::vector<Object>& objects, const std::vector<Link>& links);
    void Draw(sf::RenderWindow& window, Solver solver);
};
