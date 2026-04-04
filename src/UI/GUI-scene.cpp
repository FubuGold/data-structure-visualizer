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

using Global::toInt;

namespace GUI
{

// Scene base class implementation

template<typename T>
requires std::derived_from<T,sf::Drawable>
void Scene::addElement(std::shared_ptr<T> element_ptr)
{
    std::cerr << "Adding element\n";
    elements.push_back(std::static_pointer_cast<sf::Drawable>(element_ptr));
    if constexpr (std::derived_from<T,IInteractableElement>) {
        interactableElements.push_back(std::dynamic_pointer_cast<IInteractableElement>(element_ptr));
    }
}

void Scene::handleEvent(const std::optional<sf::Event>& e)
{
    for (std::shared_ptr<IInteractableElement> element : interactableElements) {
        element->handleEvent(e);
    }
}

void Scene::setup()
{
    for (std::shared_ptr<IInteractableElement> element : interactableElements) {
        element->setWindow(target_ptr);
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

    this->avl = std::make_shared<GUI::TreeVisualHandler>(sf::Vector2f{800, 500}, sf::Vector2f{50, 200});
    addElement(avl);

    avlHandler.setVisualizer(this->avl);

    // std::shared_ptr<ValueText<sf::String>> valueText(new ValueText<sf::String>(
    //     {700, 600}
    // ));
    // addElement(valueText);

    std::shared_ptr<TextInputField> textField(new TextInputField(
        {100, 20}, {100, 50}, {0,0}, 10,
        20, 0, 2,
        sf::Color::White, sf::Color::Magenta, sf::Color::Black
    ));
    textField->setFilter([](char32_t chr) -> bool {
        if (chr < '0' || chr > '9') return false;
        return true;
    });
    addElement(textField);

    // static int cnt = 0;

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

    std::shared_ptr<RectangleButton> clearButton(new RectangleButton(
        {100, 20}, {950, 600},
        "Clear",
        20, 0, 2
    ));
    clearButton->setReleaseCallback([this](sf::RectangleShape& rect,sf::Text& text) {
        avlHandler.clear();
    });
    addElement(clearButton);

    std::shared_ptr<ValueText<int>> output = std::make_shared<ValueText<int>>(sf::Vector2f(950,20), 20);
    (*output) = (int)Global::animationSpeed;
    addElement(output);

    std::shared_ptr<HSlider> slider = std::make_shared<HSlider>(
        sf::Vector2f(500,25),sf::Vector2f(400,20),
        5, 1, 6
    );
    slider->setChangeCb([output](float value) {
        (*output) = (int)value;
        Global::animationSpeed = value;
    });
    addElement(slider);


    // This must be last
    Scene::setup();
    

    std::cerr << "Setup completed\n";
}

void DebugScene::loopUpdate()
{
    // std::cerr << "======================================================" << '\n';
    // std::cerr << "Update run\n";
    this->avlHandler.loop();
    // std::cerr << "Update end\n";
}

//======================================================//

// Menu scene implementation

void MenuScene::setup()
{
    for (int i=0;i<6;i++) {
        if (!thumbnailImg[i].loadFromFile("./asset/thumbnail/" + filename[i])) {
            std::cerr << "Failed to load: " << i << '\n';
        }
    }

    //======================================================//
    // Title

    std::shared_ptr<sf::RectangleShape> titleBg = std::make_shared<sf::RectangleShape>();
    titleBg->setFillColor(Global::colorSet[0][Global::MAIN]);
    titleBg->setSize({1280,153});
    titleBg->setPosition({0,0});
    addElement(titleBg);

    std::shared_ptr<sf::Text> title1 = std::make_shared<sf::Text>(Global::textFont), title2 = std::make_shared<sf::Text>(Global::textFont);
    title1->setCharacterSize(50);
    title1->setFillColor(sf::Color::White);
    title1->setString("Data Structure");
    title1->setOrigin(title1->getLocalBounds().position);
    title1->setPosition({300,61});
    addElement(title1);
    
    title2->setCharacterSize(50);
    title2->setFillColor(sf::Color::Black);
    title2->setString("Visualizer");
    title2->setOrigin(title2->getLocalBounds().position);
    title2->setPosition({727,61});
    addElement(title2);

    //======================================================//
    // Singly linked list

    std::shared_ptr<GUI::RectangleButton> sllButton = std::make_shared<GUI::RectangleButton>(
        sf::Vector2f(346,248), sf::Vector2f(66,203),
        "",20,0,0,
        sf::Color::Transparent, sf::Color::Transparent, sf::Color::Transparent
    );
    sllButton->setReleaseCallback([](sf::RectangleShape &rect, sf::Text &text){

    });
    addElement(sllButton);

    std::shared_ptr<GUI::RectangleButton> sllTitle = std::make_shared<GUI::RectangleButton>(
        sf::Vector2f(346,68), sf::Vector2f(66,383),
        "Singly Linked List", 30, 0, 0,
        Global::colorSet[0][Global::MAIN], sf::Color::White
    );
    sllTitle->lock();
    addElement(sllTitle);

    std::shared_ptr<sf::Sprite> sllThumbnail = std::make_shared<sf::Sprite>(thumbnailImg[SLL]);
    sllThumbnail->setPosition({66,203});
    addElement(sllThumbnail);

    //======================================================//
    // Heap

    std::shared_ptr<GUI::RectangleButton> heapButton = std::make_shared<GUI::RectangleButton>(
        sf::Vector2f(346,248), sf::Vector2f(467,203),
        "",20,0,0,
        sf::Color::Transparent, sf::Color::Transparent, sf::Color::Transparent
    );
    heapButton->setReleaseCallback([](sf::RectangleShape &rect, sf::Text &text){

    });
    addElement(sllButton);

    std::shared_ptr<GUI::RectangleButton> heapTitle = std::make_shared<GUI::RectangleButton>(
        sf::Vector2f(346,68), sf::Vector2f(467,383),
        "Heap", 30, 0, 0,
        Global::colorSet[0][Global::MAIN], sf::Color::White
    );
    heapTitle->lock();
    addElement(heapTitle);

    std::shared_ptr<sf::Sprite> heapThumbnail = std::make_shared<sf::Sprite>(thumbnailImg[HEAP]);
    heapThumbnail->setPosition({467,203});
    addElement(heapThumbnail);

    //======================================================//
    // AVL tree

    std::shared_ptr<GUI::RectangleButton> avlButton = std::make_shared<GUI::RectangleButton>(
        sf::Vector2f(346,248), sf::Vector2f(868,203),
        "",20,0,0,
        sf::Color::Transparent, sf::Color::Transparent, sf::Color::Transparent
    );
    avlButton->setReleaseCallback([](sf::RectangleShape &rect, sf::Text &text){
        Global::curAppState = Global::SceneState::AVL;
    });
    addElement(avlButton);

    std::shared_ptr<GUI::RectangleButton> avlTitle = std::make_shared<GUI::RectangleButton>(
        sf::Vector2f(346,68), sf::Vector2f(868,383),
        "AVL Tree", 30, 0, 0,
        Global::colorSet[0][Global::MAIN], sf::Color::White
    );
    avlTitle->lock();
    addElement(avlTitle);

    std::shared_ptr<sf::Sprite> avlThumbnail = std::make_shared<sf::Sprite>(thumbnailImg[AVL]);
    avlThumbnail->setPosition({868,203});
    addElement(avlThumbnail);

    //======================================================//
    // Trie

    std::shared_ptr<GUI::RectangleButton> trieButton = std::make_shared<GUI::RectangleButton>(
        sf::Vector2f(346,248), sf::Vector2f(66,477),
        "",20,0,0,
        sf::Color::Transparent, sf::Color::Transparent, sf::Color::Transparent
    );
    trieButton->setReleaseCallback([](sf::RectangleShape &rect, sf::Text &text){

    });
    addElement(trieButton);

    std::shared_ptr<GUI::RectangleButton> trieTitle = std::make_shared<GUI::RectangleButton>(
        sf::Vector2f(346,68), sf::Vector2f(66,657),
        "Trie", 30, 0, 0,
        Global::colorSet[0][Global::MAIN], sf::Color::White
    );
    trieTitle->lock();
    addElement(trieTitle);

    std::shared_ptr<sf::Sprite> trieThumbnail = std::make_shared<sf::Sprite>(thumbnailImg[TRIE]);
    trieThumbnail->setPosition({66,477});
    addElement(trieThumbnail);

    //======================================================//
    // Shortest path

    std::shared_ptr<GUI::RectangleButton> spButton = std::make_shared<GUI::RectangleButton>(
        sf::Vector2f(346,248), sf::Vector2f(467,477),
        "",20,0,0,
        sf::Color::Transparent, sf::Color::Transparent, sf::Color::Transparent
    );
    spButton->setReleaseCallback([](sf::RectangleShape &rect, sf::Text &text){

    });
    addElement(spButton);

    std::shared_ptr<GUI::RectangleButton> spTitle = std::make_shared<GUI::RectangleButton>(
        sf::Vector2f(346,68), sf::Vector2f(467,657),
        "Shortest Path", 30, 0, 0,
        Global::colorSet[0][Global::MAIN], sf::Color::White
    );
    spTitle->lock();
    addElement(spTitle);

    std::shared_ptr<sf::Sprite> spThumbnail = std::make_shared<sf::Sprite>(thumbnailImg[SP]);
    spThumbnail->setPosition({467,477});
    addElement(spThumbnail);

    //======================================================//
    // Shortest path

    std::shared_ptr<GUI::RectangleButton> mstButton = std::make_shared<GUI::RectangleButton>(
        sf::Vector2f(346,248), sf::Vector2f(868,477),
        "",20,0,0,
        sf::Color::Transparent, sf::Color::Transparent, sf::Color::Transparent
    );
    mstButton->setReleaseCallback([](sf::RectangleShape &rect, sf::Text &text){

    });
    addElement(mstButton);

    std::shared_ptr<GUI::RectangleButton> mstTitle = std::make_shared<GUI::RectangleButton>(
        sf::Vector2f(346,68), sf::Vector2f(868,657),
        "Shortest Path", 30, 0, 0,
        Global::colorSet[0][Global::MAIN], sf::Color::White
    );
    mstTitle->lock();
    addElement(mstTitle);

    std::shared_ptr<sf::Sprite> mstThumbnail = std::make_shared<sf::Sprite>(thumbnailImg[MST]);
    mstThumbnail->setPosition({868,477});
    addElement(mstThumbnail);

    Scene::setup();
}

//======================================================//

// Data structure visual scene implementation

void VisualScene::setup()
{

    std::cerr << "Visual scene setup start\n";

    //======================================================//
    // Header bar

    std::shared_ptr<sf::RectangleShape> headerBarBg = std::make_shared<sf::RectangleShape>();
    headerBarBg->setFillColor(Global::colorSet[0][Global::COLOR_TYPE::MAIN]);
    headerBarBg->setSize({1280,80});
    addElement(headerBarBg);

    this->inputField = std::make_shared<GUI::TextInputField>(
        sf::Vector2f(164,40), sf::Vector2f(24,20), sf::Vector2f(0,0),
        9, 22, 0, 2,
        Global::colorSet[0][Global::COLOR_TYPE::BACKGROUND], 
        Global::colorSet[0][Global::COLOR_TYPE::NETURAL], 
        Global::colorSet[0][Global::COLOR_TYPE::NETURAL]
    );
    this->inputField->setFilter([](char32_t chr) -> bool {
        return chr >= '0' && chr <= '9';
    });
    addElement(this->inputField);

    // std::cerr << "Input field added\n";

    this->insertBtn = std::make_shared<GUI::RectangleButton>(
        sf::Vector2f(110,40), sf::Vector2f(208,20),
        "Insert", 22, 0, 2,
        Global::colorSet[0][Global::COLOR_TYPE::BACKGROUND], 
        Global::colorSet[0][Global::COLOR_TYPE::NETURAL], 
        Global::colorSet[0][Global::COLOR_TYPE::NETURAL]
    );
    addElement(this->insertBtn);

    std::cerr << "Insert added\n";

    this->removeBtn = std::make_shared<GUI::RectangleButton>(
        sf::Vector2f(110,40), sf::Vector2f(336,20),
        "Delete", 22, 0, 2,
        Global::colorSet[0][Global::COLOR_TYPE::BACKGROUND], 
        Global::colorSet[0][Global::COLOR_TYPE::NETURAL], 
        Global::colorSet[0][Global::COLOR_TYPE::NETURAL]
    );
    addElement(this->removeBtn);

    // std::cerr << "Remove added\n";

    this->findBtn = std::make_shared<GUI::RectangleButton>(
        sf::Vector2f(110,40), sf::Vector2f(464,20),
        "Find", 22, 0, 2,
        Global::colorSet[0][Global::COLOR_TYPE::BACKGROUND], 
        Global::colorSet[0][Global::COLOR_TYPE::NETURAL], 
        Global::colorSet[0][Global::COLOR_TYPE::NETURAL]
    );
    addElement(this->findBtn);

    // std::cerr << "Find added\n";

    this->updateBtn = std::make_shared<GUI::RectangleButton>(
        sf::Vector2f(110,40), sf::Vector2f(592,20),
        "Update", 22, 0, 2,
        Global::colorSet[0][Global::COLOR_TYPE::BACKGROUND], 
        Global::colorSet[0][Global::COLOR_TYPE::NETURAL], 
        Global::colorSet[0][Global::COLOR_TYPE::NETURAL]
    );
    addElement(this->updateBtn);
    
    // std::cerr << "Update added\n";

    this->updField = std::make_shared<GUI::TextInputField>(
        sf::Vector2f(203,40), sf::Vector2f(720,20), sf::Vector2f(10,0),
        9, 22, 0, 2,
        Global::colorSet[0][Global::COLOR_TYPE::BACKGROUND], 
        Global::colorSet[0][Global::COLOR_TYPE::NETURAL], 
        Global::colorSet[0][Global::COLOR_TYPE::NETURAL]
    );
    this->updField->setFilter([](char32_t chr) -> bool {
        return chr >= '0' && chr <= '9';
    });
    addElement(this->updField);

    // std::cerr << "Update field added\n";

    std::shared_ptr<sf::Text> updFieldText = std::make_shared<sf::Text>(Global::textFont);
    updFieldText->setString("To:");
    updFieldText->setCharacterSize(22);
    updFieldText->setFillColor(Global::colorSet[0][Global::COLOR_TYPE::NETURAL]);
    updFieldText->setOrigin(updFieldText->getLocalBounds().position + updFieldText->getLocalBounds().size * 0.5f);
    updFieldText->setPosition(sf::Vector2f(727,27) + sf::Vector2f(29,26) * 0.5f);
    addElement(updFieldText);

    this->clearBtn = std::make_shared<GUI::RectangleButton>(
        sf::Vector2f(110,40), sf::Vector2f(1008,20),
        "Clear", 22, 0, 2,
        Global::colorSet[0][Global::COLOR_TYPE::BACKGROUND], 
        Global::colorSet[0][Global::COLOR_TYPE::NETURAL], 
        Global::colorSet[0][Global::COLOR_TYPE::NETURAL]
    );
    addElement(this->clearBtn);

    this->randomBtn = std::make_shared<GUI::RectangleButton>(
        sf::Vector2f(110,40), sf::Vector2f(1135,20),
        "Random", 22, 0, 2,
        Global::colorSet[0][Global::COLOR_TYPE::BACKGROUND], 
        Global::colorSet[0][Global::COLOR_TYPE::NETURAL], 
        Global::colorSet[0][Global::COLOR_TYPE::NETURAL]
    );
    addElement(this->randomBtn);

    std::cerr << "Header bar completed\n";

    //======================================================//
    // Footer bar

    std::shared_ptr<sf::RectangleShape> footerBarBg = std::make_shared<sf::RectangleShape>();
    footerBarBg->setFillColor(Global::colorSet[0][Global::COLOR_TYPE::MAIN]);
    footerBarBg->setSize({1280,80});
    footerBarBg->setPosition({0,720});
    addElement(footerBarBg);

    this->fullUndoBtn = std::make_shared<GUI::RectangleButton>(
        sf::Vector2f(40,40), sf::Vector2f(24,740),
        "<<", 22, 0, 2,
        Global::colorSet[0][Global::COLOR_TYPE::BACKGROUND], 
        Global::colorSet[0][Global::COLOR_TYPE::NETURAL], 
        Global::colorSet[0][Global::COLOR_TYPE::NETURAL]
    );
    addElement(this->fullUndoBtn);

    this->undoBtn = std::make_shared<GUI::RectangleButton>(
        sf::Vector2f(40,40), sf::Vector2f(82,740),
        "<", 22, 0, 2,
        Global::colorSet[0][Global::COLOR_TYPE::BACKGROUND], 
        Global::colorSet[0][Global::COLOR_TYPE::NETURAL], 
        Global::colorSet[0][Global::COLOR_TYPE::NETURAL]
    );
    addElement(this->undoBtn);

    std::shared_ptr<sf::RectangleShape> animationStepBg = std::make_shared<sf::RectangleShape>();
    animationStepBg->setFillColor(Global::colorSet[0][Global::COLOR_TYPE::BACKGROUND]);
    animationStepBg->setOutlineColor(Global::colorSet[0][Global::COLOR_TYPE::NETURAL]);
    animationStepBg->setOutlineThickness(2);
    animationStepBg->setSize({528,40});
    animationStepBg->setPosition({140,740});
    addElement(animationStepBg);

    this->animationSlider = std::make_shared<GUI::HSlider>(
        sf::Vector2f(508,26), sf::Vector2f(150,747),
        1, 0, 1, 2,
        Global::colorSet[0][Global::COLOR_TYPE::BACKGROUND], 
        sf::Color(0xA0A0A0FF), 
        Global::colorSet[0][Global::COLOR_TYPE::NETURAL]
    );
    addElement(this->animationSlider);

    this->redoBtn = std::make_shared<GUI::RectangleButton>(
        sf::Vector2f(40,40), sf::Vector2f(686,740),
        ">", 22, 0, 2,
        Global::colorSet[0][Global::COLOR_TYPE::BACKGROUND], 
        Global::colorSet[0][Global::COLOR_TYPE::NETURAL], 
        Global::colorSet[0][Global::COLOR_TYPE::NETURAL]
    );
    addElement(this->redoBtn);

    this->fullRedoBtn = std::make_shared<GUI::RectangleButton>(
        sf::Vector2f(40,40), sf::Vector2f(744,740),
        ">>", 22, 0, 2,
        Global::colorSet[0][Global::COLOR_TYPE::BACKGROUND], 
        Global::colorSet[0][Global::COLOR_TYPE::NETURAL], 
        Global::colorSet[0][Global::COLOR_TYPE::NETURAL]
    );
    addElement(this->fullRedoBtn);

    std::cerr << "Added animation slider\n";

    std::shared_ptr<GUI::ValueText<std::string>> animationSpdDisplay = std::make_shared<GUI::ValueText<std::string>>(
        sf::Vector2f(872,747) + sf::Vector2f(7,-3),
        22, 0, Global::colorSet[0][Global::COLOR_TYPE::NETURAL]
    );
    *animationSpdDisplay = "1x";

    std::shared_ptr<GUI::RectangleButton> animationSpdBtn = std::make_shared<GUI::RectangleButton>(
        sf::Vector2f(110,40), sf::Vector2f(802,740),
        "", 22, 0, 2,
        Global::colorSet[0][Global::COLOR_TYPE::BACKGROUND], 
        Global::colorSet[0][Global::COLOR_TYPE::NETURAL], 
        Global::colorSet[0][Global::COLOR_TYPE::NETURAL]
    );
    animationSpdBtn->setReleaseCallback([animationSpdDisplay](sf::RectangleShape &rect, sf::Text &text) {
        Global::animationSpeed += 1;
        if (Global::animationSpeed > 5) Global::animationSpeed = 1;
        *animationSpdDisplay = std::to_string((int)Global::animationSpeed) + "x";
    });

    addElement(animationSpdBtn);
    addElement(animationSpdDisplay);

    std::shared_ptr<sf::Text> animationSpdText = std::make_shared<sf::Text>(Global::textFont);
    animationSpdText->setString("Speed");
    animationSpdText->setFillColor(Global::colorSet[0][Global::COLOR_TYPE::NETURAL]);
    animationSpdText->setCharacterSize(22);    
    animationSpdText->setOrigin(animationSpdText->getLocalBounds().position + animationSpdText->getLocalBounds().size * 0.5f);
    animationSpdText->setPosition(sf::Vector2f(805,748) + sf::Vector2f(70, 26) * 0.5f);
    addElement(animationSpdText);

    std::cerr << "Added speed button\n";

    this->fileBtn = std::make_shared<GUI::RectangleButton>(
        sf::Vector2f(110,40), sf::Vector2f(969,740),
        "File", 22, 0, 2,
        Global::colorSet[0][Global::COLOR_TYPE::BACKGROUND], 
        Global::colorSet[0][Global::COLOR_TYPE::NETURAL], 
        Global::colorSet[0][Global::COLOR_TYPE::NETURAL]
    );
    addElement(this->fileBtn);

    std::shared_ptr<GUI::RectangleButton> menuBtn = std::make_shared<GUI::RectangleButton>(
        sf::Vector2f(110,40), sf::Vector2f(1135,740),
        "Menu", 22, 0, 2,
        Global::colorSet[0][Global::COLOR_TYPE::BACKGROUND], 
        Global::colorSet[0][Global::COLOR_TYPE::NETURAL], 
        Global::colorSet[0][Global::COLOR_TYPE::NETURAL]
    );
    menuBtn->setReleaseCallback([this](sf::RectangleShape &rect, sf::Text &text){
        this->clear();
        Global::curAppState = Global::SceneState::MENU;
    });
    addElement(menuBtn);

    std::cerr << "Footer bar completed\n";

    Scene::setup();
}

//======================================================//

// AVL scene implementation

void AVLScene::setup()
{
    VisualScene::setup();

    std::cerr << "AVL scene setup start\n";

    this->treeVisual = std::make_shared<GUI::TreeVisualHandler>(sf::Vector2f{1280, 640}, sf::Vector2f{0, 80}, 60);
    avlHandler.setVisualizer(treeVisual);
    addElement(treeVisual);

    avlHandler.setAnimationSlider(this->animationSlider);
    this->animationSlider->lock();

    avlHandler.addLockableElement(this->insertBtn);
    avlHandler.addLockableElement(this->removeBtn);
    avlHandler.addLockableElement(this->findBtn);
    avlHandler.addLockableElement(this->updateBtn);
    avlHandler.addLockableElement(this->clearBtn);
    avlHandler.addLockableElement(this->randomBtn);
    avlHandler.addLockableElement(this->inputField);
    avlHandler.addLockableElement(this->updField);
    avlHandler.addLockableElement(this->fileBtn);
    avlHandler.addLockableElement(this->fullUndoBtn);
    avlHandler.addLockableElement(this->undoBtn);
    avlHandler.addLockableElement(this->redoBtn);
    // avlHandler.addLockableElement(this->fullRedoBtn);

    
    this->insertBtn->setReleaseCallback([this](sf::RectangleShape &rect, sf::Text &text){
        std::string value = this->inputField->getValue();
        if (value == "") return;
        int num = stringToInt(value);
        this->avlHandler.insert(num);
    });

    this->removeBtn->setReleaseCallback([this](sf::RectangleShape &rect, sf::Text &text){
        std::string value = this->inputField->getValue();
        if (value == "") return;
        int num = stringToInt(value);
        this->avlHandler.remove(num);
    });

    this->findBtn->setReleaseCallback([this](sf::RectangleShape &rect, sf::Text &text){
        std::string value = this->inputField->getValue();
        if (value == "") return;
        int num = stringToInt(value);
        this->avlHandler.find(num);
    });

    this->updateBtn->setReleaseCallback([this](sf::RectangleShape &rect, sf::Text &text){
        std::string value = this->inputField->getValue();
        std::string newValue = this->updField->getValue();
        if (value == "" || newValue == "") return;
        this->avlHandler.update(stringToInt(value), stringToInt(newValue));
    });

    this->randomBtn->setReleaseCallback([this](sf::RectangleShape &rect, sf::Text &text){
        this->avlHandler.random();
        this->avlHandler.endAnimation();
    });

    this->clearBtn->setReleaseCallback([this](sf::RectangleShape &rect, sf::Text &text){
        this->avlHandler.clear();
    });

    this->fullUndoBtn->setReleaseCallback([this](sf::RectangleShape &rect, sf::Text &text){
        this->avlHandler.fullUndo();
    });

    this->fullRedoBtn->setReleaseCallback([this](sf::RectangleShape &rect, sf::Text &text){
        this->avlHandler.endAnimation();
    });

    this->undoBtn->setReleaseCallback([this](sf::RectangleShape &rect, sf::Text &text){
        this->avlHandler.undo();
    });

    this->redoBtn->setReleaseCallback([this](sf::RectangleShape &rect, sf::Text &text){
        this->avlHandler.redo();
    });

    this->fileBtn->setReleaseCallback([this](sf::RectangleShape &rect, sf::Text &text){
        this->avlHandler.file();
    });

    std::vector<std::string> codeFilename = {
        "./asset/psuedo-code/avl/insert.txt",
        "./asset/psuedo-code/avl/find.txt",
        "./asset/psuedo-code/avl/delete.txt",
        "./asset/psuedo-code/avl/update.txt",
        "./asset/psuedo-code/avl/balancing.txt"
    };

    std::vector<std::string> funcName = {
        "Insert", "Find", "Delete", "Update", "Balance"
    };
    
    std::shared_ptr<GUI::CodeVisualHandler> codeVisual = std::make_shared<GUI::CodeVisualHandler>(
        sf::Vector2f(941,531), sf::Vector2f(339,20),
        14,
        funcName, codeFilename
    );
    addElement(codeVisual);

    avlHandler.setCodeVisualizer(codeVisual);

    Scene::setup();
}

void AVLScene::loopUpdate()
{
    avlHandler.loop();
}

}