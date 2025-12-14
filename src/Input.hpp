#pragma once
#include <SFML/Graphics.hpp>
#include "Object.hpp"
#include <vector>
#include "Utils.hpp"
struct DragState {
    bool is_dragging = false;
    int dragged_obj_idx = -1;
    sf::Vector2f lastmouseWorldPos;
};

class Input {
public:
    void Update(sf::RenderWindow& window, std::vector<Object>& objects, DragState& state);
};