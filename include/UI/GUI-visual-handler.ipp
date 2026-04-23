namespace GUI
{

template<typename T>
requires std::derived_from<T,sf::Drawable>
void PopupGroup::addElement(std::shared_ptr<T> element_ptr)
{
    // std::cerr << "Adding element\n";
    elements.push_back(std::static_pointer_cast<sf::Drawable>(element_ptr));
    if constexpr (std::derived_from<T,IInteractableElement>) {
        iElements.push_back(std::dynamic_pointer_cast<IInteractableElement>(element_ptr));
    }
}

template <typename T>
requires std::derived_from<T,sf::Drawable>
void ZoomView::draw(const T& drawable)
{
    window->setView(view);
    window->draw(drawable);
    window->setView(window->getDefaultView());
}

}