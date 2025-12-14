#include "Renderer.hpp"

void Renderer::DrawObjects(sf::RenderWindow& window, const std::vector<Object>& objects){
    for (auto& obj : objects){
        sf::CircleShape shape(obj.radius);
        shape.setFillColor(obj.color);
        shape.setOrigin({obj.radius, obj.radius}); 
        shape.setPosition(obj.position);
        window.draw(shape);
    }
}

void Renderer::DrawLinks(sf::RenderWindow& window, const std::vector<Object>& objects, const std::vector<Link>& links){
    sf::VertexArray lines(sf::PrimitiveType::Lines);
    for (auto& link : links){
        sf::Vector2f p1 = objects[link.obj1_idx].position;
        sf::Vector2f p2 = objects[link.obj2_idx].position;

        lines.append({ p1, sf::Color::White });
        lines.append({ p2, sf::Color::White });
    }
    window.draw(lines);
}

void Renderer::Draw(sf::RenderWindow& window, Solver solver){
    DrawObjects(window, solver.objects);
    DrawLinks(window, solver.objects, solver.links);
}