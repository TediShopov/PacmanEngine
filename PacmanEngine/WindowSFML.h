#pragma once
#include "IWindow.h"
class WindowSFML : public IWindow
{
	const std::string DefaultEngineName = "PacmanEngine";
	const sf::Vector2u DefaultEngineWindowSize = { 448,576 };
public:
	WindowSFML() : window(sf::VideoMode(DefaultEngineWindowSize),DefaultEngineName)
	{
		window.setKeyRepeatEnabled(false);
			
	}

	virtual std::optional<sf::Event> pollEvent()  override;
	virtual bool isOpen() const;
	virtual void display() override;
	virtual void clear() override;
	virtual void close() override; 
	sf::RenderWindow* raw();
private:
	sf::RenderWindow window;
};

