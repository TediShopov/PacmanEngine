#include "WindowSFML.h"

 std::optional<sf::Event> WindowSFML::pollEvent()  {
	std::optional<sf::Event> event = window.pollEvent();
	return event;
}

bool WindowSFML::isOpen() const
{
	return this->window.isOpen();
}

void WindowSFML::display() {
	return this->window.display();

}

void WindowSFML::clear() {
	return this->window.clear(sf::Color(0,0,0,0));

}

void WindowSFML::close()
{
	this->window.close();
}

 sf::RenderWindow* WindowSFML::raw() {
	return &this->window;

}
