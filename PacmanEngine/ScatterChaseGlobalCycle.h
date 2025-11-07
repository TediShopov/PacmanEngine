#pragma once
#include "Timer.h"
#include "Ghost.h"
#include <queue>
class ScatterChaseGlobalCycle
{
public:
	ScatterChaseGlobalCycle() : currentState(Scatter)
	{

	}
	//Start fomrr
	void init(std::initializer_list<float> scatterChaseTimings)
	{
		while (scatterChaseSequence.empty() != true)
			scatterChaseSequence.pop();

		for (float timeState : scatterChaseTimings)
		{
			
			auto t = std::make_unique<Timer>(timeState);
			t->setLooping(false);
			t->onTimerFinished([this](Timer* t) {dequeueTimerOnCompletion(t); });
			scatterChaseSequence.push(std::move(t));
		}

			


	}
	void start()
	{
		if (scatterChaseSequence.empty() == false)
			scatterChaseSequence.front()->resume();
	}
	void stop()
	{
		if (scatterChaseSequence.empty() == false)
			scatterChaseSequence.front()->pause();
	}
	void fixedUpdate(float dt)
	{
		if (scatterChaseSequence.empty() == false)
			scatterChaseSequence.front()->update(dt);
	}
	void dequeueTimerOnCompletion(Timer* t)
	{
		//Remove timer
		scatterChaseSequence.pop();

		//Toggle states
		if (currentState == Chase)
			currentState = Scatter;
		else if (currentState == Scatter)
			currentState = Chase;

		//Invoke the assigned event
		if(onGlobalStateChanged)
			onGlobalStateChanged(currentState);

		//Start new timer if one exists
		if (scatterChaseSequence.empty() == false)
		{
			Timer& t = *scatterChaseSequence.front().get();
			t.reset();
			t.resume();

		}
		int a = 3;

	}

	//Could only be Scatter Or Chase
	GhostStateEnum getCurrentGlobalState()
	{
		return currentState;
	}

	std::function<void(GhostStateEnum g)> onGlobalStateChanged;
private:
	GhostStateEnum currentState;
	std::queue<std::unique_ptr<Timer>> scatterChaseSequence;
};

