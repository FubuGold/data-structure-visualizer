#include <SFML/Graphics.hpp>
#include <SFML/GpuPreference.hpp>
SFML_DEFINE_DISCRETE_GPU_PREFERENCE

#include "../include/UI/GUI-scene.h"
#include "../include/global/global.h"
#include "../include/test/ds-test.h"
#include <iostream> // Debug

int main()
{
    avlTest();
    std::cerr << "======================================================" << '\n';
    sf::RenderWindow window(sf::VideoMode({1280, 800}), "Title", sf::State::Windowed);
    GUI::DebugScene debugScene;
    debugScene.setWindow((sf::RenderTarget*)&window);
    debugScene.setup();
    Global::deltaClock.restart();
    while (window.isOpen())
    {
        Global::deltaTime = Global::deltaClock.restart().asSeconds();
        while (const std::optional<sf::Event> event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            debugScene.handleEvent(event);
        }
        window.clear(sf::Color::White);

        debugScene.loopUpdate();

        debugScene.draw(window);

        window.display();
    }
}