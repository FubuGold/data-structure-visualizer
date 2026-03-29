#include "../../include/data-structure/data-structure-handler.h"


namespace Handler
{

//======================================================//

// AVL Tree handler implementation

AVLTreeHandler::AVLTreeHandler()
{
    tree.linkSnapshot(&snapshot);
}

void AVLTreeHandler::setVisualizer(std::shared_ptr<GUI::TreeVisualHandler> visualizer_p)
{
    this->visualizer = visualizer_p;
}

void AVLTreeHandler::insert(int x)
{
    std::cerr << "Insert called\n";
    snapshot.clear();
    tree.insert(x);
    curSnapshot = 0;
    visualizer->setTreeStructure(snapshot[0]);
    delaying = false;
}

void AVLTreeHandler::find(int x)
{
    std::cerr << "Find called\n";
    snapshot.clear();
    tree.find(x);
    curSnapshot = 0;
    visualizer->setTreeStructure(snapshot[0]);
    delaying = false;
}

void AVLTreeHandler::remove(int x)
{
    std::cerr << "Remove called\n";
    snapshot.clear();
    tree.remove(x);
    curSnapshot = 0;
    visualizer->setTreeStructure(snapshot[0]);
    delaying = false;
}

void AVLTreeHandler::update(int x,int newVal)
{
    std::cerr << "Update called\n";
    snapshot.clear();
    tree.update(x,newVal);
    curSnapshot = 0;
    visualizer->setTreeStructure(snapshot[0]);
    delaying = false;
}

void AVLTreeHandler::loop()
{
    visualizer->updateAnimation();
    if (visualizer->isAnimationEnd()) {
        if (delayClock.getElapsedTime().asSeconds() <= DELAY_TIME) return;
        curSnapshot++;
        if (curSnapshot >= snapshot.size()) return;
        visualizer->setTreeStructure(snapshot[curSnapshot]);
        delayClock.restart();
    }
    else delayClock.restart();
}

void AVLTreeHandler::endAnimation()
{
    visualizer->setTreeStructure(snapshot.back());
    curSnapshot = snapshot.size();
}

void AVLTreeHandler::clear()
{
    tree.clear();
    visualizer->clear();
}

//======================================================//

// Heap handler implementation

HeapHandler::HeapHandler()
{
    tree.linkSnapshot(&snapshot);
}

void HeapHandler::setVisualizer(std::shared_ptr<GUI::TreeVisualHandler> visualizer_p)
{
    this->visualizer = visualizer_p;
}

}