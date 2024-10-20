#pragma once

#include "Character.h"

class VelocityMatch
{
private:
	Body* character;
	Body* target;
	float maxAccel;
	float timeToTarget;
public:
	SteeringOutput getSteering();
	VelocityMatch (Body* _character = nullptr, Body* _target = nullptr, float _maxAccel = 1, float _timeToTarget = 0.1);
};

