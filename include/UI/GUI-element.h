#include "GUI-interactable-element.h" 

namespace GUI
{

template<typename T>
class BindableText : public sf::Drawable, public sf::Transformable
{
private:
    sf::Text text;
public:
    BindableText();

    void operator=(const T x);

    void draw(sf::RenderTarget& target, sf::RenderStates state) override;
};

class Node : public sf::Drawable, public sf::Transformable
{

};

class Edge : public sf::Drawable, public sf::Transformable
{

};

};