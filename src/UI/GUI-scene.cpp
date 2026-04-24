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
    // std::cerr << "Adding element\n";
    elements.push_back(std::static_pointer_cast<sf::Drawable>(element_ptr));
    if constexpr (std::derived_from<T,IInteractableElement>) {
        interactableElements.push_back(std::dynamic_pointer_cast<IInteractableElement>(element_ptr));
    }
}

void Scene::addZoomElement(std::shared_ptr<sf::Drawable> element_ptr)
{
    zoomElements.push_back(element_ptr);
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
    zoomView.setWindow(target_ptr);
    addElement(std::shared_ptr<GUI::ZoomView>(&zoomView));
}

void Scene::draw(sf::RenderWindow& target, sf::RenderStates state)
{
    for (std::shared_ptr<sf::Drawable> element : elements) {
        target.draw(*element,state);
    }

    // Debug
    // for (int i=0;i<debugDots.size();i++) {
    //     target.draw(debugDots[i],state);
    // }
}

void Scene::drawZoom()
{
    for (std::shared_ptr<sf::Drawable> element : zoomElements) {
        this->zoomView.draw(*element);
    }
}

void Scene::setWindow(sf::RenderWindow *target_ptr)
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

    std::shared_ptr<GUI::ThemeRectangle> titleBg = std::make_shared<GUI::ThemeRectangle>();
    // titleBg->setFillColor(Global::colorSet[0][Global::MAIN]);
    titleBg->rect.setSize({1280,100});
    titleBg->rect.setPosition({0,0});
    addElement(titleBg);

    std::shared_ptr<sf::Text> title1 = std::make_shared<sf::Text>(Global::textFont), title2 = std::make_shared<sf::Text>(Global::textFont);
    title1->setCharacterSize(50);
    title1->setFillColor(sf::Color::White);
    title1->setString("Data Structure");
    title1->setOrigin(title1->getLocalBounds().position);
    title1->setPosition({300,35});
    addElement(title1);
    
    title2->setCharacterSize(50);
    title2->setFillColor(sf::Color::Black);
    title2->setString("Visualizer");
    title2->setOrigin(title2->getLocalBounds().position);
    title2->setPosition({727,35});
    addElement(title2);

    //======================================================//
    // Singly linked list

    std::shared_ptr<GUI::RectangleButton> sllButton = std::make_shared<GUI::RectangleButton>(
        sf::Vector2f(346,248), sf::Vector2f(66,136),
        "",20,0,0,
        sf::Color::Transparent, sf::Color::Transparent, sf::Color::Transparent
    );
    sllButton->setReleaseCallback([](sf::RectangleShape &rect, sf::Text &text){
        Global::curAppState = Global::SceneState::SLL;
    });
    addElement(sllButton);

    std::shared_ptr<GUI::ThemeRectangle> sllTitleRect = std::make_shared<GUI::ThemeRectangle>(
        sf::Vector2f(346,68), sf::Vector2f(66,316)
    );
    addElement(sllTitleRect);

    std::shared_ptr<GUI::RectangleButton> sllTitle = std::make_shared<GUI::RectangleButton>(
        sf::Vector2f(346,68), sf::Vector2f(66,316),
        "Singly Linked List", 30, 0, 0,
        sf::Color::Transparent, sf::Color::White
    );
    // sllTitle->disableColor = Global::colorSet[0][Global::MAIN];
    // sllTitle->lock();
    addElement(sllTitle);

    std::shared_ptr<sf::Sprite> sllThumbnail = std::make_shared<sf::Sprite>(thumbnailImg[SLL]);
    sllThumbnail->setPosition({66,136});
    addElement(sllThumbnail);

    //======================================================//
    // Heap

    std::shared_ptr<GUI::RectangleButton> heapButton = std::make_shared<GUI::RectangleButton>(
        sf::Vector2f(346,248), sf::Vector2f(467,136),
        "",20,0,0,
        sf::Color::Transparent, sf::Color::Transparent, sf::Color::Transparent
    );
    heapButton->setReleaseCallback([](sf::RectangleShape &rect, sf::Text &text){
        Global::curAppState = Global::SceneState::HEAP;
    });
    addElement(heapButton);

    std::shared_ptr<GUI::ThemeRectangle> heapTitleRect = std::make_shared<GUI::ThemeRectangle>(
        sf::Vector2f(346,68), sf::Vector2f(467,316)
    );
    addElement(heapTitleRect);

    std::shared_ptr<GUI::RectangleButton> heapTitle = std::make_shared<GUI::RectangleButton>(
        sf::Vector2f(346,68), sf::Vector2f(467,316),
        "Heap", 30, 0, 0,
        sf::Color::Transparent, sf::Color::White
    );
    // heapTitle->disableColor = Global::colorSet[0][Global::MAIN];
    // heapTitle->lock();
    addElement(heapTitle);

    std::shared_ptr<sf::Sprite> heapThumbnail = std::make_shared<sf::Sprite>(thumbnailImg[HEAP]);
    heapThumbnail->setPosition({467,136});
    addElement(heapThumbnail);

    //======================================================//
    // AVL tree

    std::shared_ptr<GUI::RectangleButton> avlButton = std::make_shared<GUI::RectangleButton>(
        sf::Vector2f(346,248), sf::Vector2f(868,136),
        "",20,0,0,
        sf::Color::Transparent, sf::Color::Transparent, sf::Color::Transparent
    );
    avlButton->setReleaseCallback([](sf::RectangleShape &rect, sf::Text &text){
        Global::curAppState = Global::SceneState::AVL;
    });
    addElement(avlButton);

    std::shared_ptr<GUI::ThemeRectangle> avlTitleRect = std::make_shared<GUI::ThemeRectangle>(
        sf::Vector2f(346,68), sf::Vector2f(868,316)
    );
    addElement(avlTitleRect);

    std::shared_ptr<GUI::RectangleButton> avlTitle = std::make_shared<GUI::RectangleButton>(
        sf::Vector2f(346,68), sf::Vector2f(868,316),
        "AVL Tree", 30, 0, 0,
        sf::Color::Transparent, sf::Color::White
    );
    // avlTitle->disableColor = Global::colorSet[0][Global::MAIN];
    // avlTitle->lock();
    addElement(avlTitle);

    std::shared_ptr<sf::Sprite> avlThumbnail = std::make_shared<sf::Sprite>(thumbnailImg[AVL]);
    avlThumbnail->setPosition({868,136});
    addElement(avlThumbnail);

    //======================================================//
    // Trie

    std::shared_ptr<GUI::RectangleButton> trieButton = std::make_shared<GUI::RectangleButton>(
        sf::Vector2f(346,248), sf::Vector2f(266,416),
        "",20,0,0,
        sf::Color::Transparent, sf::Color::Transparent, sf::Color::Transparent
    );
    trieButton->setReleaseCallback([](sf::RectangleShape &rect, sf::Text &text){
        Global::curAppState = Global::SceneState::TRIE;
    });
    addElement(trieButton);

    std::shared_ptr<GUI::ThemeRectangle> trieTitleRect = std::make_shared<GUI::ThemeRectangle>(
        sf::Vector2f(346,68), sf::Vector2f(266,596)
    );
    addElement(trieTitleRect);

    std::shared_ptr<GUI::RectangleButton> trieTitle = std::make_shared<GUI::RectangleButton>(
        sf::Vector2f(346,68), sf::Vector2f(266,596),
        "Trie", 30, 0, 0,
        sf::Color::Transparent, sf::Color::White
    );
    // trieTitle->disableColor = Global::colorSet[0][Global::MAIN];
    // trieTitle->lock();
    addElement(trieTitle);

    std::shared_ptr<sf::Sprite> trieThumbnail = std::make_shared<sf::Sprite>(thumbnailImg[TRIE]);
    trieThumbnail->setPosition({266,416});
    addElement(trieThumbnail);

    //======================================================//
    // Graph

    std::shared_ptr<GUI::RectangleButton> graphButton = std::make_shared<GUI::RectangleButton>(
        sf::Vector2f(346,248), sf::Vector2f(667,416),
        "",20,0,0,
        sf::Color::Transparent, sf::Color::Transparent, sf::Color::Transparent
    );
    graphButton->setReleaseCallback([](sf::RectangleShape &rect, sf::Text &text){
        Global::curAppState = Global::SceneState::GRAPH;
    });
    addElement(graphButton);

    std::shared_ptr<GUI::ThemeRectangle> graphTitleRect = std::make_shared<GUI::ThemeRectangle>(
        sf::Vector2f(346,68), sf::Vector2f(667,596)
    );
    addElement(graphTitleRect);

    std::shared_ptr<GUI::RectangleButton> graphTitle = std::make_shared<GUI::RectangleButton>(
        sf::Vector2f(346,68), sf::Vector2f(667,596),
        "Graph", 30, 0, 0,
        sf::Color::Transparent, sf::Color::White
    );
    // graphTitle->disableColor = Global::colorSet[0][Global::MAIN];
    // graphTitle->lock();
    addElement(graphTitle);

    std::shared_ptr<sf::Sprite> graphThumbnail = std::make_shared<sf::Sprite>(thumbnailImg[SP]);
    graphThumbnail->setPosition({667,416});
    addElement(graphThumbnail);

    //======================================================//

    // Footer bar

    std::shared_ptr<GUI::ThemeRectangle> footerBg = std::make_shared<GUI::ThemeRectangle>(
        sf::Vector2f(1280,100), sf::Vector2f(0,700)
    );
    addElement(footerBg);

    std::shared_ptr<sf::RectangleShape> settingBg = std::make_shared<sf::RectangleShape>();
    settingBg->setPosition({389,722});
    settingBg->setSize({502,55});
    settingBg->setFillColor(Global::colorSet[0][Global::COLOR_TYPE::BACKGROUND]);
    settingBg->setOutlineThickness(2);
    settingBg->setOutlineColor(Global::colorSet[0][Global::COLOR_TYPE::NEUTRAL]);
    addElement(settingBg);

    std::shared_ptr<GUI::RectangleButton> settingName = std::make_shared<GUI::RectangleButton>(
        sf::Vector2f(187,55), sf::Vector2f(389,722),
        "Color setting", 22, 0, 0,
        sf::Color::Transparent,
        Global::colorSet[0][Global::COLOR_TYPE::NEUTRAL],
        sf::Color::Transparent
    );
    addElement(settingName);

    for (int i = 0; i < Global::numColorSet; i++) {
        std::shared_ptr<GUI::RectangleButton> colorSetting = std::make_shared<GUI::RectangleButton>(
            sf::Vector2f(30,30), sf::Vector2f(591 + 60 * i,735),
            "", 1, 0, 2,
            Global::colorSet[i][Global::COLOR_TYPE::MAIN],
            sf::Color::Transparent,
            Global::colorSet[i][Global::COLOR_TYPE::NEUTRAL]
        );
        colorSetting->setReleaseCallback([this,i](sf::RectangleShape &rect, sf::Text &text){
            Global::curColorSet = i;
        });
        addElement(colorSetting);
    }

    Scene::setup();
}

//======================================================//

// Data structure visual scene implementation

void VisualScene::setup()
{

    std::cerr << "Visual scene setup start\n";

    zoomView.setCenter(sf::Vector2f(1280,700) * 0.5f + sf::Vector2f(0,50));
    zoomView.setSize(sf::Vector2f(1280,700));

    //======================================================//
    // Header bar

    std::shared_ptr<GUI::ThemeRectangle> headerBarBg = std::make_shared<GUI::ThemeRectangle>();
    // headerBarBg->setFillColor(Global::colorSet[0][Global::COLOR_TYPE::MAIN]);
    headerBarBg->rect.setSize({1280,50});
    addElement(headerBarBg);

    this->inputField = std::make_shared<GUI::TextInputField>(
        sf::Vector2f(164,26), sf::Vector2f(29,12), sf::Vector2f(0,0),
        9, 22, 0, 2,
        Global::colorSet[0][Global::COLOR_TYPE::BACKGROUND], 
        Global::colorSet[0][Global::COLOR_TYPE::NEUTRAL], 
        Global::colorSet[0][Global::COLOR_TYPE::NEUTRAL]
    );
    this->inputField->setFilter([](char32_t chr) -> bool {
        return chr >= '0' && chr <= '9';
    });
    addElement(this->inputField);

    // std::cerr << "Input field added\n";

    this->insertBtn = std::make_shared<GUI::RectangleButton>(
        sf::Vector2f(110,26), sf::Vector2f(213,12),
        "Insert", 22, 0, 2,
        Global::colorSet[0][Global::COLOR_TYPE::BACKGROUND], 
        Global::colorSet[0][Global::COLOR_TYPE::NEUTRAL], 
        Global::colorSet[0][Global::COLOR_TYPE::NEUTRAL]
    );
    addElement(this->insertBtn);

    std::cerr << "Insert added\n";

    this->removeBtn = std::make_shared<GUI::RectangleButton>(
        sf::Vector2f(110,26), sf::Vector2f(341,12),
        "Delete", 22, 0, 2,
        Global::colorSet[0][Global::COLOR_TYPE::BACKGROUND], 
        Global::colorSet[0][Global::COLOR_TYPE::NEUTRAL], 
        Global::colorSet[0][Global::COLOR_TYPE::NEUTRAL]
    );
    addElement(this->removeBtn);

    // std::cerr << "Remove added\n";

    this->findBtn = std::make_shared<GUI::RectangleButton>(
        sf::Vector2f(110,26), sf::Vector2f(469,12),
        "Find", 22, 0, 2,
        Global::colorSet[0][Global::COLOR_TYPE::BACKGROUND], 
        Global::colorSet[0][Global::COLOR_TYPE::NEUTRAL], 
        Global::colorSet[0][Global::COLOR_TYPE::NEUTRAL]
    );
    addElement(this->findBtn);

    // std::cerr << "Find added\n";

    this->updateBtn = std::make_shared<GUI::RectangleButton>(
        sf::Vector2f(110,26), sf::Vector2f(597,12),
        "Update", 22, 0, 2,
        Global::colorSet[0][Global::COLOR_TYPE::BACKGROUND], 
        Global::colorSet[0][Global::COLOR_TYPE::NEUTRAL], 
        Global::colorSet[0][Global::COLOR_TYPE::NEUTRAL]
    );
    addElement(this->updateBtn);
    
    // std::cerr << "Update added\n";

    this->updField = std::make_shared<GUI::TextInputField>(
        sf::Vector2f(203,26), sf::Vector2f(725,12), sf::Vector2f(10,0),
        9, 22, 0, 2,
        Global::colorSet[0][Global::COLOR_TYPE::BACKGROUND], 
        Global::colorSet[0][Global::COLOR_TYPE::NEUTRAL], 
        Global::colorSet[0][Global::COLOR_TYPE::NEUTRAL]
    );
    this->updField->setFilter([](char32_t chr) -> bool {
        return chr >= '0' && chr <= '9';
    });
    addElement(this->updField);

    // std::cerr << "Update field added\n";

    std::shared_ptr<sf::Text> updFieldText = std::make_shared<sf::Text>(Global::textFont);
    updFieldText->setString("To:");
    updFieldText->setCharacterSize(22);
    updFieldText->setFillColor(Global::colorSet[0][Global::COLOR_TYPE::NEUTRAL]);
    updFieldText->setOrigin(updFieldText->getLocalBounds().position + updFieldText->getLocalBounds().size * 0.5f);
    updFieldText->setPosition(sf::Vector2f(732,12) + sf::Vector2f(29,26) * 0.5f);
    addElement(updFieldText);

    this->clearBtn = std::make_shared<GUI::RectangleButton>(
        sf::Vector2f(110,26), sf::Vector2f(1013,12),
        "Clear", 22, 0, 2,
        Global::colorSet[0][Global::COLOR_TYPE::BACKGROUND], 
        Global::colorSet[0][Global::COLOR_TYPE::NEUTRAL], 
        Global::colorSet[0][Global::COLOR_TYPE::NEUTRAL]
    );
    addElement(this->clearBtn);

    this->randomBtn = std::make_shared<GUI::RectangleButton>(
        sf::Vector2f(110,26), sf::Vector2f(1140,12),
        "Random", 22, 0, 2,
        Global::colorSet[0][Global::COLOR_TYPE::BACKGROUND], 
        Global::colorSet[0][Global::COLOR_TYPE::NEUTRAL], 
        Global::colorSet[0][Global::COLOR_TYPE::NEUTRAL]
    );
    addElement(this->randomBtn);

    std::cerr << "Header bar completed\n";

    //======================================================//

    std::shared_ptr<GUI::RectangleButton> viewResetBtn = std::make_shared<GUI::RectangleButton>(
        sf::Vector2f(110,26), sf::Vector2f(1140, 64),
        "Reset", 22, 0, 2,
        Global::colorSet[0][Global::COLOR_TYPE::BACKGROUND],
        Global::colorSet[0][Global::COLOR_TYPE::NEUTRAL],
        Global::colorSet[0][Global::COLOR_TYPE::NEUTRAL]
    );
    viewResetBtn->setReleaseCallback([this](sf::RectangleShape &rect, sf::Text &text){
        zoomView.resetView();
    });
    addElement(viewResetBtn);

    //======================================================//
    // Footer bar

    std::shared_ptr<GUI::ThemeRectangle> footerBarBg = std::make_shared<GUI::ThemeRectangle>();
    // footerBarBg->setFillColor(Global::colorSet[0][Global::COLOR_TYPE::MAIN]);
    footerBarBg->rect.setSize({1280,50});
    footerBarBg->rect.setPosition({0,750});
    addElement(footerBarBg);

    this->fullUndoBtn = std::make_shared<GUI::RectangleButton>(
        sf::Vector2f(40,26), sf::Vector2f(29,762),
        "<<", 22, 0, 2,
        Global::colorSet[0][Global::COLOR_TYPE::BACKGROUND], 
        Global::colorSet[0][Global::COLOR_TYPE::NEUTRAL], 
        Global::colorSet[0][Global::COLOR_TYPE::NEUTRAL]
    );
    addElement(this->fullUndoBtn);

    this->undoBtn = std::make_shared<GUI::RectangleButton>(
        sf::Vector2f(40,26), sf::Vector2f(87,762),
        "<", 22, 0, 2,
        Global::colorSet[0][Global::COLOR_TYPE::BACKGROUND], 
        Global::colorSet[0][Global::COLOR_TYPE::NEUTRAL], 
        Global::colorSet[0][Global::COLOR_TYPE::NEUTRAL]
    );
    addElement(this->undoBtn);

    std::shared_ptr<sf::RectangleShape> animationStepBg = std::make_shared<sf::RectangleShape>();
    animationStepBg->setFillColor(Global::colorSet[0][Global::COLOR_TYPE::BACKGROUND]);
    animationStepBg->setOutlineColor(Global::colorSet[0][Global::COLOR_TYPE::NEUTRAL]);
    animationStepBg->setOutlineThickness(2);
    animationStepBg->setSize({528,26});
    animationStepBg->setPosition({145,762});
    addElement(animationStepBg);

    this->animationSlider = std::make_shared<GUI::HSlider>(
        sf::Vector2f(508,18), sf::Vector2f(155,766),
        1, 0, 1, 2,
        Global::colorSet[0][Global::COLOR_TYPE::BACKGROUND], 
        sf::Color(0xA0A0A0FF), 
        Global::colorSet[0][Global::COLOR_TYPE::NEUTRAL]
    );
    addElement(this->animationSlider);

    this->redoBtn = std::make_shared<GUI::RectangleButton>(
        sf::Vector2f(40,26), sf::Vector2f(691,762),
        ">", 22, 0, 2,
        Global::colorSet[0][Global::COLOR_TYPE::BACKGROUND], 
        Global::colorSet[0][Global::COLOR_TYPE::NEUTRAL], 
        Global::colorSet[0][Global::COLOR_TYPE::NEUTRAL]
    );
    addElement(this->redoBtn);

    this->fullRedoBtn = std::make_shared<GUI::RectangleButton>(
        sf::Vector2f(40,26), sf::Vector2f(749,762),
        ">>", 22, 0, 2,
        Global::colorSet[0][Global::COLOR_TYPE::BACKGROUND], 
        Global::colorSet[0][Global::COLOR_TYPE::NEUTRAL], 
        Global::colorSet[0][Global::COLOR_TYPE::NEUTRAL]
    );
    addElement(this->fullRedoBtn);

    std::cerr << "Added animation slider\n";

    std::shared_ptr<GUI::ValueText<std::string>> animationSpdDisplay = std::make_shared<GUI::ValueText<std::string>>(
        sf::Vector2f(877,762) + sf::Vector2f(7,-3),
        22, 0, Global::colorSet[0][Global::COLOR_TYPE::NEUTRAL]
    );
    *animationSpdDisplay = "1x";

    std::shared_ptr<GUI::RectangleButton> animationSpdBtn = std::make_shared<GUI::RectangleButton>(
        sf::Vector2f(110,26), sf::Vector2f(807,762),
        "", 22, 0, 2,
        Global::colorSet[0][Global::COLOR_TYPE::BACKGROUND], 
        Global::colorSet[0][Global::COLOR_TYPE::NEUTRAL], 
        Global::colorSet[0][Global::COLOR_TYPE::NEUTRAL]
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
    animationSpdText->setFillColor(Global::colorSet[0][Global::COLOR_TYPE::NEUTRAL]);
    animationSpdText->setCharacterSize(22);    
    animationSpdText->setOrigin(animationSpdText->getLocalBounds().position + animationSpdText->getLocalBounds().size * 0.5f);
    animationSpdText->setPosition(sf::Vector2f(807,762) + sf::Vector2f(70, 26) * 0.5f);
    addElement(animationSpdText);

    std::cerr << "Added speed button\n";

    this->fileBtn = std::make_shared<GUI::RectangleButton>(
        sf::Vector2f(110,26), sf::Vector2f(974,762),
        "File", 22, 0, 2,
        Global::colorSet[0][Global::COLOR_TYPE::BACKGROUND], 
        Global::colorSet[0][Global::COLOR_TYPE::NEUTRAL], 
        Global::colorSet[0][Global::COLOR_TYPE::NEUTRAL]
    );
    addElement(this->fileBtn);

    std::shared_ptr<GUI::RectangleButton> menuBtn = std::make_shared<GUI::RectangleButton>(
        sf::Vector2f(110,26), sf::Vector2f(1140,762),
        "Menu", 22, 0, 2,
        Global::colorSet[0][Global::COLOR_TYPE::BACKGROUND], 
        Global::colorSet[0][Global::COLOR_TYPE::NEUTRAL], 
        Global::colorSet[0][Global::COLOR_TYPE::NEUTRAL]
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

// SLL scene implementation

void SLLScene::setup()
{
    VisualScene::setup();

    std::cerr << "AVL scene setup start\n";

    this->listVisual = std::make_shared<GUI::SLLVisualHandler>(
        sf::Vector2f{1280, 700}, sf::Vector2f(0, 50), 320, 30, 60
    );
    sllHandler.setVisualizer(listVisual);
    // addElement(listVisual);
    addZoomElement(listVisual);

    sllHandler.setAnimationSlider(this->animationSlider);
    // this->animationSlider->lock();

    sllHandler.addLockableElement(this->insertBtn);
    sllHandler.addLockableElement(this->removeBtn);
    sllHandler.addLockableElement(this->findBtn);
    sllHandler.addLockableElement(this->updateBtn);
    sllHandler.addLockableElement(this->clearBtn);
    sllHandler.addLockableElement(this->randomBtn);
    sllHandler.addLockableElement(this->inputField);
    sllHandler.addLockableElement(this->updField);
    sllHandler.addLockableElement(this->fileBtn);
    sllHandler.addLockableElement(this->fullUndoBtn);
    sllHandler.addLockableElement(this->undoBtn);
    sllHandler.addLockableElement(this->redoBtn);
    // sllHandler.addLockableElement(this->fullRedoBtn);

    
    this->insertBtn->setReleaseCallback([this](sf::RectangleShape &rect, sf::Text &text){
        std::string value = this->inputField->getValue();
        if (value == "") return;
        int num = stringToInt(value);
        this->sllHandler.insert(num);
    });

    this->removeBtn->setReleaseCallback([this](sf::RectangleShape &rect, sf::Text &text){
        std::string value = this->inputField->getValue();
        if (value == "") return;
        int num = stringToInt(value);
        this->sllHandler.remove(num);
    });

    this->findBtn->setReleaseCallback([this](sf::RectangleShape &rect, sf::Text &text){
        std::string value = this->inputField->getValue();
        if (value == "") return;
        int num = stringToInt(value);
        this->sllHandler.find(num);
    });

    this->updateBtn->setReleaseCallback([this](sf::RectangleShape &rect, sf::Text &text){
        std::string value = this->inputField->getValue();
        std::string newValue = this->updField->getValue();
        if (value == "" || newValue == "") return;
        this->sllHandler.update(stringToInt(value), stringToInt(newValue));
    });

    this->randomBtn->setReleaseCallback([this](sf::RectangleShape &rect, sf::Text &text){
        this->sllHandler.random();
        this->sllHandler.endAnimation();
    });

    this->clearBtn->setReleaseCallback([this](sf::RectangleShape &rect, sf::Text &text){
        this->sllHandler.clear();
    });

    this->fullUndoBtn->setReleaseCallback([this](sf::RectangleShape &rect, sf::Text &text){
        this->sllHandler.fullUndo();
    });

    this->fullRedoBtn->setReleaseCallback([this](sf::RectangleShape &rect, sf::Text &text){
        this->sllHandler.endAnimation();
    });

    this->undoBtn->setReleaseCallback([this](sf::RectangleShape &rect, sf::Text &text){
        this->sllHandler.undo();
    });

    this->redoBtn->setReleaseCallback([this](sf::RectangleShape &rect, sf::Text &text){
        this->sllHandler.redo();
    });

    this->fileBtn->setReleaseCallback([this](sf::RectangleShape &rect, sf::Text &text){
        this->sllHandler.file();
    });

    std::vector<std::string> codeFilename = {
        "./asset/psuedo-code/sll/insert.txt",
        "./asset/psuedo-code/sll/find.txt",
        "./asset/psuedo-code/sll/remove.txt",
        "./asset/psuedo-code/sll/update.txt"
    };

    std::vector<std::string> funcName = {
        "Insert", "Find", "Remove", "Update"
    };
    
    std::shared_ptr<GUI::CodeVisualHandler> codeVisual = std::make_shared<GUI::CodeVisualHandler>(
        sf::Vector2f(870,531), sf::Vector2f(410,20),
        14,
        funcName, codeFilename
    );
    addElement(codeVisual);

    sllHandler.setCodeVisualizer(codeVisual);

    Scene::setup();
}

void SLLScene::loopUpdate()
{
    sllHandler.loop();
}

//======================================================//

// Heap scene implementation

void HeapScene::setup()
{
    VisualScene::setup();

    std::cerr << "AVL scene setup start\n";

    this->treeVisual = std::make_shared<GUI::TreeVisualHandler>(
        sf::Vector2f{1280, 700}, sf::Vector2f{0, 50}, 60
    );
    heapHandler.setVisualizer(treeVisual);
    // addElement(treeVisual);
    addZoomElement(treeVisual);

    heapHandler.setAnimationSlider(this->animationSlider);
    // this->animationSlider->lock();

    heapHandler.addLockableElement(this->insertBtn);
    heapHandler.addLockableElement(this->removeBtn);
    heapHandler.addLockableElement(this->findBtn);
    heapHandler.addLockableElement(this->updateBtn);
    heapHandler.addLockableElement(this->clearBtn);
    heapHandler.addLockableElement(this->randomBtn);
    heapHandler.addLockableElement(this->inputField);
    heapHandler.addLockableElement(this->updField);
    heapHandler.addLockableElement(this->fileBtn);
    heapHandler.addLockableElement(this->fullUndoBtn);
    heapHandler.addLockableElement(this->undoBtn);
    heapHandler.addLockableElement(this->redoBtn);
    // heapHandler.addLockableElement(this->fullRedoBtn);

    
    this->insertBtn->setReleaseCallback([this](sf::RectangleShape &rect, sf::Text &text){
        std::string value = this->inputField->getValue();
        if (value == "") return;
        int num = stringToInt(value);
        this->heapHandler.insert(num);
    });

    this->removeBtn->setString("Id remove");
    this->removeBtn->setReleaseCallback([this](sf::RectangleShape &rect, sf::Text &text){
        std::string value = this->inputField->getValue();
        if (value == "") return;
        int num = stringToInt(value);
        this->heapHandler.removeById(num);
    });

    // Find button is now for pop function
    this->findBtn->setString("Pop");
    this->findBtn->setReleaseCallback([this](sf::RectangleShape &rect, sf::Text &text){
        // std::string value = this->inputField->getValue();
        // if (value == "") return;
        // int num = stringToInt(value);
        this->heapHandler.pop();
    });

    this->updateBtn->setString("Id update");
    this->updateBtn->setReleaseCallback([this](sf::RectangleShape &rect, sf::Text &text){
        std::string value = this->inputField->getValue();
        std::string newValue = this->updField->getValue();
        if (value == "" || newValue == "") return;
        this->heapHandler.updateById(stringToInt(value), stringToInt(newValue));
    });

    this->randomBtn->setReleaseCallback([this](sf::RectangleShape &rect, sf::Text &text){
        this->heapHandler.random();
        this->heapHandler.endAnimation();
    });

    this->clearBtn->setReleaseCallback([this](sf::RectangleShape &rect, sf::Text &text){
        this->heapHandler.clear();
    });

    this->fullUndoBtn->setReleaseCallback([this](sf::RectangleShape &rect, sf::Text &text){
        this->heapHandler.fullUndo();
    });

    this->fullRedoBtn->setReleaseCallback([this](sf::RectangleShape &rect, sf::Text &text){
        this->heapHandler.endAnimation();
    });

    this->undoBtn->setReleaseCallback([this](sf::RectangleShape &rect, sf::Text &text){
        this->heapHandler.undo();
    });

    this->redoBtn->setReleaseCallback([this](sf::RectangleShape &rect, sf::Text &text){
        this->heapHandler.redo();
    });

    this->fileBtn->setReleaseCallback([this](sf::RectangleShape &rect, sf::Text &text){
        this->heapHandler.file();
    });

    std::vector<std::string> codeFilename = {
        "./asset/psuedo-code/heap/insert.txt",
        "./asset/psuedo-code/heap/pop.txt",
        "./asset/psuedo-code/heap/removeById.txt",
        "./asset/psuedo-code/heap/updateById.txt",
        "./asset/psuedo-code/heap/downheap.txt",
        "./asset/psuedo-code/heap/upheap.txt"
    };

    std::vector<std::string> funcName = {
        "Insert", "Pop", "Id remove", "Id update", "Downheap", "Upheap"
    };
    
    std::shared_ptr<GUI::CodeVisualHandler> codeVisual = std::make_shared<GUI::CodeVisualHandler>(
        sf::Vector2f(941,531), sf::Vector2f(339,20),
        14,
        funcName, codeFilename
    );
    addElement(codeVisual);

    heapHandler.setCodeVisualizer(codeVisual);

    Scene::setup();
}

void HeapScene::loopUpdate()
{
    heapHandler.loop();
}

//======================================================//

// AVL scene implementation

void AVLScene::setup()
{
    VisualScene::setup();

    std::cerr << "AVL scene setup start\n";

    this->treeVisual = std::make_shared<GUI::TreeVisualHandler>(
        sf::Vector2f{1280, 700}, sf::Vector2f{0, 50}, 60
    );
    avlHandler.setVisualizer(treeVisual);
    // addElement(treeVisual);
    addZoomElement(treeVisual);

    avlHandler.setAnimationSlider(this->animationSlider);
    // this->animationSlider->lock();

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
        "./asset/psuedo-code/avl/remove.txt",
        "./asset/psuedo-code/avl/update.txt",
        "./asset/psuedo-code/avl/balance.txt"
    };

    std::vector<std::string> funcName = {
        "Insert", "Find", "Remove", "Update", "Balance"
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

//======================================================//

// AVL scene implementation

bool charFilter(char32_t c)
{
    return c >= 'a' && c <= 'z';
}

void TrieScene::setup()
{
    VisualScene::setup();

    std::cerr << "AVL scene setup start\n";

    this->treeVisual = std::make_shared<GUI::TrieVisualHandler>(
        sf::Vector2f{1280, 700}, sf::Vector2f{0, 50}, 70, 30
    );
    trieHandler.setVisualizer(treeVisual);
    // addElement(treeVisual);
    addZoomElement(treeVisual);

    trieHandler.setAnimationSlider(this->animationSlider);
    // this->animationSlider->lock();

    trieHandler.addLockableElement(this->insertBtn);
    trieHandler.addLockableElement(this->removeBtn);
    trieHandler.addLockableElement(this->findBtn);
    trieHandler.addLockableElement(this->updateBtn);
    trieHandler.addLockableElement(this->clearBtn);
    trieHandler.addLockableElement(this->randomBtn);
    trieHandler.addLockableElement(this->inputField);
    trieHandler.addLockableElement(this->updField);
    trieHandler.addLockableElement(this->fileBtn);
    trieHandler.addLockableElement(this->fullUndoBtn);
    trieHandler.addLockableElement(this->undoBtn);
    trieHandler.addLockableElement(this->redoBtn);
    // trieHandler.addLockableElement(this->fullRedoBtn);

    this->inputField->setFilter(charFilter);
    this->insertBtn->setReleaseCallback([this](sf::RectangleShape &rect, sf::Text &text){
        std::string value = this->inputField->getValue();
        if (value == "") return;
        this->trieHandler.insert(value);
    });

    this->removeBtn->setReleaseCallback([this](sf::RectangleShape &rect, sf::Text &text){
        std::string value = this->inputField->getValue();
        if (value == "") return;
        this->trieHandler.remove(value);
    });

    this->findBtn->setReleaseCallback([this](sf::RectangleShape &rect, sf::Text &text){
        std::string value = this->inputField->getValue();
        if (value == "") return;
        this->trieHandler.find(value);
    });

    this->updField->setFilter(charFilter);
    this->updateBtn->setReleaseCallback([this](sf::RectangleShape &rect, sf::Text &text){
        std::string value = this->inputField->getValue();
        std::string newValue = this->updField->getValue();
        if (value == "" || newValue == "") return;
        this->trieHandler.update(value, newValue);
    });

    this->randomBtn->setReleaseCallback([this](sf::RectangleShape &rect, sf::Text &text){
        this->trieHandler.random();
        this->trieHandler.endAnimation();
    });

    this->clearBtn->setReleaseCallback([this](sf::RectangleShape &rect, sf::Text &text){
        this->trieHandler.clear();
    });

    this->fullUndoBtn->setReleaseCallback([this](sf::RectangleShape &rect, sf::Text &text){
        this->trieHandler.fullUndo();
    });

    this->fullRedoBtn->setReleaseCallback([this](sf::RectangleShape &rect, sf::Text &text){
        this->trieHandler.endAnimation();
    });

    this->undoBtn->setReleaseCallback([this](sf::RectangleShape &rect, sf::Text &text){
        this->trieHandler.undo();
    });

    this->redoBtn->setReleaseCallback([this](sf::RectangleShape &rect, sf::Text &text){
        this->trieHandler.redo();
    });

    this->fileBtn->setReleaseCallback([this](sf::RectangleShape &rect, sf::Text &text){
        this->trieHandler.file();
    });

    std::vector<std::string> codeFilename = {
        "./asset/psuedo-code/trie/insert.txt",
        "./asset/psuedo-code/trie/find.txt",
        "./asset/psuedo-code/trie/remove.txt",
        "./asset/psuedo-code/trie/update.txt"
    };

    std::vector<std::string> funcName = {
        "Insert", "Find", "Remove", "Update"
    };
    
    std::shared_ptr<GUI::CodeVisualHandler> codeVisual = std::make_shared<GUI::CodeVisualHandler>(
        sf::Vector2f(870,510), sf::Vector2f(410,20),
        14,
        funcName, codeFilename
    );
    addElement(codeVisual);

    trieHandler.setCodeVisualizer(codeVisual);

    Scene::setup();
}

void TrieScene::loopUpdate()
{
    trieHandler.loop();
}

//======================================================//

void GraphScene::setup()
{
    zoomView.setCenter(sf::Vector2f(1280,700) * 0.5f + sf::Vector2f(0,50));
    zoomView.setSize(sf::Vector2f(1280,700));

    this->graphVisual = std::make_shared<GraphVisualHandler>(
        sf::Vector2f(1280,700), sf::Vector2f(0,50)
    );
    handler.setVisualizer(graphVisual);
    // addElement(graphVisual);
    addZoomElement(graphVisual);

    std::shared_ptr<GUI::ThemeRectangle> headerBarBg = std::make_shared<GUI::ThemeRectangle>();
    // headerBarBg->setFillColor(Global::colorSet[0][Global::COLOR_TYPE::MAIN]);
    headerBarBg->rect.setSize({1280,50});
    addElement(headerBarBg);

    std::shared_ptr<GUI::PopupGroup> inputGroup = std::make_shared<GUI::PopupGroup>();
    std::shared_ptr<GUI::ThemeRectangle> inputBg = std::make_shared<GUI::ThemeRectangle>();
    inputBg->rect.setSize({304,402});
    inputBg->rect.setPosition({29,68});
    // inputBg->setFillColor(Global::colorSet[0][Global::COLOR_TYPE::MAIN]);
    inputBg->rect.setOutlineColor(Global::colorSet[0][Global::COLOR_TYPE::NEUTRAL]);
    inputBg->rect.setOutlineThickness(2);
    inputGroup->addElement(inputBg);
    graphInputField = std::make_shared<GUI::MultilineTextField>(
        sf::Vector2f{285,341}, sf::Vector2f{38,77}
    );
    graphInputField->setFilter([this](char32_t chr) -> bool {
        return (chr >= '0' && chr <= '9') || chr == '\n' || chr == ' ';
    });
    inputGroup->addElement(graphInputField);
    std::shared_ptr<GUI::RectangleButton> inputSubmit = std::make_shared<GUI::RectangleButton>(
        sf::Vector2f(110,26), sf::Vector2f(213,432),
        "Submit", 22, 0, 2,
        Global::colorSet[0][Global::COLOR_TYPE::BACKGROUND],
        Global::colorSet[0][Global::COLOR_TYPE::NEUTRAL],
        Global::colorSet[0][Global::COLOR_TYPE::NEUTRAL]
    );
    inputSubmit->setReleaseCallback(
        [this,inputGroup](sf::RectangleShape &rect, sf::Text &text){
            std::string value = this->graphInputField->getValue();
            this->handler.buildGraph(value);
            inputGroup->close();
        }
    );
    inputGroup->addElement(inputSubmit);
    addElement(inputGroup);

    std::shared_ptr<GUI::RectangleButton> updateBtn = std::make_shared<GUI::RectangleButton>(
        sf::Vector2f(110,26), sf::Vector2f(29,12),
        "Update", 22, 0, 2,
        Global::colorSet[0][Global::COLOR_TYPE::BACKGROUND],
        Global::colorSet[0][Global::COLOR_TYPE::NEUTRAL],
        Global::colorSet[0][Global::COLOR_TYPE::NEUTRAL]
    );
    addElement(updateBtn);
    handler.addLockableElement(updateBtn);
    updateBtn->setReleaseCallback([this,inputGroup](sf::RectangleShape &rect, sf::Text &text){
        inputGroup->open();
    });

    std::shared_ptr<GUI::RectangleButton> primBtn = std::make_shared<GUI::RectangleButton>(
        sf::Vector2f(110,26), sf::Vector2f(156,12),
        "Prim", 22, 0, 2,
        Global::colorSet[0][Global::COLOR_TYPE::BACKGROUND],
        Global::colorSet[0][Global::COLOR_TYPE::NEUTRAL],
        Global::colorSet[0][Global::COLOR_TYPE::NEUTRAL]
    );
    addElement(primBtn);
    primBtn->setReleaseCallback([this](sf::RectangleShape &rect, sf::Text &text){
        handler.prim();
    });
    handler.addLockableElement(primBtn);

    std::shared_ptr<GUI::RectangleButton> dijkstraBtn = std::make_shared<GUI::RectangleButton>(
        sf::Vector2f(110,26), sf::Vector2f(283,12),
        "Dijkstra", 22, 0, 2,
        Global::colorSet[0][Global::COLOR_TYPE::BACKGROUND],
        Global::colorSet[0][Global::COLOR_TYPE::NEUTRAL],
        Global::colorSet[0][Global::COLOR_TYPE::NEUTRAL]
    );
    addElement(dijkstraBtn);
    handler.addLockableElement(dijkstraBtn);
    
    std::shared_ptr<GUI::TextInputField> dijkstraFromField = std::make_shared<GUI::TextInputField>(
        sf::Vector2f(120,26), sf::Vector2f(410,12),
        sf::Vector2f(30,1), 4,
        22, 0, 2,
        Global::colorSet[0][Global::COLOR_TYPE::BACKGROUND],
        Global::colorSet[0][Global::COLOR_TYPE::NEUTRAL],
        Global::colorSet[0][Global::COLOR_TYPE::NEUTRAL]
    );
    dijkstraFromField->setFilter([this](char32_t chr) -> bool {
        return chr >= '0' && chr <= '9';
    });
    addElement(dijkstraFromField);
    handler.addLockableElement(dijkstraFromField);

    std::shared_ptr<GUI::RectangleButton> dijkstraFromFieldTxt = std::make_shared<GUI::RectangleButton>(
        sf::Vector2f(60,26), sf::Vector2f(410,12),
        "From:", 22, 0, 0,
        sf::Color::Transparent,
        Global::colorSet[0][Global::COLOR_TYPE::NEUTRAL],
        sf::Color::Transparent
    );
    dijkstraFromFieldTxt->disableColor = sf::Color::Transparent;
    dijkstraFromFieldTxt->lock();
    addElement(dijkstraFromFieldTxt);

    std::shared_ptr<GUI::TextInputField> dijkstraToField = std::make_shared<GUI::TextInputField>(
        sf::Vector2f(120,26), sf::Vector2f(547,12),
        sf::Vector2f(30,1), 4,
        22, 0, 2,
        Global::colorSet[0][Global::COLOR_TYPE::BACKGROUND],
        Global::colorSet[0][Global::COLOR_TYPE::NEUTRAL],
        Global::colorSet[0][Global::COLOR_TYPE::NEUTRAL]
    );
    dijkstraToField->setFilter([this](char32_t chr) -> bool {
        return chr >= '0' && chr <= '9';
    });
    addElement(dijkstraToField);
    handler.addLockableElement(dijkstraToField);

    std::shared_ptr<GUI::RectangleButton> dijkstraToFieldTxt = std::make_shared<GUI::RectangleButton>(
        sf::Vector2f(60,26), sf::Vector2f(547,12),
        "To:", 22, 0, 0,
        sf::Color::Transparent,
        Global::colorSet[0][Global::COLOR_TYPE::NEUTRAL],
        sf::Color::Transparent
    );
    dijkstraToFieldTxt->disableColor = sf::Color::Transparent;
    dijkstraToFieldTxt->lock();
    addElement(dijkstraToFieldTxt);

    dijkstraBtn->setReleaseCallback([this,dijkstraFromField,dijkstraToField](sf::RectangleShape &rect, sf::Text &text){
        std::string fromTxt = dijkstraFromField->getValue(), toTxt = dijkstraToField->getValue();
        if (fromTxt == "" || toTxt == "") return;
        handler.dijkstra(stringToInt(fromTxt), stringToInt(toTxt));
    });


    std::shared_ptr<GUI::RectangleButton> clearBtn = std::make_shared<GUI::RectangleButton>(
        sf::Vector2f(110,26), sf::Vector2f(1013,12),
        "Clear", 22, 0, 2,
        Global::colorSet[0][Global::COLOR_TYPE::BACKGROUND],
        Global::colorSet[0][Global::COLOR_TYPE::NEUTRAL],
        Global::colorSet[0][Global::COLOR_TYPE::NEUTRAL]
    );
    clearBtn->setReleaseCallback([this](sf::RectangleShape &rect, sf::Text &text){
        handler.clear();
    });
    addElement(clearBtn);
    handler.addLockableElement(clearBtn);

    std::shared_ptr<GUI::RectangleButton> randomBtn = std::make_shared<GUI::RectangleButton>(
        sf::Vector2f(110,26), sf::Vector2f(1140,12),
        "Random", 22, 0, 2,
        Global::colorSet[0][Global::COLOR_TYPE::BACKGROUND],
        Global::colorSet[0][Global::COLOR_TYPE::NEUTRAL],
        Global::colorSet[0][Global::COLOR_TYPE::NEUTRAL]
    );
    randomBtn->setReleaseCallback([this](sf::RectangleShape &rect, sf::Text &text){
        handler.random();
    });
    addElement(randomBtn);
    handler.addLockableElement(randomBtn);

    //======================================================//

    std::shared_ptr<GUI::RectangleButton> viewResetBtn = std::make_shared<GUI::RectangleButton>(
        sf::Vector2f(110,26), sf::Vector2f(1140, 64),
        "Reset", 22, 0, 2,
        Global::colorSet[0][Global::COLOR_TYPE::BACKGROUND],
        Global::colorSet[0][Global::COLOR_TYPE::NEUTRAL],
        Global::colorSet[0][Global::COLOR_TYPE::NEUTRAL]
    );
    viewResetBtn->setReleaseCallback([this](sf::RectangleShape &rect, sf::Text &text){
        zoomView.resetView();
    });
    addElement(viewResetBtn);

    //======================================================//
    // Footer bar

    std::shared_ptr<GUI::ThemeRectangle> footerBarBg = std::make_shared<GUI::ThemeRectangle>();
    // footerBarBg->setFillColor(Global::colorSet[0][Global::COLOR_TYPE::MAIN]);
    footerBarBg->rect.setSize({1280,50});
    footerBarBg->rect.setPosition({0,750});
    addElement(footerBarBg);

    std::shared_ptr<GUI::RectangleButton> fullUndoBtn = std::make_shared<GUI::RectangleButton>(
        sf::Vector2f(40,26), sf::Vector2f(29,762),
        "<<", 22, 0, 2,
        Global::colorSet[0][Global::COLOR_TYPE::BACKGROUND], 
        Global::colorSet[0][Global::COLOR_TYPE::NEUTRAL], 
        Global::colorSet[0][Global::COLOR_TYPE::NEUTRAL]
    );
    addElement(fullUndoBtn);
    handler.addLockableElement(fullUndoBtn);

    std::shared_ptr<GUI::RectangleButton> undoBtn = std::make_shared<GUI::RectangleButton>(
        sf::Vector2f(40,26), sf::Vector2f(87,762),
        "<", 22, 0, 2,
        Global::colorSet[0][Global::COLOR_TYPE::BACKGROUND], 
        Global::colorSet[0][Global::COLOR_TYPE::NEUTRAL], 
        Global::colorSet[0][Global::COLOR_TYPE::NEUTRAL]
    );
    addElement(undoBtn);
    handler.addLockableElement(undoBtn);

    std::shared_ptr<sf::RectangleShape> animationStepBg = std::make_shared<sf::RectangleShape>();
    animationStepBg->setFillColor(Global::colorSet[0][Global::COLOR_TYPE::BACKGROUND]);
    animationStepBg->setOutlineColor(Global::colorSet[0][Global::COLOR_TYPE::NEUTRAL]);
    animationStepBg->setOutlineThickness(2);
    animationStepBg->setSize({528,26});
    animationStepBg->setPosition({145,762});
    addElement(animationStepBg);

    std::shared_ptr<GUI::HSlider> animationSlider = std::make_shared<GUI::HSlider>(
        sf::Vector2f(508,18), sf::Vector2f(155,766),
        1, 0, 1, 2,
        Global::colorSet[0][Global::COLOR_TYPE::BACKGROUND], 
        sf::Color(0xA0A0A0FF), 
        Global::colorSet[0][Global::COLOR_TYPE::NEUTRAL]
    );
    addElement(animationSlider);
    handler.setAnimationSlider(animationSlider);

    std::shared_ptr<GUI::RectangleButton> redoBtn = std::make_shared<GUI::RectangleButton>(
        sf::Vector2f(40,26), sf::Vector2f(691,762),
        ">", 22, 0, 2,
        Global::colorSet[0][Global::COLOR_TYPE::BACKGROUND], 
        Global::colorSet[0][Global::COLOR_TYPE::NEUTRAL], 
        Global::colorSet[0][Global::COLOR_TYPE::NEUTRAL]
    );
    addElement(redoBtn);
    handler.addLockableElement(redoBtn);

    std::shared_ptr<GUI::RectangleButton> fullRedoBtn = std::make_shared<GUI::RectangleButton>(
        sf::Vector2f(40,26), sf::Vector2f(749,762),
        ">>", 22, 0, 2,
        Global::colorSet[0][Global::COLOR_TYPE::BACKGROUND], 
        Global::colorSet[0][Global::COLOR_TYPE::NEUTRAL], 
        Global::colorSet[0][Global::COLOR_TYPE::NEUTRAL]
    );
    fullRedoBtn->setReleaseCallback([this](sf::RectangleShape &rect, sf::Text &text){
        this->handler.endAnimation();
    });
    addElement(fullRedoBtn);

    std::cerr << "Added animation slider\n";

    std::shared_ptr<GUI::ValueText<std::string>> animationSpdDisplay = std::make_shared<GUI::ValueText<std::string>>(
        sf::Vector2f(877,762) + sf::Vector2f(8,-3),
        22, 0, Global::colorSet[0][Global::COLOR_TYPE::NEUTRAL]
    );
    *animationSpdDisplay = "1x";

    std::shared_ptr<GUI::RectangleButton> animationSpdBtn = std::make_shared<GUI::RectangleButton>(
        sf::Vector2f(110,26), sf::Vector2f(807,762),
        "", 22, 0, 2,
        Global::colorSet[0][Global::COLOR_TYPE::BACKGROUND], 
        Global::colorSet[0][Global::COLOR_TYPE::NEUTRAL], 
        Global::colorSet[0][Global::COLOR_TYPE::NEUTRAL]
    );
    animationSpdBtn->setReleaseCallback([animationSpdDisplay](sf::RectangleShape &rect, sf::Text &text) {
        Global::animationSpeed += 1;
        if (Global::animationSpeed > 5) Global::animationSpeed = 1;
        *animationSpdDisplay = std::to_string((int)Global::animationSpeed) + "x";
    });

    addElement(animationSpdBtn);
    addElement(animationSpdDisplay);

    std::shared_ptr<GUI::RectangleButton> animationSpdText = std::make_shared<GUI::RectangleButton>(
        sf::Vector2f(70,26), sf::Vector2f(807,762),
        "Speed", 22, 0, 0,
        sf::Color::Transparent, 
        Global::colorSet[0][Global::COLOR_TYPE::NEUTRAL],
        sf::Color::Transparent
    );
    addElement(animationSpdText);

    std::cerr << "Added speed button\n";

    std::shared_ptr<GUI::RectangleButton> fileBtn = std::make_shared<GUI::RectangleButton>(
        sf::Vector2f(110,26), sf::Vector2f(974,762),
        "File", 22, 0, 2,
        Global::colorSet[0][Global::COLOR_TYPE::BACKGROUND], 
        Global::colorSet[0][Global::COLOR_TYPE::NEUTRAL], 
        Global::colorSet[0][Global::COLOR_TYPE::NEUTRAL]
    );
    fileBtn->setReleaseCallback([this](sf::RectangleShape &rect, sf::Text &text){
        handler.file();
    });
    addElement(fileBtn);
    handler.addLockableElement(fileBtn);

    std::shared_ptr<GUI::RectangleButton> menuBtn = std::make_shared<GUI::RectangleButton>(
        sf::Vector2f(110,26), sf::Vector2f(1140,762),
        "Menu", 22, 0, 2,
        Global::colorSet[0][Global::COLOR_TYPE::BACKGROUND], 
        Global::colorSet[0][Global::COLOR_TYPE::NEUTRAL], 
        Global::colorSet[0][Global::COLOR_TYPE::NEUTRAL]
    );
    menuBtn->setReleaseCallback([this](sf::RectangleShape &rect, sf::Text &text){
        this->clear();
        Global::curAppState = Global::SceneState::MENU;
    });
    addElement(menuBtn);

    // Global::GRAPH_FUNC::

    std::vector<std::string> codeFilename = {
        "./asset/psuedo-code/graph/dijkstra.txt",
        "./asset/psuedo-code/graph/prim.txt",
    };

    std::vector<std::string> funcName = {
        "Dijkstra", "Prim"
    };
    
    std::shared_ptr<GUI::CodeVisualHandler> codeVisual = std::make_shared<GUI::CodeVisualHandler>(
        sf::Vector2f(860,470), sf::Vector2f(420,18),
        12,
        funcName, codeFilename
    );
    addElement(codeVisual);
    handler.setCodeVisualizer(codeVisual);

    Scene::setup();

    std::cerr << "Graph scene completed\n";

}

void GraphScene::loopUpdate()
{
    handler.loop();
    graphInputField->update();
}

}