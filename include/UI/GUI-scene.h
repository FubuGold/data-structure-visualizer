#ifndef UI_GUI_SCENE_H
#define UI_GUI_SCENE_H

#include "GUI-interactable-element.h"
#include <SFML/Graphics.hpp>
#include <vector>

namespace GUI
{

/**
 * @brief Base class for scene
 * 
 */
class Scene
{
protected:
    std::vector<std::shared_ptr<sf::Drawable>> elements;
    std::vector<std::shared_ptr<IInteractableElement>> interactableElements;

    template<typename T>
    requires std::derived_from<T,sf::Drawable>
    void addElement(std::shared_ptr<T> element_ptr);

    sf::RenderTarget *target_ptr;
public:

    void handleEvent(const std::optional<sf::Event>& e);

    virtual void setup() = 0;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates state = sf::RenderStates::Default);

    void setWindow(sf::RenderTarget *target_ptr);
};

class DebugScene : public Scene
{
public:
    void setup() override;
};

}

#endif // UI_GUI_SCENE_H