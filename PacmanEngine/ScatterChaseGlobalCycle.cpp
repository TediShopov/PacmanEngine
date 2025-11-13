#include "ScatterChaseGlobalCycle.h"

ScatterChaseGlobalCycle::ScatterChaseGlobalCycle() : currentState(Scatter)
{
}

 void ScatterChaseGlobalCycle::init(std::initializer_list<float> scatterChaseTimings)
{
	 this->currentState = Scatter;


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

 void ScatterChaseGlobalCycle::start()
{
	if (scatterChaseSequence.empty() == false)
		scatterChaseSequence.front()->resume();
}

 void ScatterChaseGlobalCycle::stop()
{
	if (scatterChaseSequence.empty() == false)
		scatterChaseSequence.front()->pause();
}

 void ScatterChaseGlobalCycle::fixedUpdate(float dt)
{
	if (scatterChaseSequence.empty() == false)
		scatterChaseSequence.front()->update(dt);
}

 void ScatterChaseGlobalCycle::dequeueTimerOnCompletion(Timer* t)
{
	//Remove timer
	scatterChaseSequence.pop();

	//Toggle states
	if (currentState == Chase)
		currentState = Scatter;
	else if (currentState == Scatter)
		currentState = Chase;

	//Invoke the assigned event
	if (onGlobalStateChanged)
		onGlobalStateChanged(currentState);

	//Start new timer if one exists
	if (scatterChaseSequence.empty() == false)
	{
		Timer& t = *scatterChaseSequence.front().get();
		t.reset();
		t.resume();
	}
}

//Could only be Scatter Or Chase

 GhostStateEnum ScatterChaseGlobalCycle::getCurrentGlobalState()
{
	return currentState;
}