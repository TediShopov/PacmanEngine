#pragma once
#include "Timer.h"
#include "Ghost.h"
#include <queue>

/**
 * @brief Controls the global Scatter/Chase state cycle for all ghosts.
 *
 * Manages a timed sequence of alternating Scatter and Chase phases
 * using internal timers. Notifies listeners via a callback when the
 * global ghost mode changes, ensuring all ghosts remain synchronized
 * to the original Pac-Man timing pattern.
 */
class ScatterChaseGlobalCycle
{
public:
	ScatterChaseGlobalCycle();
	//Start fomrr
	void init(std::initializer_list<float> scatterChaseTimings);
	void start();
	void stop();
	void fixedUpdate(float dt);
	void dequeueTimerOnCompletion(Timer* t);

	//Could only be Scatter Or Chase
	GhostStateEnum getCurrentGlobalState();

	std::function<void(GhostStateEnum g)> onGlobalStateChanged;
private:
	GhostStateEnum currentState;
	std::queue<std::unique_ptr<Timer>> scatterChaseSequence;
};

