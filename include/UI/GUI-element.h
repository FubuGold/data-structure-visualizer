#ifndef UI_GUI_ELEMENT
#define UI_GUI_ELEMENT

#include <SFML/Graphics.hpp>
#include <functional>
#include <concepts>

namespace GUI
{

sf::Font font("asset/font/SourceCodePro-VariableFont_wght.ttf");

/**
 * @brief Base element. Only for drawing
 * 
 * This is only for class hierarchy. Use the derived class.
 * 
 */
class BaseElement : public sf::Drawable, public sf::Transformable
{
protected:
    sf::RenderWindow *window;
public:
    void setWindow(sf::RenderWindow* window_ptr);

    virtual bool containPos(sf::Vector2f pos) = 0;

    void draw(sf::RenderTarget& target, sf::RenderStates state) const override final {};
    virtual void draw(sf::RenderStates state = sf::RenderStates::Default) = 0;
};

/**
 * @brief Interface for interactive element
 */
class IInteractiveElement : public BaseElement
{
public:
    virtual ~IInteractiveElement() = default;
    virtual void handleEvent(const std::optional<sf::Event>& e) = 0;
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
class InteractiveElement : public IInteractiveElement
{
protected:
    callback_t onClickCallback, onReleaseCallback, onHoverInCallback, onHoverOutCallback;
    bool clicked = false;
    bool hovered = false;
    sf::RenderWindow *window;

    virtual void onClick();
    virtual void onHoverIn();
    virtual void onHoverOut();
    virtual void onRelease();
    
    void click();
    void release();
    void hoverIn();
    void hoverOut();

public:
    void setClickCallback(callback_t callback);
    void setHoverInCallback(callback_t callback);
    void setHoverOutCallback(callback_t callback);
    void setReleaseCallback(callback_t callback);

};

//======================================================//

/**
 * @brief Rectangle button with text in the center
 * 
 * @tparam callback_t 
 */
template<typename callback_t = std::function<void(sf::RectangleShape&,sf::Text&)>>
requires std::invocable<callback_t,sf::RectangleShape&,sf::Text&>
class RectangleButton : public InteractiveElement<callback_t,sf::RectangleShape&,sf::Text&>
{
protected:

    sf::RectangleShape rect;
    sf::Text text;

    void onClick() override;
    void onRelease() override;
    void onHoverIn() override;
    void onHoverOut() override;

public:
    RectangleButton(
        sf::Vector2f btnSize = {300, 500},
        sf::Vector2f btnPos = {0, 0},
        std::string text = "",
        int characterSize = 20,
        int borderThickness = 2,
        sf::Color borderColor = sf::Color::Black,
        sf::Color bgColor = sf::Color::White,
        sf::Color textColor = sf::Color::Black
    );

    void draw(sf::RenderStates state = sf::RenderStates::Default) override;
    
    bool containPos(sf::Vector2f pos) override;
    
    void handleEvent(const std::optional<sf::Event>& e) override;
};

}

#include "GUI-element.ipp"

#endif