#include <SFML/Graphics.hpp>
#include <SFML/GpuPreference.hpp>
SFML_DEFINE_DISCRETE_GPU_PREFERENCE

#include "../include/UI/GUI-scene.h"
#include <iostream> // Debug

int main()
{
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Title", sf::State::Windowed);
    GUI::DebugScene debugScene;
    debugScene.setWindow((sf::RenderTarget*)&window);
    debugScene.setup();
    while (window.isOpen())
    {
        while (const std::optional<sf::Event> event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            debugScene.handleEvent(event);
        }
        window.clear(sf::Color::White);

        debugScene.draw(window);

        window.display();
    }
}