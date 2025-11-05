#pragma once
#include "ChaseMovementStrategy.h"
//Uses target position/orientation and allies position/orientation to generate pseudo-random, unpredictable behaviours.
class InkyMovementStrategy :
    public ChaseMovementStrategy
{
public:
	InkyMovementStrategy(const GridEntity* target, const GridEntity* ally) : 
		ChaseMovementStrategy(target),
		ally(ally)
	{

			
	}

protected:
	const GridEntity* ally;


    

};

