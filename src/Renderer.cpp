#include "Renderer.hpp"

void Renderer::DrawSpeed(sf::RenderWindow& window){
    sf::Font font("assets/fonts/ARIAL.TTF");
    sf::Text text(font);

    text.setString("Speed of Light : " + std::to_string(SPEED_OF_LIGHT));
    text.setCharacterSize(24);
    text.setPosition({1500.0f, 20.0f});
    text.setFillColor(sf::Color::White);
    text.setStyle(sf::Text::Bold);

    window.draw(text);
}

void Renderer::DrawObjects(sf::RenderWindow& window, const std::vector<Object>& objects, float dt){
    for (auto& obj : objects){
        sf::CircleShape shape(obj.radius);
        shape.setOrigin({obj.radius, obj.radius}); 

        sf::Vector2f velocity = (obj.position - obj.last_position) / dt;
        float speed = length(velocity);
        float beta = speed / SPEED_OF_LIGHT;
        float gamma = 1.0f / std::sqrt(1.0f - beta * beta);

        int r = static_cast<int>(255 * beta);
        int b = static_cast<int>(255 * (1.0f - beta));
        int g = 20; 

        shape.setFillColor(sf::Color(r, g, b));

        shape.setScale({1.0f / gamma, 1.0f});

        if (speed > 1.0f) {
            float angle_float = std::atan2(velocity.y, velocity.x) * 180.0f / 3.14159f;
            sf::Angle angle = sf::degrees(angle_float);
            shape.setRotation(angle);
        }

        shape.setPosition(obj.position);
        window.draw(shape);
    }
}

void Renderer::DrawLinks(sf::RenderWindow& window, const std::vector<Object>& objects, const std::vector<Link>& links, float dt){
    sf::VertexArray lines(sf::PrimitiveType::Lines);
    for (auto& link : links){
        sf::Vector2f p1 = objects[link.obj1_idx].position;
        sf::Vector2f p2 = objects[link.obj2_idx].position;

        lines.append({ p1, sf::Color::White });
        lines.append({ p2, sf::Color::White });
    }
    window.draw(lines);
}

void Renderer::DrawGrid(sf::RenderWindow& window, Solver& solver){
    //solver.tree.draw(0, window);
}

void Renderer::Draw(sf::RenderWindow& window, Solver& solver, float dt){
    DrawObjects(window, solver.objects, dt);
    DrawLinks(window, solver.objects, solver.links, dt);
    DrawGrid(window, solver);
    DrawSpeed(window);
}