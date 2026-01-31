#include <SFML/Graphics.hpp>
#include <SFML/GpuPreference.hpp>
SFML_DEFINE_DISCRETE_GPU_PREFERENCE

#include "../include/UI/GUI-element.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode::getFullscreenModes().at(0), "Title", sf::State::Fullscreen);
    GUI::RectangleButton<> button({300, 500}, {400, 500});
    button.setWindow(&window);
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            button.handleEvent(event);
        }
        window.clear();
        button.draw();
        
        window.display();
    }
}