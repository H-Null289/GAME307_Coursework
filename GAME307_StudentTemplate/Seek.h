#pragma once

#include "Character.h"

class Seek
{
private:
	Body* character;
	Body* target;
	float maxAccel;
public:
	SteeringOutput getSteering();
	Seek(Body*_character = nullptr, Body*_target = nullptr, float _maxAccel = 1);
};

