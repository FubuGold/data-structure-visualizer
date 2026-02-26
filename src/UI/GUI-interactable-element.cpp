#include "../../include/UI/GUI-interactable-element.h"

namespace GUI
{

void IInteractableElement::setWindow(sf::RenderTarget *target_ptr)
{
    this->target_ptr = target_ptr;
}

//======================================================//

// Rectangle button implementation

// Protected functions

void RectangleButton::onClick()
{
    if (this->onClickCallback) this->onClickCallback(rect,text);
}
void RectangleButton::onRelease()
{
    if (this->onReleaseCallback) this->onReleaseCallback(rect,text);
}
void RectangleButton::onHoverIn()
{
    if (this->onHoverInCallback) this->onHoverInCallback(rect,text);
}
void RectangleButton::onHoverOut()
{
    if (this->onHoverOutCallback) this->onHoverOutCallback(rect,text);
}

void RectangleButton::draw(sf::RenderTarget& target, sf::RenderStates state) const
{
    target.draw(this->rect,state);
    target.draw(this->text,state);
}


// Public functions

RectangleButton::RectangleButton(
    sf::Vector2f btnSize,
    sf::Vector2f btnPos,
    std::string text,
    int characterSize,
    int textOutline,
    int borderThickness,
    sf::Color borderColor,
    sf::Color bgColor,
    sf::Color textColor
  ) : text(Config::font)
{
    this->rect.setSize(btnSize);

    this->rect.setPosition(btnPos);
    this->rect.setFillColor(bgColor);
    this->rect.setOutlineThickness(borderThickness);
    this->rect.setOutlineColor(borderColor);

    this->text.setString(text);
    this->text.setOutlineThickness(textOutline);
    this->text.setCharacterSize(characterSize);
    this->text.setFillColor(textColor);

    this->text.setOrigin(this->text.getLocalBounds().position + this->text.getLocalBounds().size * 0.5f);
    this->text.setPosition(btnPos + btnSize * 0.5f);
}

bool RectangleButton::containPos(sf::Vector2f pos)
{
    return rect.getGlobalBounds().contains(pos);
}

void RectangleButton::handleEvent(const std::optional<sf::Event>& e)
{
    if (const sf::Event::MouseMoved *mouseMoved = e->getIf<sf::Event::MouseMoved>()) {
        sf::Vector2f mousePos = this->target_ptr->mapPixelToCoords(mouseMoved->position);
        if (containPos(mousePos)) this->hoverIn();
        else this->hoverOut();
    }
    else if (const sf::Event::MouseButtonPressed *mousePressed = e->getIf<sf::Event::MouseButtonPressed>()) {
        sf::Vector2f mousePos = this->target_ptr->mapPixelToCoords(mousePressed->position);
        if (mousePressed->button == sf::Mouse::Button::Left && containPos(mousePos)) this->click();
    }
    else if (const sf::Event::MouseButtonReleased *mouseReleased = e->getIf<sf::Event::MouseButtonReleased>()) {
        sf::Vector2f mousePos = this->target_ptr->mapPixelToCoords(mouseReleased->position);
        if (mouseReleased->button == sf::Mouse::Button::Left && containPos(mousePos)) this->release();
    }
}

//======================================================//

// TextInputField class implementation

// Protected funtions

void TextInputField::click()
{
    if (clicked) return;
    if (!hovered) focused = 0;
    else {
        focused = 1;
        onClick();
    }
}

void TextInputField::draw(sf::RenderTarget& target, sf::RenderStates state) const
{
    target.draw(this->rect,state);
    target.draw(this->text,state);

    // for (int i=0;i<debugDots.size();i++) {
    //     target.draw(debugDots[i], state);
    // }
}

void TextInputField::updateValue(char32_t chr)
{
    // Backspace
    if (chr == 8) {
        if (value.getSize()) value.erase(value.getSize()-1);
    }
    else if (!this->filter || this->filter(chr)) {
        value.insert(value.getSize(), sf::String(chr));
    }

    text.setString(value);
}

// Public functions 

TextInputField::TextInputField(
    sf::Vector2f fieldSize,
    sf::Vector2f fieldPos,
    int characterSize,
    int textOutline,
    int borderThickness,
    sf::Color borderColor,
    sf::Color bgColor,
    sf::Color textColor
) : value(""), focused(0), text(Config::font)
{
    this->rect.setSize(fieldSize);

    this->rect.setPosition(fieldPos);
    this->rect.setFillColor(bgColor);
    this->rect.setOutlineThickness(borderThickness);
    this->rect.setOutlineColor(borderColor);

    this->text.setOutlineThickness(textOutline);
    this->text.setCharacterSize(characterSize);
    this->text.setFillColor(textColor);

    // To anyone who read this: The trick here is to set a character to init a local bound, then calculate on that local bound for the origin, then empty the string
    this->text.setString("0");
    this->text.setOrigin(
        {this->text.getLocalBounds().position.x,
            this->text.getLocalBounds().position.y + this->text.getLocalBounds().size.y * 0.5f}
    );
    this->text.setString("");
    this->text.setPosition({fieldPos.x, fieldPos.y + fieldSize.y * 0.5f});

    // debugDots.push_back(DebugDot(this->text.getPosition()));
}

void TextInputField::setFilter(inputFieldFilterCb_t filter)
{
    this->filter = filter;
}

sf::String TextInputField::getValue()
{
    return this->value;
}

bool TextInputField::containPos(sf::Vector2f pos)
{
    return this->rect.getGlobalBounds().contains(pos);
}

void TextInputField::handleEvent(const std::optional<sf::Event>& e)
{
    if (const sf::Event::MouseMoved *mouseMoved = e->getIf<sf::Event::MouseMoved>()) {
        sf::Vector2f mousePos = this->target_ptr->mapPixelToCoords(mouseMoved->position);
        if (containPos(mousePos)) this->hoverIn();
        else this->hoverOut();
    }
    else if (const sf::Event::MouseButtonPressed *mousePressed = e->getIf<sf::Event::MouseButtonPressed>()) {
        sf::Vector2f mousePos = this->target_ptr->mapPixelToCoords(mousePressed->position);
        if (mousePressed->button == sf::Mouse::Button::Left && containPos(mousePos)) this->click();
    }
    else if (const sf::Event::MouseButtonReleased *mouseReleased = e->getIf<sf::Event::MouseButtonReleased>()) {
        sf::Vector2f mousePos = this->target_ptr->mapPixelToCoords(mouseReleased->position);
        if (mouseReleased->button == sf::Mouse::Button::Left && containPos(mousePos)) this->release();
    }
    else if (const sf::Event::TextEntered *textEntered = e->getIf<sf::Event::TextEntered>())
    {
        if (!this->focused) return;
        this->updateValue(textEntered->unicode);
    }
}

}