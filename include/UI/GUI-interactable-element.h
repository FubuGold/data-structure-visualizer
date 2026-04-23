#ifndef UI_GUI_INTERACTABLE_ELEMENT_H
#define UI_GUI_INTERACTABLE_ELEMENT_H

#include "GUI-element.h"

#include <SFML/Graphics.hpp>
#include <functional>
#include <concepts>
#include <vector>

namespace GUI
{

using voidCallback_t = std::function<void()>;

/**
 * @brief Interface for interactable element
 */
class IInteractableElement : public sf::Drawable, public sf::Transformable
{
protected:
    sf::RenderTarget *target_ptr;
    bool locked = false;
public:
    sf::Color baseColor, disableColor = Global::colorSet[0][Global::COLOR_TYPE::DISABLE];
    IInteractableElement() = default;
    virtual ~IInteractableElement() = default;
    virtual void handleEvent(const std::optional<sf::Event>& e) = 0;
    virtual bool containPos(sf::Vector2f pos) = 0;
    virtual void setWindow(sf::RenderTarget *target_ptr);
    /**
     * @brief This will completely lock the element, drop all event. Unlock by outside call.
     * 
     */
    virtual void lock();
    /**
     * @brief Unlock the element
     * 
     */
    virtual void unlock();
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
        sf::Color bgColor = sf::Color::White,
        sf::Color textColor = sf::Color::Black,
        sf::Color borderColor = sf::Color::Black
    );
    ~RectangleButton() = default;
    
    bool containPos(sf::Vector2f pos) override;
    
    void handleEvent(const std::optional<sf::Event>& e) override;

    void setString(const std::string &s);

    void lock() override;
    void unlock() override;
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
    std::string value;
    sf::Text text;
    sf::RectangleShape rect;
    bool focused;
    inputFieldFilterCb_t filter;

    int maxChar;

    // std::vector<DebugDot> debugDots;

    void click() override;

    void draw(sf::RenderTarget& target, sf::RenderStates state = sf::RenderStates::Default) const override;
    
    void updateValue(char32_t chr);

public:
    TextInputField(
        sf::Vector2f fieldSize = {300, 500},
        sf::Vector2f fieldPos = {0, 0},
        sf::Vector2f textOffset = {0,0},
        int maxChar = 10,
        int characterSize = 20,
        int textOutline = 0,
        int borderThickness = 2,
        sf::Color bgColor = sf::Color::White,
        sf::Color textColor = sf::Color::Black,
        sf::Color borderColor = sf::Color::Black
    );
    ~TextInputField() = default;

    void setFilter(inputFieldFilterCb_t filter);

    sf::String getValue();

    bool containPos(sf::Vector2f pos) override;

    void handleEvent(const std::optional<sf::Event>& e) override; 

    void lock() override;
    void unlock() override;
};

//======================================================//


using sliderUpdateCb_t = std::function<void(float)>;

/**
 * @brief Horizontal slider with progress bar
 * 
 */
class HSlider : public InteractableElement<voidCallback_t>
{
protected:

    int numSteps;
    float startValue, endValue, curValue;
    float valueStep;

    sliderUpdateCb_t changeCallback;

    sf::Vector2f pos,size;
    sf::RectangleShape bgRect,progressRect;
    
    void draw(sf::RenderTarget &target, sf::RenderStates state = sf::RenderStates::Default) const override;

    int findSegment(sf::Vector2f pos);

    void updateValue(int segment);

public:

    HSlider(
        sf::Vector2f size,
        sf::Vector2f pos,
        int numSteps,
        int startValue,
        int endValue,
        int borderThickness = 2,
        sf::Color bgColor = sf::Color::White,
        sf::Color progressColor = sf::Color(100,100,100),
        sf::Color borderColor = sf::Color::Black
    );
    ~HSlider() = default;

    void handleEvent(const std::optional<sf::Event>& e) override;
    bool containPos(sf::Vector2f pos) override;

    void setChangeCb(sliderUpdateCb_t cb);

    float getValue();
    void setValue(float val);

    void setNewRange(float startValue, float endValue, int numSteps);
};

//======================================================//

class MultilineTextField : public IInteractableElement
{
private:
    sf::RectangleShape box;
    sf::RectangleShape caretShape;
    sf::Text textTemplate; // used to measure & render

    std::string content;
    inputFieldFilterCb_t filter;
    std::size_t caretIndex = 0;

    float scrollX = 0.f;
    float scrollY = 0.f;
    float padding = 5.f;

    bool focused = false;

    sf::Clock caretClock;
    bool showCaret = true;

    float lineHeight;

    std::vector<std::string> splitLines() const;
    sf::Vector2f getCaretPixelPos() const;

    void updateScroll();
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

public:
    MultilineTextField(
        sf::Vector2f size,
        sf::Vector2f pos,
        const sf::Font& font = Global::numberFont,
        unsigned int charSize = 22
    );

    void handleEvent(const std::optional<sf::Event>& e) override;
    bool containPos(sf::Vector2f pos) override;

    void update();
    void setFilter(inputFieldFilterCb_t filter);
    std::string getValue();
};

}

#include "GUI-interactable-element.ipp"

#endif // UI_GUI_INTERACTABLE_ELEMENT_H