#include "../../include/UI/GUI-scene.h"

#include <iostream>

namespace GUI
{

// Scene base class implementation

template<typename T>
requires std::derived_from<T,sf::Drawable>
void Scene::addElement(std::shared_ptr<T> element_ptr)
{
    elements.push_back(std::static_pointer_cast<sf::Drawable>(element_ptr));
    if constexpr (std::derived_from<T,IInteractableElement>) {
        interactableElements.push_back(std::static_pointer_cast<IInteractableElement>(element_ptr));
    }
}

void Scene::handleEvent(const std::optional<sf::Event>& e)
{
    for (std::shared_ptr<IInteractableElement> element : interactableElements) {
        element->handleEvent(e);
    }
}

void Scene::draw(sf::RenderTarget& target, sf::RenderStates state)
{
    for (std::shared_ptr<sf::Drawable> element : elements) {
        target.draw(*element,state);
    }

    // Debug
    for (int i=0;i<debugDots.size();i++) {
        target.draw(debugDots[i],state);
    }
}

void Scene::setWindow(sf::RenderTarget *target_ptr)
{
    this->target_ptr = target_ptr;
}

//======================================================//

// Debug scene implementation

void DebugScene::setup()
{
    // std::shared_ptr<RectangleButton> button1(new RectangleButton(
    //     {200, 50}, {300,500},
    //     "This is a button",
    //     20, 0, 2,
    //     sf::Color::White, sf::Color::Blue, sf::Color::Black
    // ));
    // addElement(button1);
    // button1->setClickCallback([](sf::RectangleShape &rect,sf::Text &text){
    //     std::cerr << "Click registered\n";
    // });
    // button1->setReleaseCallback([](sf::RectangleShape &rect,sf::Text &text){
    //     std::cerr << "Release registered\n";
    // });
    // button1->setHoverInCallback([](sf::RectangleShape &rect,sf::Text &text){
    //     std::cerr << "Hover in detected\n";
    // });
    // button1->setHoverOutCallback([](sf::RectangleShape &rect,sf::Text &text){
    //     std::cerr << "Hover out detected\n";
    // });

    std::shared_ptr<TreeHandler> tree(new TreeHandler({900, 600}, {100, 300}));
    addElement(tree);

    // std::shared_ptr<ValueText<sf::String>> valueText(new ValueText<sf::String>(
    //     {700, 600}
    // ));
    // addElement(valueText);

    std::shared_ptr<TextInputField> textField(new TextInputField(
        {350, 50}, {600, 50},
        20, 0, 2,
        sf::Color::White, sf::Color::Magenta, sf::Color::Black
    ));
    addElement(textField);

    static int cnt = 0;

    std::shared_ptr<RectangleButton> inputEnter(new RectangleButton(
        {100, 30}, {960, 50},
        "Enter",
        20, 0, 2
    ));
    inputEnter->setReleaseCallback([textField, tree](sf::RectangleShape& rect,sf::Text& text) {
        int pa = cnt ? (cnt-1) / 2 : -1;
        tree->insert(cnt,pa,(pa * 2 + 1) == cnt,textField->getValue());
        
        cnt++;
    });
    addElement(inputEnter);
    
    // std::shared_ptr<Node> nodeTest(new Node(
    //     {1000,800},
    //     "1",
    //     20
    // ));
    // addElement(nodeTest);
    
    // std::shared_ptr<Line> edgeTest(new Line(
    //     {340, 750}, {900, 102}, "abc"
    // ));
    // addElement(edgeTest);

    // This must be last
    for (std::shared_ptr<IInteractableElement> element : interactableElements) {
        element->setWindow(target_ptr);
    }
}

}