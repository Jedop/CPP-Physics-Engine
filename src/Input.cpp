#include "Input.hpp"

void Input::Update(sf::RenderWindow& window, std::vector<Object>& objects, DragState& state) {
    bool mouseDown = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
    sf::Vector2f mouseWorldPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    if (mouseDown){
        if (!state.is_dragging){
            float min_dist = 100000.0f;
            for (int i = 0; i < objects.size(); ++i){
                float d = length(objects[i].position - mouseWorldPos);
                if (d < min_dist && d < objects[i].radius * 3.0f) { 
                    min_dist = d;
                    state.dragged_obj_idx = i;
                    state.is_dragging = true;
                }
            }
        }

        if (state.is_dragging && state.dragged_obj_idx != -1){
            Object& obj = objects[state.dragged_obj_idx];
            obj.last_position = mouseWorldPos;
            obj.position = mouseWorldPos;
        }
    } else {
        if (state.is_dragging && state.dragged_obj_idx != -1) {
            Object& obj = objects[state.dragged_obj_idx];
            sf::Vector2f throwVelocity = mouseWorldPos - state.lastmouseWorldPos;
            obj.last_position = obj.position - throwVelocity;
        }
        state.is_dragging = false;
        state.dragged_obj_idx = -1;
    }
    state.lastmouseWorldPos = mouseWorldPos;
}