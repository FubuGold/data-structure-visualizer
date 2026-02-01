#include <SFML/Graphics.hpp>
#include <SFML/GpuPreference.hpp>
SFML_DEFINE_DISCRETE_GPU_PREFERENCE

#include "../include/UI/GUI-element.h"
#include <iostream> // Debug

// Setup the scenes
void setup();

int main()
{
    sf::RenderWindow window(sf::VideoMode::getFullscreenModes().at(0), "Title", sf::State::Fullscreen);
    GUI::RectangleButton<> button(
        {100,400}, {200,500},
        "This is a button",
        40, 2,
        sf::Color::Black, sf::Color::Green, sf::Color::Red
        );
    button.setWindow(&window);
    button.setClickCallback([](sf::RectangleShape &rect,sf::Text &text){
        std::cerr << "Click registered\n";
    });
    button.setReleaseCallback([](sf::RectangleShape &rect,sf::Text &text){
        std::cerr << "Release registered\n";
    });
    button.setHoverInCallback([](sf::RectangleShape &rect,sf::Text &text){
        std::cerr << "Hover in detected\n";
    });
    button.setHoverOutCallback([](sf::RectangleShape &rect,sf::Text &text){
        std::cerr << "Hover out detected\n";
    });
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
        window.clear(sf::Color::White);

        button.draw();

        window.display();
    }
}

void setup() {}