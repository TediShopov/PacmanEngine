#include "GridEntity.h"
#include "IInput.h"
#include "Grid.h"

void GridEntity::updateInput(const IInput* input)
{
	 float moveSpeed = 0.1;
	 if (input->isKeyDown((int)sf::Keyboard::Key::A))
		 this->worldPos.x += -moveSpeed;
	 else if (input->isKeyDown((int)sf::Keyboard::Key::D))
		 this->worldPos.x += moveSpeed;
	 else if (input->isKeyDown((int)sf::Keyboard::Key::W))
		 this->worldPos.y += -moveSpeed;
	 else if (input->isKeyDown((int)sf::Keyboard::Key::S))
		 this->worldPos.y += moveSpeed;


}

void GridEntity::update(float dt)
{
}

 void GridEntity::draw(sf::RenderWindow& win)
{
	 this->sprite->setPosition(worldPos);
	 win.draw(*this->sprite);
}
