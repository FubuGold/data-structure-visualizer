#include "../../include/UI/GUI-interactable-element.h"

namespace GUI
{

void IInteractableElement::setWindow(sf::RenderTarget *target_ptr)
{
    this->target_ptr = target_ptr;
}

void IInteractableElement::lock()
{
    this->locked = true;
}

void IInteractableElement::unlock()
{
    this->locked = false;
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
    sf::Color bgColor,
    sf::Color textColor,
    sf::Color borderColor
  ) : text(Global::textFont)
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
    if (locked) return;

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

void RectangleButton::setString(const std::string &s)
{
    this->text.setString(s);

    this->text.setOrigin(this->text.getLocalBounds().position + this->text.getLocalBounds().size * 0.5f);
    this->text.setPosition(this->rect.getGlobalBounds().position + this->rect.getLocalBounds().size * 0.5f);
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
        if (value.size()) value.erase(value.size()-1);
    }
    else if (value.size() < maxChar && (!this->filter || this->filter(chr))) {
        value.insert(value.size(), sf::String(chr));
    }

    text.setString(value);
}

// Public functions 

TextInputField::TextInputField(
    sf::Vector2f fieldSize,
    sf::Vector2f fieldPos,
    sf::Vector2f textOffset,
    int maxChar,
    int characterSize,
    int textOutline,
    int borderThickness,
    sf::Color bgColor,
    sf::Color textColor,
    sf::Color borderColor
) : value(""), focused(0), text(Global::numberFont)
{
    this->rect.setSize(fieldSize);

    this->rect.setPosition(fieldPos);
    this->rect.setFillColor(bgColor);
    this->rect.setOutlineThickness(borderThickness);
    this->rect.setOutlineColor(borderColor);

    this->text.setOutlineThickness(textOutline);
    this->text.setCharacterSize(characterSize);
    this->text.setFillColor(textColor);

    this->maxChar = maxChar;

    std::string tmp = "";
    for (int i=0;i<maxChar;i++) tmp = std::move(tmp) + "0";
    this->text.setString(tmp);
    // this->text.setOrigin(
    //     {this->text.getLocalBounds().position.x,
    //         this->text.getLocalBounds().position.y + this->text.getLocalBounds().size.y * 0.5f}
    // );
    this->text.setOrigin(this->text.getLocalBounds().position + this->text.getLocalBounds().size * 0.5f);
    this->text.setString("");
    this->text.setPosition(fieldPos + fieldSize * 0.5f + textOffset);

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
    if (locked) return;

    if (const sf::Event::MouseMoved *mouseMoved = e->getIf<sf::Event::MouseMoved>()) {
        sf::Vector2f mousePos = this->target_ptr->mapPixelToCoords(mouseMoved->position);
        if (containPos(mousePos)) this->hoverIn();
        else this->hoverOut();
    }
    else if (const sf::Event::MouseButtonPressed *mousePressed = e->getIf<sf::Event::MouseButtonPressed>()) {
        sf::Vector2f mousePos = this->target_ptr->mapPixelToCoords(mousePressed->position);
        if (mousePressed->button == sf::Mouse::Button::Left) {
            if (containPos(mousePos)) this->click();
            else this->focused = 0;
        }
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

//======================================================//

// Horizontal slider implemenentation

HSlider::HSlider(
    sf::Vector2f size,
    sf::Vector2f pos,
    int numSteps,
    int startValue,
    int endValue,
    int borderThickness,
    sf::Color bgColor,
    sf::Color progressColor,
    sf::Color borderColor
)
{
    this->size = size;
    this->pos = pos;
    
    this->numSteps = numSteps;
    this->startValue = startValue;
    this->endValue = endValue;

    this->valueStep = (endValue - startValue) / numSteps;
    // this->sizeStep = size.x / numSteps;

    this->bgRect.setSize(size);
    this->bgRect.setPosition(pos);
    this->bgRect.setFillColor(bgColor);
    this->bgRect.setOutlineThickness(borderThickness);
    this->bgRect.setOutlineColor(borderColor);

    this->progressRect.setPosition(pos);
    this->progressRect.setFillColor(progressColor);
    this->progressRect.setSize({0,size.y});
}

void HSlider::draw(sf::RenderTarget &target, sf::RenderStates state) const
{
    target.draw(bgRect);
    target.draw(progressRect);
}

int HSlider::findSegment(sf::Vector2f pos)
{
    int diff = (pos.x - this->pos.x);
    if (diff < 0) diff = 0;
    if (diff > this->size.x) diff = this->size.x;
    
    float curStepF = diff * 1.0 * numSteps / size.x;
    int curStep = curStepF;
    if (curStepF - curStep >= 0.5) curStep++;
    
    return std::min(curStep, this->numSteps);
}

void HSlider::updateValue(int segment)
{
    this->curValue = this->startValue + this->valueStep * segment;
    this->progressRect.setSize({this->size.x * segment / numSteps,this->size.y});

    if (changeCallback) changeCallback(this->curValue);
}

bool HSlider::containPos(sf::Vector2f pos)
{
    return this->bgRect.getGlobalBounds().contains(pos);
}

void HSlider::handleEvent(const std::optional<sf::Event>& e)
{
    if (locked) return;

    if (const sf::Event::MouseMoved *mouseMoved = e->getIf<sf::Event::MouseMoved>()) {
        sf::Vector2f mousePos = this->target_ptr->mapPixelToCoords(mouseMoved->position);
        if (containPos(mousePos)) this->hoverIn();
        else this->hoverOut();
        if (this->clicked) {
            this->updateValue(this->findSegment(mousePos));
        }
    }
    else if (const sf::Event::MouseButtonPressed *mousePressed = e->getIf<sf::Event::MouseButtonPressed>()) {
        sf::Vector2f mousePos = this->target_ptr->mapPixelToCoords(mousePressed->position);
        if (mousePressed->button == sf::Mouse::Button::Left && containPos(mousePos)) {
            this->click();
            this->updateValue(this->findSegment(mousePos));
        }
    }
    else if (const sf::Event::MouseButtonReleased *mouseReleased = e->getIf<sf::Event::MouseButtonReleased>()) {
        sf::Vector2f mousePos = this->target_ptr->mapPixelToCoords(mouseReleased->position);
        if (this->clicked) this->clicked = false;
        if (mouseReleased->button == sf::Mouse::Button::Left && containPos(mousePos)) this->release();
    }
}

void HSlider::setChangeCb(sliderUpdateCb_t cb)
{
    this->changeCallback = cb;
}

float HSlider::getValue()
{
    return this->curValue;
}

}