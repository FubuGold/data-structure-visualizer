#ifndef DATA_STRUCTURE_HANDLER
#define DATA_STRUCTURE_HANDLER

#include "data-structure.h"
#include "../UI/GUI-visual-handler.h"
#include "../UI/GUI-interactable-element.h"
#include "../global/global.h"

#include <SFML/Graphics.hpp>

//======================================================//

namespace Handler
{

/**
 * @brief A middleman class to handle input, internal update and GUI update
 * 
 */
class AVLTreeHandler
{
protected:

    static constexpr int VALUE_MAX = 99, VALUE_MIN = 0;

    std::vector<Global::TreeStructure> snapshot;
    DataStructure::AVLTree tree;
    std::shared_ptr<GUI::TreeVisualHandler> visualizer;
    std::shared_ptr<GUI::HSlider> animationSlider;
    std::shared_ptr<GUI::CodeVisualHandler> codeVisualizer;

    // This is for locking the element
    std::vector<std::shared_ptr<GUI::IInteractableElement>> lockableElement;

    int curSnapshot = 0;

    const float DELAY_TIME = 0.3f; // seconds
    bool delaying = 0;
    sf::Clock delayClock;

    bool animationLock = 0;
    bool buttonLock = 0;

    void preprocessing();
    void postprocessing();

    void lockElement();
    void unlockElement();

    void setSnapshot(int id);

public:

    AVLTreeHandler();
    
    void setVisualizer(std::shared_ptr<GUI::TreeVisualHandler> visualizer_p);
    void setAnimationSlider(std::shared_ptr<GUI::HSlider> animationSlider_p);
    void setCodeVisualizer(std::shared_ptr<GUI::CodeVisualHandler> codeVisualizer_p);

    void addLockableElement(std::shared_ptr<GUI::IInteractableElement> element);

    void loop();

    void endAnimation();

    void fullUndo();
    void undo();
    void redo();

    void insert(int x);
    void find(int x);
    void remove(int x);
    void update(int x,int newVal);
    void random();
    void clear();
    void file();

};

//======================================================//

/**
 * @brief A middleman class to handle input, internal update and GUI update
 * 
 */
class HeapHandler
{
private:

    std::vector<Global::TreeStructure> snapshot;
    DataStructure::AVLTree tree;
    std::shared_ptr<GUI::TreeVisualHandler> visualizer;

    int curSnapshot = 0;

    const float DELAY_TIME = 0.3f; // seconds
    bool delaying = 0;
    sf::Clock delayClock;

public:

    HeapHandler();
    
    void setVisualizer(std::shared_ptr<GUI::TreeVisualHandler> visualizer_p);

    void insert(int x);
    void find(int x) {};
    void remove(int x) {};
    void update(int x,int newVal) {};
    void clear();

    void loop();

    void endAnimation();
};

}

#endif // DATA_STRUCTURE_HANDLER