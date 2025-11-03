#pragma once
#include "SFML/Graphics.hpp"
class IWindow {
public:
	virtual ~IWindow() = default;
	virtual std::optional<sf::Event> pollEvent()  = 0 ;
 	virtual bool isOpen() const = 0;
	virtual void display() = 0;
	virtual void clear() = 0;
	virtual void close() = 0;
	
};
