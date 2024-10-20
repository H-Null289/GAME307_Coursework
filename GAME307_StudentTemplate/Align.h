#pragma once

#include "Character.h"

class Align
{
private:
	Body* character;
	Body* target;
	float maxAngularAccel;
	float maxRotation;

	// radius for arriving at the target
	float targetRadius;

	// radius for beginning to slow down
	float slowRadius;

	// time over which to achieve target speed
	float timetoTarget = 0.1;

public:
	SteeringOutput getSteering();
	Align(Body* _character = nullptr, Body* _target = nullptr, 
		float _maxAngularAccel = 1, float _maxRotation = 1,
		float _targetRadius = 1, float _slowRadius = 2, float _timetoTarget = 0.1);
};

