#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <iostream>
#include <ctime>
#include <cstring>

#include "Link.hpp"
#include "Object.hpp"
#include "Utils.hpp"
#include "Solver.hpp"
#include "Renderer.hpp"
#include "Input.hpp"
#include "QuadTree.hpp"

int main(int argc, char* argv[])
{
    SceneType currentScene = SceneType::FLUID;
    if (argc > 1) {
        std::string arg = argv[1];
        if (arg == "--fluid")    currentScene = SceneType::FLUID;
        else if (arg == "--cloth")    currentScene = SceneType::CLOTH;
        else if (arg == "--collider") currentScene = SceneType::COLLIDER;
        else if (arg == "--pendulum") currentScene = SceneType::PENDULUM;
        else {
            std::cout << "Unknown argument. Available: --fluid, --cloth, --collider, --pendulum" << std::endl;
        }
    }

    std::srand(std::time(nullptr)); // seeding the rng based on current time
    auto window = sf::RenderWindow(sf::VideoMode({1920u, 1080u}), "CMake SFML Project");
    window.setFramerateLimit(144);
    
    float dt = 1.0f / 144.0f; // time frame, good to match with fps
    int frame_count = 0; 
   
    Solver solver;
    Renderer renderer;
    Input input;
    DragState dragState;
    
    solver.initScene(currentScene);
    while (window.isOpen())
    {
        frame_count++;    
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        // Variable Speed of Light
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
            SPEED_OF_LIGHT += 100.0f;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
            if (SPEED_OF_LIGHT > 500.0f) { 
                SPEED_OF_LIGHT -= 100.0f;
            }
        }

        input.Update(window, solver.objects, dragState);
        solver.update(dt);

        window.clear();
        renderer.Draw(window, solver, dt);
        window.display();
    }
}
