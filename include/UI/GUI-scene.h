#ifndef UI_GUI_SCENE_H
#define UI_GUI_SCENE_H

#include "GUI-interactable-element.h"
#include "GUI-element.h"
#include "GUI-visual-handler.h"
#include "../data-structure/data-structure-handler.h"
#include <SFML/Graphics.hpp>
#include <vector>

namespace GUI
{

/**
 * @brief Base class for scene
 * 
 */
class Scene
{
protected:
    std::vector<std::shared_ptr<sf::Drawable>> elements;
    std::vector<std::shared_ptr<IInteractableElement>> interactableElements;
    std::vector<std::shared_ptr<sf::Drawable>> zoomElements;

    template<typename T>
    requires std::derived_from<T,sf::Drawable>
    void addElement(std::shared_ptr<T> element_ptr);

    void addZoomElement(std::shared_ptr<sf::Drawable> element_ptr);

    sf::RenderWindow *target_ptr;
    GUI::ZoomView zoomView; // For visual scenes
    
public:
    
    void handleEvent(const std::optional<sf::Event>& e);
    
    virtual void setup();
    virtual void draw(sf::RenderWindow& target, sf::RenderStates state = sf::RenderStates::Default);
    void drawZoom();

    virtual void loopUpdate() {}
    virtual void clear() {}

    void setWindow(sf::RenderWindow *target_ptr);
};

//======================================================//

class DebugScene : public Scene
{
protected:

    std::shared_ptr<GUI::TreeVisualHandler> avl;
    Handler::AVLTreeHandler avlHandler;

public:
    void setup() override;

    void loopUpdate() override;
};

//======================================================//

class MenuScene : public Scene
{
protected:

    enum DSType 
    {
        SLL,
        HEAP,
        AVL,
        TRIE,
        SP,
        MST
    };

    sf::Texture thumbnailImg[6];
    static constexpr std::string filename[6] = {
        "SLL.png","Heap.png","AVL.png","Trie.png","SP.png","MST.png"
    };

public:

    void setup() override;

};

//======================================================//

/**
 * @brief Base scene for data structure visual
 * 
 */
class VisualScene : public Scene
{
protected:

    // Header bar
    std::shared_ptr<GUI::RectangleButton> insertBtn, removeBtn, findBtn, updateBtn, clearBtn, randomBtn;
    std::shared_ptr<GUI::TextInputField> inputField, updField;
    // Footer bar
    std::shared_ptr<GUI::RectangleButton> fullUndoBtn, undoBtn, redoBtn, fullRedoBtn, speedBtn, fileBtn;
    std::shared_ptr<GUI::HSlider> animationSlider;

public:

    void setup() override;

};

class SLLScene : public VisualScene
{
protected:

    std::shared_ptr<GUI::SLLVisualHandler> listVisual;
    Handler::SLLHandler sllHandler;

public:

    void setup() override;
    void loopUpdate() override;
};

class HeapScene : public VisualScene
{
protected:

    std::shared_ptr<GUI::TreeVisualHandler> treeVisual;
    Handler::HeapHandler heapHandler;

public:

    void setup() override;
    void loopUpdate() override;

};

class AVLScene : public VisualScene
{
protected:

    std::shared_ptr<GUI::TreeVisualHandler> treeVisual;
    Handler::AVLTreeHandler avlHandler;

public:

    void setup() override;
    void loopUpdate() override;
};

class TrieScene : public VisualScene
{
protected:

    std::shared_ptr<GUI::TrieVisualHandler> treeVisual;
    Handler::TrieHandler trieHandler;

public:

    void setup() override;
    void loopUpdate() override;
};

//======================================================//

class GraphScene : public Scene
{
protected:

    Handler::GraphHandler handler;
    std::shared_ptr<GraphVisualHandler> graphVisual;
    std::shared_ptr<GUI::MultilineTextField> graphInputField;

public:

    void setup() override;
    void loopUpdate() override;

};

}

#endif // UI_GUI_SCENE_H