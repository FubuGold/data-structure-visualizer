#ifndef UI_GUI_SCENE
#define UI_GUI_SCENE

#include "GUI-element.h"
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
    std::vector<std::shared_ptr<BaseElement>> elements;
    std::vector<std::shared_ptr<IInteractiveElement>> interactableElements;
    sf::RenderWindow* window;

    template<typename T>
    requires std::derived_from<T,BaseElement>
    void addElement(std::shared_ptr<T> element_ptr);

public:
    void setupWindow(sf::RenderWindow *window_ptr);

    virtual void setup() = 0;

    virtual void draw();
};

class DebugScene : public Scene
{
public:
    void setup() override;
};

}

#endif