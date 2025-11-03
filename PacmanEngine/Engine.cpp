#include "Engine.h"

inline Engine::Engine() :
	window(std::make_unique<WindowSFML>()),
	input(std::make_unique<Input>())
{

}

 int Engine::run()
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

 void Engine::update()
 {

 }
