#include <SFML/Graphics.hpp>
#include <vector>
#include <functional>

namespace GUI
{

using callback_t = std::function<void()>;

/**
 * @brief Base class for GUI elements
 * 
 * Recommend use other elements built from this.
 * 
 * Use when need to create a special element.
 * 
 */
class BaseElement
{
protected:
    std::vector<std::shared_ptr<sf::Drawable>> parts;
    sf::RectangleShape interactBound;
    std::shared_ptr<sf::RenderWindow> window;

    bool clicked = false;
    bool hovered = false;
    
    callback_t onClickCallback, onReleaseCallback, onHoverInCallback, onHoverOutCallback;

    void onClick();
    void onHoverIn();
    void onHoverOut();
    void onRelease();
    
    bool containPos(sf::Vector2f pos);

    // Note: This function does not check the position of the mouse
    void click(sf::Vector2f pos);
    // Note: This function does not check the position of the mouse
    void release(sf::Vector2f pos);
    // Note: This function does not check the position of the mouse
    void hoverIn(sf::Vector2f pos);
    // Note: This function does not check the position of the mouse
    void hoverOut(sf::Vector2f pos);
public:
    // Set callback functions

    void setClickCallback(callback_t callback);
    void setHoverInCallback(callback_t callback);
    void setHoverOutCallback(callback_t callback);
    void setReleaseCallback(callback_t callback);

    // Set window pointer
    void setWindow(std::shared_ptr<sf::RenderWindow> window_ptr);

    // Process the event
    virtual void pollEvent(const sf::Event& e);

    // Set the interactable boundary directly
    void setBound();

    // Calculate the interactable boundary through the parts
    void recalBound();
};

}