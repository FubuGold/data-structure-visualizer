#ifndef DATA_STRUCTURE_HANDLER
#define DATA_STRUCTURE_HANDLER

#include "data-structure.h"
#include "../UI/GUI-visual-handler.h"
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
private:

    std::vector<Global::TreeStructure> snapshot;
    DataStructure::AVLTree tree;
    std::shared_ptr<GUI::TreeVisualHandler> visualizer;

    int curSnapshot = 0;

    const float DELAY_TIME = 0.3f; // seconds
    bool delaying = 0;
    sf::Clock delayClock;

public:

    AVLTreeHandler();
    
    void setVisualizer(std::shared_ptr<GUI::TreeVisualHandler> visualizer_p);

    void insert(int x);
    void find(int x);
    void remove(int x);
    void update(int x,int newVal);
    void clear();

    void loop();

    void endAnimation();
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