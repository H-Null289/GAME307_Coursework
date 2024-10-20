#pragma once

#include "Character.h"

class Flee
{
	private:
		Body* character;
		Body* target;
		float maxAccel;
	public:
		SteeringOutput getSteering();
		Flee(Body* _character = nullptr, Body* _target = nullptr, float _maxAccel = 1);
};

