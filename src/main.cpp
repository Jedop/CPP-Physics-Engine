#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <iostream>
#include <ctime>

#include "Link.hpp"
#include "Object.hpp"
#include "Utils.hpp"
#include "Solver.hpp"
#include "Renderer.hpp"
#include "Input.hpp"

int main()
{
    std::srand(std::time(nullptr)); // seeding the rng based on current time
    auto window = sf::RenderWindow(sf::VideoMode({1920u, 1080u}), "CMake SFML Project");
    window.setFramerateLimit(144);

    float dt = 1.0f / 144.0f; // time frame, good to match with fps
    int frame_count = 0; 
   
    Solver solver;
    Renderer renderer;
    Input input;
    DragState dragState;
    
    solver.initScene();
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
        
        input.Update(window, solver.objects, dragState);
        solver.update(dt);
        
        window.clear();
        renderer.Draw(window, solver);
        window.display();
    }
}
