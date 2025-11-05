#pragma once
#include "IMovementStrategy.h"
#include "IInput.h"
class InputMovementStrategy :
    public IMovementStrategy
{
public:
    InputMovementStrategy(const IInput* in) : input(in)
    {

    }

    // Inherited via IMovementStrategy
    sf::Vector2i computeDesiredDirection(const GridEntity& self) const override;

private:
    const IInput* input;

};

