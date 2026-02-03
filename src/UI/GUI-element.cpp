#include "../../include/UI/GUI-element.h"

namespace GUI
{

void IInteractableElement::setWindow(sf::RenderTarget *target_ptr)
{
    this->target_ptr = target_ptr;
}

}