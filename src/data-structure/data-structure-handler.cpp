#include "../../include/data-structure/data-structure-handler.h"
#include "../../include/data-structure/file-validator.h"


#include <chrono> // For seeding seed
#include <random> // For random
#include <fstream> // For file reading

std::mt19937 rndGen(std::chrono::high_resolution_clock::now().time_since_epoch().count());

int rng(int l,int r)
{
    return std::uniform_int_distribution<int>(l,r)(rndGen);
}

namespace Handler
{

//======================================================//

// AVL Tree handler implementation

AVLTreeHandler::AVLTreeHandler()
{
    tree.linkSnapshot(&snapshot);
}

void AVLTreeHandler::preprocessing()
{
    AVLTreeHandler::endAnimation();
    snapshot.clear();
    animationLock = 1;
    std::cerr << "Done preprocessing\n";
}

void AVLTreeHandler::postprocessing()
{
    curSnapshot = 0;
    delaying = false;
    lockElement();
    std::cerr << "Post processing\n";
    if (snapshot.size()) {
        setSnapshot(0);
    }

    animationSlider->setNewRange(0, std::max(0,(int)snapshot.size()-1), std::max(0,(int)snapshot.size()-1));
    animationSlider->setValue(0);
}

void AVLTreeHandler::lockElement()
{
    for (int i = 0; i < lockableElement.size(); i++) {
        lockableElement[i]->lock();
    }
}

void AVLTreeHandler::unlockElement()
{
    for (int i = 0; i < lockableElement.size(); i++) {
        lockableElement[i]->unlock();
    }
}

void AVLTreeHandler::setSnapshot(int id)
{
    std::cerr << id << ' ' << snapshot[id].codeFunction << ' ' << snapshot[id].codeLine << '\n';
    visualizer->setTreeStructure(snapshot[id]);
    animationSlider->setValue(id);
    codeVisualizer->setFunc(snapshot[id].codeFunction);
    codeVisualizer->setLine(snapshot[id].codeLine);
}

void AVLTreeHandler::setVisualizer(std::shared_ptr<GUI::TreeVisualHandler> visualizer_p)
{
    this->visualizer = visualizer_p;
}
void AVLTreeHandler::setAnimationSlider(std::shared_ptr<GUI::HSlider> animationSlider_p)
{
    this->animationSlider = animationSlider_p;
}
void AVLTreeHandler::setCodeVisualizer(std::shared_ptr<GUI::CodeVisualHandler> codeVisualizer_p)
{
    this->codeVisualizer = codeVisualizer_p;
}

void AVLTreeHandler::addLockableElement(std::shared_ptr<GUI::IInteractableElement> element)
{
    this->lockableElement.push_back(element);
}

void AVLTreeHandler::insert(int x)
{
    std::cerr << "Insert called\n";
    preprocessing();
    tree.insert(x);
    postprocessing();
}

void AVLTreeHandler::find(int x)
{
    std::cerr << "Find called\n";
    preprocessing();
    tree.find(x);
    postprocessing();
}

void AVLTreeHandler::remove(int x)
{
    std::cerr << "Remove called\n";
    preprocessing();
    tree.remove(x);
    postprocessing();
}

void AVLTreeHandler::update(int x,int newVal)
{
    std::cerr << "Update called\n";
    preprocessing();
    tree.update(x,newVal);
    postprocessing();
    // std::cerr << "Update completed\n";
}

void AVLTreeHandler::loop()
{
    // std::cerr << curSnapshot << ' ' << snapshot.size() << '\n';
    visualizer->updateAnimation();
    if (!animationLock) return;
    if (visualizer->isAnimationEnd()) {
        if (delayClock.getElapsedTime().asSeconds() <= DELAY_TIME) return;
        if (curSnapshot >= (int)snapshot.size() - 1) {
            if (animationLock) {
                animationLock = 0;
                unlockElement();
            }
            return;
        }
        curSnapshot++;
        setSnapshot(curSnapshot);
        delayClock.restart();
    }
    else delayClock.restart();
}

void AVLTreeHandler::random()
{
    tree.clear();
    visualizer->clear();
    snapshot.clear();

    int n = rng(15,20);
    for (int i=0;i<n;i++) {
        int val = rng(VALUE_MIN,VALUE_MAX);
        tree.insert(val);
    }
    
    postprocessing();
    endAnimation();
}

void AVLTreeHandler::clear()
{
    tree.clear();
    visualizer->clear();
    snapshot.clear();
    curSnapshot = 0;
}

void AVLTreeHandler::file()
{
    std::string filename = Global::openFile();
    if (filename == "") return;
    if (!Validator::avlValidator(filename)) return;
    
    preprocessing();

    std::ifstream inp(filename);
    int n; inp >> n;
    for (int i = 0; i < n; i++) {
        int x; inp >> x;
        tree.insert(x);
    }

    postprocessing();
    // endAnimation();
}

void AVLTreeHandler::endAnimation()
{
    // std::cerr << "End animation called\n";
    if (snapshot.size() != 0) {
        // visualizer->setTreeStructure(snapshot.back());
        setSnapshot(snapshot.size()-1);
        visualizer->endAnimation();
    }
    else {
        codeVisualizer->setFunc(-1);
    }
    curSnapshot = std::max(0,(int)snapshot.size() - 1);
    animationLock = 0;
    // std::cerr << "After if " << curSnapshot << '\n';
    // std::cerr << animationSlider << '\n';
    animationSlider->setValue(curSnapshot);
    unlockElement();
}

void AVLTreeHandler::fullUndo()
{
    if (snapshot.size() != 0) {
        // visualizer->setTreeStructure(snapshot[0]);
        setSnapshot(0);
        visualizer->endAnimation();
    }
    else {
        codeVisualizer->setFunc(-1);
    }
    curSnapshot = 0;
    animationLock = 0;
    // animationSlider->setValue(curSnapshot); 
}

void AVLTreeHandler::undo()
{
    if (curSnapshot > 0) {
        // std::cerr << curSnapshot << ' ' << snapshot.size() << '\n';
        visualizer->endAnimation();
        curSnapshot--;
        // visualizer->setTreeStructure(snapshot[curSnapshot]);
        // animationSlider->setValue(curSnapshot);
        setSnapshot(curSnapshot);
    }
}

void AVLTreeHandler::redo()
{
    if (curSnapshot < (int)snapshot.size()-1) {
        // std::cerr << curSnapshot << ' ' << snapshot.size() << '\n';
        visualizer->endAnimation();
        curSnapshot++;
        // visualizer->setTreeStructure(snapshot[curSnapshot]);
        // animationSlider->setValue(curSnapshot);
        setSnapshot(curSnapshot);
    }
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