#ifndef UI_GUI_COMPONENT
#define UI_GUI_COMPONENT

#include <SFML/Graphics.hpp>
#include <functional>
#include <concepts>

namespace GUI
{

/**
 * @brief Base class for GUI elements
 * 
 * Recommend use other elements built from this.
 * 
 * Use when need to create a special element.
 * 
 */
template<typename callback_t>
requires std::invocable<callback_t> && std::same_as<std::invoke_result_t<callback_t>, void>
class BaseElement : public sf::Drawable, public sf::Transformable
{
protected:
    callback_t onClickCallback, onReleaseCallback, onHoverInCallback, onHoverOutCallback;
    bool clicked = false;
    bool hovered = false;

    void onClick();
    void onHoverIn();
    void onHoverOut();
    void onRelease();
    
    void click(sf::Vector2f pos);
    void release(sf::Vector2f pos);
    void hoverIn(sf::Vector2f pos);
    void hoverOut(sf::Vector2f pos);
public:

    void setClickCallback(callback_t callback);
    void setHoverInCallback(callback_t callback);
    void setHoverOutCallback(callback_t callback);
    void setReleaseCallback(callback_t callback);

    virtual bool containPos(sf::Vector2f pos) = 0;

    virtual void handleEvent(const sf::Event& e) = 0;
};

//======================================================//

template<typename callback_t>
requires std::invocable<callback_t,sf::Text&,sf::RectangleShape&>
class RectangleButton : public BaseElement<callback_t>
{
protected:
    sf::Text text;
    sf::RectangleShape rect;
public:
    void draw() override;
    
    void containPos(sf::Vector2f pos) override;
    
    void handleEvent(const sf::Event& e) override;
};

}

#endif