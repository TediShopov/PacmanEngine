#pragma once
#include <SFML/Graphics.hpp>
#include "WindowSFML.h"
#include "Input.h"

class Engine
{
	const std::string DefaultEngineName = "PacmanEngine";
	const sf::Vector2u DefaultEngineWindowSize = { 448,576 };

public:

	sf::Clock time;
	std::unique_ptr<IWindow> window;
	std::unique_ptr<IInput> input;

	Engine():
		window(std::make_unique<WindowSFML>()),
		input(std::make_unique<Input>())
	{
			
	}

	int run()
	{
		while (window->isOpen()) {
			//$input->pumpEvents(*window);
			input->pumpEvents(*window);
			if (input->isQuitRequested())
				break;

			//time_->tick();
		   // while (time_->stepFixed()) {
		   //	 // later: update logic
		   // }

			//auto& win = static_cast<WindowSFML&>(*window).raw();
			window->clear();
			window->display();
		}

		window->close();
		return 0;


	}




	
};

