#ifndef UI_GUI_INTERACTABLE_ELEMENT_H
#define UI_GUI_INTERACTABLE_ELEMENT_H

#include "GUI-element.h"

#include <SFML/Graphics.hpp>
#include <functional>
#include <concepts>
#include <vector>

namespace GUI
{

/**
 * @brief Interface for interactable element
 */
class IInteractableElement : public sf::Drawable, public sf::Transformable
{
protected:
    sf::RenderTarget *target_ptr;
public:
    virtual ~IInteractableElement() = default;
    virtual void handleEvent(const std::optional<sf::Event>& e) = 0;
    virtual bool containPos(sf::Vector2f pos) = 0;
    void setWindow(sf::RenderTarget *target_ptr);
};

/**
 * @brief Base class for interactable GUI elements
 * 
 * @tparam callback_t The type of callback functions used in the element
 * 
 * This is only for class hierarchy. Use the derived class.
 * 
 */
template<typename callback_t, typename ...Args>
requires std::invocable<callback_t, Args...>
class InteractableElement : public IInteractableElement
{
protected:
    callback_t onClickCallback, onReleaseCallback, onHoverInCallback, onHoverOutCallback;
    bool clicked = false;
    bool hovered = false;

    // This will be called when the mouse is clicked is in the bounding box
    virtual void onClick();
    // This will be called when the mouse is hovered into the bounding box
    virtual void onHoverIn();
    // This will be called when the mouse is hovered out of the bounding box
    virtual void onHoverOut();
    // This will be called when the mouse is released when in the bounding box
    virtual void onRelease();
    
    virtual void click();
    virtual void release();
    virtual void hoverIn();
    virtual void hoverOut();

public:
    virtual ~InteractableElement() = default;

    // The callback will be called when the mouse is clicked is in the bounding box
    void setClickCallback(callback_t callback);
    // The callback will be called when the mouse is hovered into the bounding box
    void setHoverInCallback(callback_t callback);
    // The callback will be called when the mouse is hovered out of the bounding box
    void setHoverOutCallback(callback_t callback);
    // The callback will be called when the mouse is released when in the bounding box
    void setReleaseCallback(callback_t callback);

};

//======================================================//

using rectCallback_t = std::function<void(sf::RectangleShape&,sf::Text&)>; 

/**
 * @brief Rectangle button with text in the center
 * 
 */
class RectangleButton : public InteractableElement<rectCallback_t,sf::RectangleShape&,sf::Text&>
{
protected:

    sf::RectangleShape rect;
    sf::Text text;

    void onClick() override;
    void onRelease() override;
    void onHoverIn() override;
    void onHoverOut() override;

    void draw(sf::RenderTarget& target, sf::RenderStates state = sf::RenderStates::Default) const override;

public:
    RectangleButton(
        sf::Vector2f btnSize = {300, 500},
        sf::Vector2f btnPos = {0, 0},
        std::string text = "",
        int characterSize = 20,
        int textOutline = 0,
        int borderThickness = 2,
        sf::Color borderColor = sf::Color::Black,
        sf::Color bgColor = sf::Color::White,
        sf::Color textColor = sf::Color::Black
    );
    ~RectangleButton() = default;
    
    bool containPos(sf::Vector2f pos) override;
    
    void handleEvent(const std::optional<sf::Event>& e) override;
};

//======================================================//

using inputFieldFilterCb_t = std::function<bool(char32_t)>;

/**
 * @brief Text input field, with filter
 * 
 */
class TextInputField : public InteractableElement<std::function<void()>>
{
protected:
    sf::String value;
    sf::Text text;
    sf::RectangleShape rect;
    bool focused;
    inputFieldFilterCb_t filter;

    // std::vector<DebugDot> debugDots;

    void click() override;

    void draw(sf::RenderTarget& target, sf::RenderStates state = sf::RenderStates::Default) const override;
    
    void updateValue(char32_t chr);

public:
    TextInputField(
        sf::Vector2f fieldSize = {300, 500},
        sf::Vector2f fieldPos = {0, 0},
        int characterSize = 20,
        int textOutline = 0,
        int borderThickness = 2,
        sf::Color borderColor = sf::Color::Black,
        sf::Color bgColor = sf::Color::White,
        sf::Color textColor = sf::Color::Black
    );
    ~TextInputField() = default;

    void setFilter(inputFieldFilterCb_t filter);

    sf::String getValue();

    bool containPos(sf::Vector2f pos) override;

    void handleEvent(const std::optional<sf::Event>& e) override; 
};

}

#include "GUI-interactable-element.ipp"

#endif // UI_GUI_INTERACTABLE_ELEMENT_H