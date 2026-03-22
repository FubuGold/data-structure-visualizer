#include "../../include/UI/GUI-scene.h"

#include <iostream>
#include <string>

int stringToInt(std::string s)
{
    int x = 0;
    for (int i=0;i<s.size();i++) {
        x = x * 10 + s[i] - '0';
    }
    return x;
}

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

    this->avl = std::make_shared<GUI::TreeVisualHandler>(sf::Vector2f{800, 500}, sf::Vector2f{50, 200});
    addElement(avl);

    avlHandler.setVisualizer(this->avl);

    // std::shared_ptr<ValueText<sf::String>> valueText(new ValueText<sf::String>(
    //     {700, 600}
    // ));
    // addElement(valueText);

    std::shared_ptr<TextInputField> textField(new TextInputField(
        {100, 20}, {100, 50},
        20, 0, 2,
        sf::Color::White, sf::Color::Magenta, sf::Color::Black
    ));
    addElement(textField);

    static int cnt = 0;

    std::shared_ptr<RectangleButton> inputEnter(new RectangleButton(
        {100, 20}, {220, 50},
        "Enter",
        20, 0, 2
    ));
    inputEnter->setReleaseCallback([textField, this](sf::RectangleShape& rect,sf::Text& text) {
        std::cerr << "Input registered\n";
        int x = stringToInt(textField->getValue());
        std::cerr << "Input processed\n";
        avlHandler.insert(x);
    });
    addElement(inputEnter);

    // std::shared_ptr<TextInputField> idField1(new TextInputField(
    //     {100, 20}, {450, 50},
    //     20, 0, 2,
    //     sf::Color::White, sf::Color::Magenta, sf::Color::Black
    // ));
    // idField1->setClickCallback([](){
    //     std::cerr << "ID field clicked : 1\n";
    // });
    // idField1->setHoverInCallback([](){
    //     std::cerr << "ID field hovered in : 1\n";
    // });
    // idField1->setHoverOutCallback([](){
    //     std::cerr << "ID field hovered out : 1\n";
    // });
    // addElement(idField1);

    // std::shared_ptr<TextInputField> idField2(new TextInputField(
    //     {100, 20}, {450, 70},
    //     20, 0, 2,
    //     sf::Color::White, sf::Color::Magenta, sf::Color::Black
    // ));
    // idField2->setClickCallback([](){
    //     std::cerr << "ID field clicked : 2\n";
    // });
    // idField2->setHoverInCallback([](){
    //     std::cerr << "ID field hovered in : 2\n";
    // });
    // idField2->setHoverOutCallback([](){
    //     std::cerr << "ID field hovered out : 2\n";
    // });
    // addElement(idField2);

    // std::shared_ptr<RectangleButton> idEnter(new RectangleButton(
    //     {100, 20}, {570, 60},
    //     "Enter",
    //     20, 0, 2
    // ));
    // idEnter->setReleaseCallback([idField1, idField2, this](sf::RectangleShape& rect,sf::Text& text) {
    //     int id1 = std::stoi( std::string(idField1->getValue()) );
    //     int id2 = std::stoi( std::string(idField2->getValue()) );
        
    //     avl->swap(id1,id2);
    // });
    // addElement(idEnter);

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

    std::shared_ptr<RectangleButton> clearButton(new RectangleButton(
        {100, 20}, {950, 600},
        "Clear",
        20, 0, 2
    ));
    clearButton->setReleaseCallback([this](sf::RectangleShape& rect,sf::Text& text) {
        avlHandler.clear();
    });
    addElement(clearButton);

    // This must be last
    for (std::shared_ptr<IInteractableElement> element : interactableElements) {
        element->setWindow(target_ptr);
    }
    
    std::cerr << "Setup completed\n";
}

void DebugScene::loopUpdate()
{
    // std::cerr << "======================================================" << '\n';
    // std::cerr << "Update run\n";
    this->avlHandler.loop();
    // std::cerr << "Update end\n";
}

}