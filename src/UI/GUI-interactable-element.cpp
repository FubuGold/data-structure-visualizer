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
    this->baseColor = bgColor;
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

void RectangleButton::lock()
{
    IInteractableElement::lock();
    this->rect.setFillColor(disableColor);
}

void RectangleButton::unlock()
{
    IInteractableElement::unlock();
    this->rect.setFillColor(baseColor);
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
    this->baseColor = bgColor;
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

void TextInputField::lock()
{
    IInteractableElement::lock();
    this->rect.setFillColor(disableColor);
}

void TextInputField::unlock()
{
    IInteractableElement::unlock();
    this->rect.setFillColor(baseColor);
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

    // if (changeCallback) changeCallback(this->curValue);
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
            if (changeCallback) changeCallback(this->curValue);
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

void HSlider::setValue(float val)
{
    std::cerr << "Slider set value: " << startValue << ' ' << endValue << ' ' << val << '\n';
    assert(val >= startValue && val <= endValue);
    
    float curStepF = (val - startValue) / valueStep;
    int curStep = curStepF;
    if (curStepF - curStep >= 0.5) curStep++;
    
    this->curValue = this->startValue + this->valueStep * curStep;
    this->progressRect.setSize({this->size.x * curStep / numSteps,this->size.y});
    // if (changeCallback) changeCallback(this->curValue);
}

void HSlider::setNewRange(float startValue, float endValue, int numSteps)
{
    this->numSteps = numSteps;
    this->startValue = startValue;
    this->endValue = endValue;

    this->valueStep = (endValue - startValue) / numSteps;
}

//======================================================//

// Multi-line text field implementation

MultilineTextField::MultilineTextField(
    sf::Vector2f size,
    sf::Vector2f pos,
    const sf::Font& font, 
    unsigned int charSize
) : textTemplate(font)
{
    textTemplate.setFont(font);
    textTemplate.setCharacterSize(charSize);
    textTemplate.setFillColor(Global::colorSet[0][Global::COLOR_TYPE::NETURAL]);

    lineHeight = textTemplate.getCharacterSize() * 1.3f;

    box.setFillColor(Global::colorSet[0][Global::COLOR_TYPE::BACKGROUND]);
    box.setOutlineColor(Global::colorSet[0][Global::COLOR_TYPE::NETURAL]);
    box.setOutlineThickness(1.f);
    box.setSize(size);
    box.setPosition(pos);

    caretShape.setSize({1.f,lineHeight});
    caretShape.setFillColor(Global::colorSet[0][Global::COLOR_TYPE::NETURAL]);
}

std::vector<std::string> MultilineTextField::splitLines() const
{
    std::vector<std::string> lines;
    std::string current;

    for (char c : content) {
        if (c == '\n') {
            lines.push_back(current);
            current.clear();
        }
        else {
            current.push_back(c);
        }
    }

    lines.push_back(current);
    return lines;
}

sf::Vector2f MultilineTextField::getCaretPixelPos() const
{
    std::vector<std::string> lines = splitLines();

    size_t index = caretIndex;
    size_t row = 0;

    for (size_t i = 0; i < lines.size(); ++i) {
        if (index <= lines[i].size()) {
            row = i;
            break;
        }
        index -= (lines[i].size() + 1);
    }

    size_t col = index;

    sf::Text temp = textTemplate;
    temp.setString(lines[row].substr(0, col));

    float x = temp.getLocalBounds().size.x;
    float y = row * lineHeight;
    
    return {x, y};
}

void MultilineTextField::updateScroll()
{
    sf::Vector2f caret = getCaretPixelPos();

    float visibleW = box.getSize().x - 2 * padding;
    float visibleH = box.getSize().y - 2 * padding;

    // Horizontal
    if (caret.x - scrollX > visibleW)
        scrollX = caret.x - visibleW;
    else if (caret.x - scrollX < 0)
        scrollX = caret.x;

    // Vertical
    if (caret.y - scrollY > visibleH - lineHeight)
        scrollY = caret.y - (visibleH - lineHeight);
    else if (caret.y - scrollY < 0)
        scrollY = caret.y;

    caretShape.setPosition({
        box.getPosition().x + padding + caret.x - scrollX + 1,
        box.getPosition().y + padding + caret.y - scrollY
    });
}

void MultilineTextField::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(box, states);

    sf::View oldView = target.getView();

    sf::FloatRect viewport(
        {box.getPosition().x / target.getSize().x, box.getPosition().y / target.getSize().y},
        {box.getSize().x / target.getSize().x, box.getSize().y / target.getSize().y}
    );

    sf::View clipView = oldView;
    clipView.setScissor(viewport);
    target.setView(clipView);

    std::vector<std::string> lines = splitLines();

    for (size_t i = 0; i < lines.size(); ++i) {
        sf::Text line = textTemplate;
        line.setString(lines[i]);

        line.setPosition({
            box.getPosition().x + padding - scrollX,
            box.getPosition().y + padding + i * lineHeight - scrollY
        });

        target.draw(line, states);
    }

    // caret
    if (focused && showCaret) {
        target.draw(caretShape, states);
    }

    target.setView(oldView);
}

bool MultilineTextField::containPos(sf::Vector2f pos)
{
    return box.getGlobalBounds().contains(pos);
}

void MultilineTextField::handleEvent(const std::optional<sf::Event>& e)
{
    if (locked) return;

    if (const auto* pressed = e->getIf<sf::Event::MouseButtonPressed>()) {
        sf::Vector2f mouse = target_ptr->mapPixelToCoords(pressed->position);
        focused = containPos(mouse);
    }

    if (!focused) return;

    if (const auto* textEntered = e->getIf<sf::Event::TextEntered>()) {
        if (textEntered->unicode == 13) // Enter
        {
            content.insert(caretIndex, "\n");
            caretIndex++;
        }
        else if (this->filter && this->filter(textEntered->unicode))
        {
            content.insert(content.begin() + caretIndex, (char)textEntered->unicode);
            caretIndex++;
        }
    }
    else if (const auto* key = e->getIf<sf::Event::KeyPressed>()) {
        if (key->code == sf::Keyboard::Key::Backspace && caretIndex > 0) {
            content.erase(caretIndex - 1, 1);
            caretIndex--;
        }
        else if (key->code == sf::Keyboard::Key::Left && caretIndex > 0)
            caretIndex--;
        else if (key->code == sf::Keyboard::Key::Right && caretIndex < content.size())
            caretIndex++;
        else if (key->code == sf::Keyboard::Key::Up)
            caretIndex = std::max((int)caretIndex - 10, 0); 
        else if (key->code == sf::Keyboard::Key::Down)
            caretIndex = std::min((int)content.size(), (int)caretIndex + 10);
    }

    updateScroll();
}

void MultilineTextField::update()
{
    if (!focused) return;

    if (caretClock.getElapsedTime().asSeconds() > 0.5f) {
        showCaret = !showCaret;
        caretClock.restart();
    }
}

void MultilineTextField::setFilter(inputFieldFilterCb_t filter)
{
    this->filter = filter;
}

std::string MultilineTextField::getValue()
{
    return content;
}

}