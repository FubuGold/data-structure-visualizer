#include <SFML/Graphics.hpp>
#include <SFML/GpuPreference.hpp>
SFML_DEFINE_DISCRETE_GPU_PREFERENCE

#include "../include/UI/GUI-scene.h"
#include "../include/global/global.h"
#include "../include/test/ds-test.h"
#include <iostream> // Debug

std::shared_ptr<GUI::Scene> scenes[Global::numScene];

void setupScene(sf::RenderTarget *target)
{
    using Global::toInt;
    using Global::SceneState;
    scenes[toInt(SceneState::MENU)] = std::make_shared<GUI::MenuScene>();
    scenes[toInt(SceneState::AVL)] = std::make_shared<GUI::AVLScene>();

    for (int i = 0; i < Global::numScene; i++) {
        if (scenes[i]) {
            scenes[i]->setWindow(target);
            scenes[i]->setup();
        }
    }
}

int main()
{

    avlTest();
    std::cerr << "======================================================" << '\n';
    sf::RenderWindow window(sf::VideoMode({1280, 800}), "Data structure visualizer", sf::State::Windowed);
    
    setupScene(&window);
    std::cerr << "======================================================" << '\n';
    
    std::shared_ptr<GUI::Scene> curScene = scenes[toInt(Global::curAppState)];

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
            curScene->handleEvent(event);
        }

        curScene = scenes[toInt(Global::curAppState)];

        window.clear(sf::Color::White);

        curScene->loopUpdate();
        curScene->draw(window);

        window.display();
    }
}